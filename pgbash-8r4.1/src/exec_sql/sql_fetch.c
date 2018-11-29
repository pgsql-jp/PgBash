/*----------------------------------------------------------------
 * sql_fetch.c:
 *	The operation in the ftech Into statement.
 *
 * Change Logs
 * 2001.04.15: Delete read_only_check of shell variable.
 * 2001.06.11: Add SELECT INTO.
 *
 *-----------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "pg_config_manual.h"
#include "libpq-fe.h"

/*-----------------------------------------------------------------
 * The definition of sql_fetch.
 *---------------------------------------------------------------*/
#include "sql_fetch.h"
#include "sql_errno.h"
#include "utils.h"


/*-----------------------------------------------------------------
 * The function list.
 *-----------------------------------------------------------------
 * int exec_fetchInto()         : bind host variable and SHELL variable.
 * int parse_select_fetch_into(): parse select/fetch into statement.
 * void free_PSfetchList()      : free PSfetchList memory.
 */
static int parse_selectInto(char *select_stmt, PSfetchList **pfet0);
static int parse_fetchInto(char *fetch_stmt, PSfetchList **pfet0);
static int host_indi_name(char *p, char *p2, char *name);


/*
 * ############################################################### 
 * The global routine of sql_fetch.
 * ###############################################################
 */

int
exec_fetchInto(PSconnSet * pset, PGresult * res, PSfetchList * pfet0)
/*****************************************************************
 * The host variable is substituted for the shell variable.
 *
 *   return(0)  : normal end
 *   return(100): EOF
 *   return(-1) : bind_variable error
 ****************************************************************/
{
	int             nFields;
	int             nTups;
	char           *value;
	PSfetchList    *pfet = pfet0;
	int             j, i = 0;

	/*----------- number of field -----------------------*/
	nFields = PQnfields(res);

	/*----- only set rows with at least 1 field.  -------*/
	if (nFields <= 0)
		return (-1);

	/*------ check number of tuples ---------------------*/
	nTups = PQntuples(res);
	if (nTups == 0)
	{
		setSQLCA(pset->sqlca, SQL_NOT_FOUND, "", 0, nFields);
		return (100);	/* EOF */
	}
	/*------ bind hostvariables -------------------------*/
	for (j = 0; j < nFields; j++)
	{
		if (pfet == NULL)
		{
			setSQLCA(pset->sqlca, SQL_TOO_FEW_ARGUMENTS,
			       "Error: too few arguments.", nTups, nFields);
			if (!pset->quiet)
				printSQLerror(pset->fout, pset->sqlca);
			break;
		}
		/*---------- get value ----------------*/
		value = (char *) PQgetvalue(res, i, j);

		/*---------- bind host variable ---------*/
		if (wrap_bind_char_variable(pfet->hostvar, value) < 0)
		{
			setSQLCA(pset->sqlca, SQL_SYSTEM_ERROR,
				 getSQLerrmc("bind_variable(%s)", pfet->hostvar), 0, 0);
			if (!pset->quiet)
				printSQLerror(get_Ferr(), pset->sqlca);
			return (-1);
		}
		/*---------------- indicator ------------------*/
		if (pfet->indicat != NULL)
		{
			/*------- bind indicator variable ------*/
			if (wrap_bind_int_variable(pfet->indicat, -PQgetisnull(res, i, j)) < 0)
			{
				setSQLCA(pset->sqlca, SQL_SYSTEM_ERROR,
					 getSQLerrmc("bind_variable(%s)", pfet->indicat), 0, 0);
				if (!pset->quiet)
					printSQLerror(get_Ferr(), pset->sqlca);
				return (-1);
			} 
		}
		/*---------- next host variable --------*/
		pfet = pfet->next;
	}

	/*----------- check arguments ------------------*/
	if (pfet != NULL)
	{
		setSQLCA(pset->sqlca, SQL_TOO_MANY_ARGUMENTS,
			 "Error: too many arguments.", nTups, nFields);
		if (!pset->quiet)
			printSQLerror(pset->fout, pset->sqlca);
	}
	return (0);
}


int
parse_select_fetch_into(char *stmt, PSfetchList ** pfet0)
{
	if (strUcmp(stmt, "SELECT ", 7)==0)
	{
		return( parse_selectInto(stmt, pfet0) );
	}
	else if (strUcmp(stmt, "FETCH ", 6)==0)
	{
		return(  parse_fetchInto(stmt, pfet0) );
	}
        else
	{
		*pfet0 = NULL;
		return(-1);
	}
}


static int
parse_selectInto(char *select_stmt, PSfetchList ** pfet0)
/*******************************************************************
 * The syntactic analysis of SELECT INTO statement.
 *
 *  ex.
 *   input :
 *     select_stmt="select a,b into :a:ind1,:b:ind2  from xxx where ..;
 *  output:
 *     select_stmt="select a,b from xxx where.."
 *     pfet      = host/indicator variables list
 *     pinto     = " INTO " pointer
 *
 *   return( 0) : normal end
 *   return(-1) : syntax error
 *   return(-2) : host name error
 *   return(-3) : indicator name error
 ******************************************************************/
{
	PSfetchList    *pfet, *pfet1;
	char           *p, *p2;
	char            name[NAMEDATALEN];
	char		*pinto=NULL, *pfrom=NULL;

	*pfet0 = NULL;

	/*------------ check SQL statement --------------*/
	if (select_stmt == NULL)
		return (-1);

	/*------------ initial set ----------------------*/
	pfet = pfet1 = NULL;

	/*------------ search  "SELECT" ----------------*/
	p = select_stmt;
	p = skipSpace(p);
	p2 = skipToken(p);

	if (p2 - p != 6 || strUcmp(p, "SELECT", 6) != 0)
		return (-1);

	/*------------ search  "into" ---------------------*/
	while (*p2 != '\0' && *p2 != ';' && *p != ':')
	{
		p = p2;
		p = skipSpace(p);
		p2 = p; while(*p2!='\0' && *p2!=' ') p2++;

		if (p2 - p == 4 && strUcmp(p, "INTO", 4) == 0)
		{
			char *p3;
			if ((p3=skipSpace(p2))!=NULL && (*p3!=':'))
					continue;

			pinto = p;
			break;
		}
		if (p2 - p == 4 && strUcmp(p, "FROM", 4) == 0)
			return(-1);
	}

	if (pinto == NULL)
		return (-1);

	/*-------- set host/indicator variables ---------*/
	while (1)
	{
		p = p2;
		p = skipSpace(p);
		p2 = skipToken(p);

		/*----------------------------------------
		   	    host variable
		-----------------------------------------*/
		if (*p != ':')
		{
			free_PSfetchList(*pfet0);
			return (-1);
		}
		/*----- check host variable ------*/
		p++;
		if (host_indi_name(p, p2, name) < 0)
		{
			free_PSfetchList(*pfet0);
			return (-2);
		}
		/*------ malloc host variable -----*/
		pfet = (PSfetchList *) wrapMalloc(sizeof(PSfetchList));

		/*------ set host variable --------*/
		pfet->hostvar = wrapStrdup(name);
		pfet->indicat = NULL;
		pfet->next = NULL;

		/*------ set head pointer ---------*/
		if (*pfet0 == NULL)
			*pfet0 = pfet;

		/*------ set next pointer ---------*/
		if (pfet1 != NULL)
			pfet1->next = pfet;
		pfet1 = pfet;

		/*----------------------------------------
	   		    indicator variable
		-----------------------------------------*/
		p = p2;
		p = skipSpace(p);
		p2 = skipToken(p);

		if (strUcmp(p, "FROM", 4)==0 )
		{
			pfrom = p;
			break;
		}

		if (p2 - p == 9 && strUcmp(p, "INDICATOR", 9) == 0)
		{
			p = p2;
			p = skipSpace(p);
			p2 = skipToken(p);
		}
		if (*p == ':')
		{
			/*----- check indicator -----*/
			p++;
			if (host_indi_name(p, p2, name) < 0)
			{
				free_PSfetchList(*pfet0);
				return (-3);
			}
			/*------ set indicator --------*/
			pfet->indicat = wrapStrdup(name);
		}
		/*----------- end of statement -----------*/
		p2 = skipSpace(p2);
		if (*p2 == '\0' || *p2 == ';')
			break;

		/*----------- next host variable ---------*/
		if (*p2 == ',')
		{
			p2++;
		} else
		{
			free_PSfetchList(*pfet0);
			return (-1);
		}
	}

	/*------------------ delete INTO clause ------------------*/
	if (pinto != NULL && pfrom != NULL)
	{
		char *sql_select;

		sql_select = (char *)wrapMalloc(strlen(select_stmt));
		strncpy(sql_select, select_stmt, (pinto-select_stmt));
		strcpy(sql_select+(pinto-select_stmt), pfrom);
		strcpy(select_stmt, sql_select);
		free(sql_select);
	}
	else if (pinto != NULL && pfrom == NULL) 
	{
		*pinto = '\0';
	}

	return (0);
}

static int
parse_fetchInto(char *fetch_stmt, PSfetchList ** pfet0)
/*******************************************************************
 * The syntactic analysis of fetch statement.
 *
 *  ex.
 *   input :
 *     fetch_stmt="fetch forward in cur into :hst1:ind1, :hst2, :hst3:ind3"
 *  output:
 *     fetch_stmt="fetch in cur"
 *     pfet      = host/indicator variables list
 *     pinto     = " INTO " pointer
 *
 *   return( 0) : normal end
 *   return(-1) : syntax error
 *   return(-2) : host name error
 *   return(-3) : indicator name error
 ******************************************************************/
{
	PSfetchList    *pfet, *pfet1;
	char           *p, *p2;
	char            name[NAMEDATALEN];
	char		*pinto=NULL;

	*pfet0 = NULL;

	/*------------ check SQL statement --------------*/
	if (fetch_stmt == NULL)
		return (-1);

	/*------------ initial set ----------------------*/
	pfet = pfet1 = NULL;

	/*------------ search  "fetch" ----------------*/
	p = fetch_stmt;
	p = skipSpace(p);
	p2 = skipToken(p);

	if (p2 - p != 5 || strUcmp(p, "FETCH", 5) != 0)
		return (-1);

	/*------------ search  "into" ---------------------*/
	while (*p2 != '\0' && *p2 != ';' && *p != ':')
	{
		p = p2;
		p = skipSpace(p);
		p2 = skipToken(p);

		if (p2 - p == 4 && strUcmp(p, "INTO", 4) == 0)
		{
			pinto = p;
			break;
		}
	}

	if (pinto == NULL)
		return (-1);

	/*-------- set host_variables/indicator ---------*/
	while (1)
	{
		p = p2;
		p = skipSpace(p);
		p2 = skipToken(p);

		/*----------------------------------------
		   	    host variable
		-----------------------------------------*/
		if (*p != ':')
		{
			free_PSfetchList(*pfet0);
			return (-1);
		}
		/*----- check host variable ------*/
		p++;
		if (host_indi_name(p, p2, name) < 0)
		{
			free_PSfetchList(*pfet0);
			return (-2);
		}
		/*------ malloc host variable -----*/
		pfet = (PSfetchList *) wrapMalloc(sizeof(PSfetchList));

		/*------ set host variable --------*/
		pfet->hostvar = wrapStrdup(name);
		pfet->indicat = NULL;
		pfet->next = NULL;

		/*------ set head pointer ---------*/
		if (*pfet0 == NULL)
			*pfet0 = pfet;

		/*------ set next pointer ---------*/
		if (pfet1 != NULL)
			pfet1->next = pfet;
		pfet1 = pfet;

		/*----------------------------------------
	   		    indicator variable
		-----------------------------------------*/
		p = p2;
		p = skipSpace(p);
		p2 = skipToken(p);

		if (p2 - p == 9 && strUcmp(p, "INDICATOR", 9) == 0)
		{
			p = p2;
			p = skipSpace(p);
			p2 = skipToken(p);
		}
		if (*p == ':')
		{
			/*----- check indicator -----*/
			p++;
			if (host_indi_name(p, p2, name) < 0)
			{
				free_PSfetchList(*pfet0);
				return (-3);
			}
			/*------ set indicator --------*/
			pfet->indicat = wrapStrdup(name);
		}
		/*----------- end of statement -----------*/
		p2 = skipSpace(p2);
		if (*p2 == '\0' || *p2 == ';')
			break;

		/*----------- next host variable ---------*/
		if (*p2 == ',')
		{
			p2++;
		} else
		{
			free_PSfetchList(*pfet0);
			return (-1);
		}
	}

	/*--- delete INTO clause ---*/
	if (pinto != NULL )
		*pinto = '\0';

	return (0);
}

static int
host_indi_name(char *p, char *p2, char *name)
{
	/*----- check variable len --------*/
	if (p2 - p > NAMEDATALEN - 1)
		return (-1);

	/*------ copy variable name -------*/
	strncpy(name, p, (p2 - p));
	name[p2 - p] = '\0';

	/*------ check variable name ------*/
	if (checkName(name) < 0)
		return (-1);

	return (0);
}


void
free_PSfetchList(PSfetchList * pfet0)
/*****************************************************************
 *  A memory release of PSfetchList.
 ****************************************************************/
{
	PSfetchList    *pfet = pfet0, *pfetnext;

	while (pfet != NULL)
	{
		if (pfet->hostvar != NULL)
			free(pfet->hostvar);
		if (pfet->indicat != NULL)
			free(pfet->indicat);
		pfetnext = pfet->next;
		free(pfet);
		pfet = pfetnext;
	}
}
