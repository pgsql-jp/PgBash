/*-------------------------------------------------------------------------
 *
 * psql_sub.c
 *	  A sub set of psql(an interactive front-end)
 *
 * Copyright (c) 1996, Regents of the University of California
 * Copyright 2000 by PostgreSQL Global Development Group
 *
 * This software was modified from "psql.c" by SAKAIDA Masaaki --
 * Osaka,Japan.
 *
 *
 * Change logs
 * 2000.03.22: Change PQmblen(s) to PSmblen(s,encoding)	
 * 2000.03.23: Change (dF)FunctionsList query for PostgreSQL ver 6 and 7.
 * 2000.04.21: Change that SQLCA is not set when END is issued.
 * 2000.04.28: Change list All DBs to new query.
 * 2001.04.06: Delete "mb.h". Delete PSmblen().
 * 2001.04.15: Delete SendCommand() and backslash_routines.
 * 2001.04.15: Add large_object routines.
 * 2001.06.11: Add <BR> tag into a SQL parse error message.
 * 2001.06.11: Add SELECT INTO(parse_select_fetch_into).
 * 2001.06.11: Add PGBASH_DESCRIPTION table.
 * 2011.08.10: Add the parameter 0 in bind_array_variable() and bind_variable().
 * 2011.08.15: Change bind_array_variable() to wrap_bind_array_variable().
 *
 *-------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#include "libpq-fe.h"

#include "psql_sub.h"
#include "sql_fetch.h"
#include "utils.h"
#include "sql_errno.h"
#include "sql_set.h"

#define PGBASH_DESCRIPTION		"pgbash_description"
#define PGBASH_DESCRIPTION_OBJOID_INDEX	"pgbash_description_index"


/*------------------------------------------------------------------
 *  Function List
 *------------------------------------------------------------------
 * bool SendQuery(PSconnSet *pset, char *query);
 * int lo_exec(char *prog_name, char *parameter);
 */
static bool handleCopyOut(PGconn * conn, FILE * copystream);
static bool handleCopyIn(PGconn * conn, const bool mustprompt, 
						FILE * copystream);
static bool do_lo_export(PSconnSet *pset, const char *loid_arg,
 					const char *filename_arg);
static int  do_lo_import(PSconnSet *pset, const char *filename_arg, 
 					const char *comment_arg);
static bool do_lo_unlink(PSconnSet *pset, const char *loid_arg);
static bool param_lo_export(char *parameter, char *oid, char *filenm);
static bool param_lo_import(char *parameter, char *filenm, char *comment);
static bool param_lo_unlink(char *parameter, char *oid);

static bool create_pgbash_description(PSconnSet *pset);

/*
 * ################################################################
 * psql_sub routines
 * ################################################################
 */

bool
SendQuery(PSconnSet * pset, char *query)
/*******************************************************************
 * send the query string to the backend.
 *
 *  return(true)  : normal end
 *  return(false) : error
 ******************************************************************/
{
	bool            success = false;
	PGresult       *results;
	PGnotify       *notify;
	FILE           *fout = pset->fout;
	PSfetchList    *pfet = NULL;
	int             fetflg = -1;
	const char     *r_oid = NULL;
	int		query_end = 0;

	/*-------- if query is "END" statement -----------*/
	if( strUcmp(query, "END", 3 ) == 0 ) 
	{
		query_end = 1;        /* SQLCA is not set */
	}

	/*-------- echo query sent to the backend --------*/
	if (pset->echoQuery)
	{
		fprintf(fout, "########## QUERY ##########%s\n%s%s\n",
				get_CGI_BR(), query, get_CGI_BR());
		fprintf(fout, "###########################%s\n", get_CGI_BR());
		fprintf(fout, "%s\n", get_CGI_BR());
		fflush(fout);
	}
	/*-------------- SELECT/FETCH INTO ---------------*/
	fetflg = parse_select_fetch_into(query, &pfet);

	/*-------- clear OID shell variable --------------*/
	bind_variable(SQLOID, "", 0);

	/*-------------- execute query -------------------*/
	results = PQexec(pset->dbset->db, query);

	/*-------------- query error ---------------------*/
	if (results == NULL)
	{
		setSQLCA(pset->sqlca, SQL_BAD_RESPONSE,
			 PQerrorMessage(pset->dbset->db), 0, 0);
		printSQLerror(get_Ferr(), pset->sqlca);

		success = false;
		return success;
	}
	/*-------------- query OK! -----------------------*/
	switch (PQresultStatus(results))
	{

	case PGRES_TUPLES_OK:
		if( query_end == 0 )
			setSQLCA(pset->sqlca, SQL_OK, "",
				 PQntuples(results), PQnfields(results));
		success = true;

		/*-------- SELECT query -------------*/
		if (fetflg == -1 || PQntuples(results) > 1)
		{
			/*---- print out ------*/
			alloc_printOpt(&(pset->opt), results);
			PSprint(pset->fout, &(pset->opt), pset->dbset->encoding);
			free_printOpt(&(pset->opt));

			fflush(pset->fout);
		}
		/*--------- SELECT/FETCH INTO --------*/
		else
		{
			if (exec_fetchInto(pset, results, pfet) == 0)
				(void) free_PSfetchList(pfet);
			else
				success = false;
		}

		/*---- set Field Name to SHELL var ---*/
		if (PQnfields(results) >= 1)
		{
			int             i;
			for (i = 0; i < PQnfields(results); i++)
				wrap_bind_array_variable("SQLFIELDNAME", i, PQfname(results, i), 0);
		}
		break;

	case PGRES_EMPTY_QUERY:
		if( query_end == 0 )
			setSQLCA(pset->sqlca, SQL_EMPTY_QUERY, "",
				 PQntuples(results), PQnfields(results));
		success = true;
		break;

	case PGRES_COMMAND_OK:
		if( query_end == 0 )
			setSQLCA(pset->sqlca, SQL_OK, "",
				 PQntuples(results), PQnfields(results));

		/*-------- OID of recent insert -----*/
		r_oid = PQoidStatus(results);
		if (r_oid != NULL && r_oid[0] != '\0')
			bind_variable(SQLOID, (char *) r_oid, 0);

		/*-------- set client_encoding to local DB pointer -----*/
		set_client_encoding(pset, query);

		success = true;

		if (!pset->quiet && !pset->notty)
			printf("%s%s\n", PQcmdStatus(results), get_CGI_BR());
		break;

	case PGRES_COPY_OUT:  /* copy TABLE to STDOUT; */  
		setSQLCA(pset->sqlca, SQL_OK, "",
			 PQntuples(results), PQnfields(results));
		success = handleCopyOut(pset->dbset->db, pset->fout);

		if( !success )
			setSQLCA(pset->sqlca, SQL_INVALID_DATA,
				PQerrorMessage(pset->dbset->db), 0, 0);
		fflush(pset->fout);
		break;

	case PGRES_COPY_IN:  /* copy TABLE from STDIN; */
		setSQLCA(pset->sqlca, SQL_OK, "",
			 PQntuples(results), PQnfields(results));
		if (!pset->quiet && !pset->notty)
			success = handleCopyIn(pset->dbset->db, true, stdin);
		else
			success = handleCopyIn(pset->dbset->db, false, stdin);

		if( !success )
			setSQLCA(pset->sqlca, SQL_INVALID_DATA,
				PQerrorMessage(pset->dbset->db), 0, 0);
		break;

	case PGRES_NONFATAL_ERROR:
		setSQLCA(pset->sqlca, SQL_NONFATAL_ERROR,
			 PQerrorMessage(pset->dbset->db), 0, 0);
		if (!pset->quiet)
			printSQLerror(pset->fout, pset->sqlca);
		success = false;
		break;

	case PGRES_FATAL_ERROR:
		setSQLCA(pset->sqlca, SQL_FATAL_ERROR,
			 PQerrorMessage(pset->dbset->db), 0, 0);
		if (!pset->quiet)
		{
			if (strstr(pset->sqlca->sqlerrm.sqlerrmc, "parser") != NULL)
				fprintf(pset->fout, "Error  SQL :%s\n%s%s\n%s\n",
					get_CGI_BR(), query, get_CGI_BR(), get_CGI_BR());
			printSQLerror(pset->fout, pset->sqlca);
		}
		success = false;
		break;

	case PGRES_BAD_RESPONSE:
		setSQLCA(pset->sqlca, SQL_BAD_RESPONSE,
			 PQerrorMessage(pset->dbset->db), 0, 0);
		if (!pset->quiet)
			printSQLerror(pset->fout, pset->sqlca);
		success = false;
		break;
	}

	/*------------- connection bad ----------------------*/
	if (PQstatus(pset->dbset->db) == CONNECTION_BAD)
	{
		setSQLCA(pset->sqlca, SQL_CONNECTION_BAD, "\
  We have lost the connection to the backend, so\n\
  further processing is impossible. \n\
  Terminating.", 0, 0);

		printSQLerror(get_Ferr(), pset->sqlca);

		/*******/
		exit(2);
		/*******/
	}
	/*---------- check for asynchronous returns ----------*/
	while ((notify = PQnotifies(pset->dbset->db)) != NULL)
	{
		char            buf[512];

		sprintf(buf, "ASYNC NOTIFY of '%s' from backend pid '%d' received.", 
						notify->relname, notify->be_pid);
		setSQLCA(pset->sqlca, SQL_CONNECTION_BAD, buf, 0, 0);

		printSQLerror(get_Ferr(), pset->sqlca);

		free(notify);
	}

	if (results)
		PQclear(results);

	return success;
}


/*##########################################################################
 * 'copy TABLE {to|from} {STDOUT|STDIN};' command
 * (if you use a 'copy TABLE(col1,..)' command, please look at "sql_copy.c")
 *##########################################################################
 */

static          bool
handleCopyOut(PGconn * conn, FILE * copystream)
/******************************************************************
 *  copy TABLE to STDOUT;
 *****************************************************************/
{
	bool            copydone;
	char            copybuf[COPYBUFSIZ];
	int             ret;

	copydone = false;

	while (!copydone)
	{
		ret = PQgetline(conn, copybuf, COPYBUFSIZ);

		if (copybuf[0] == '\\' &&
		    copybuf[1] == '.' &&
		    copybuf[2] == '\0')
		{
			copydone = true;	/* don't print this... */
		} else
		{
			fputs(copybuf, copystream);

			switch (ret)
			{
			case EOF:
				copydone = true;
				/* FALLTHROUGH */
			case 0:
				fputc('\n', copystream);
				break;
			case 1:
				break;
			}
		}
	}
	fflush(copystream);
	return !PQendcopy(conn);
}



static          bool
handleCopyIn(PGconn * conn, const bool mustprompt, FILE * copystream)
/******************************************************************
 *  copy TABLE from STDIN;
 *****************************************************************/
{
	bool            copydone = false;
	bool            firstload;
	bool            linedone;
	char            copybuf[COPYBUFSIZ];
	char           *s;
	int             buflen;
	int             c = 0;

	if (mustprompt)
	{
		fputs("Enter data separated with a [Tab] followed by a newline\n", stdout);
		fputs("End with a '\\.' \n", stdout);
	}
	/*--------- for each input line -----------*/
	while (!copydone)
	{
		if (mustprompt)
		{
			fputs(">> ", stdout);
			fflush(stdout);
		}
		firstload = true;
		linedone = false;

		/*----- for each buffer -----*/
		while (!linedone)
		{
			s = copybuf;

			for (buflen = COPYBUFSIZ; buflen > 1; buflen--)
			{
				c = getc(copystream);
				if (c == '\n' || c == EOF)
				{
					linedone = true;
					break;
				}
				*s++ = c;
			}
			*s = '\0';
			if (c == EOF)
			{
				PQputline(conn, "\\.");
				copydone = true;
				break;
			}
			PQputline(conn, copybuf);

			if (firstload)
			{
				if (!strcmp(copybuf, "\\."))
					copydone = true;
				firstload = false;
			}
		}
		PQputline(conn, "\n");
	}
	return !PQendcopy(conn);
}


/*##########################################################################
 *  latge object routines
 *##########################################################################
 */

int  
lo_exec(PSconnSet *pset, char *prog_name, char *parameter)
/*************************************************************************
 *  Main routine for executing large_object routines.
 *
 * prog_name: e.g. "lo_export", "lo_import", "lo_unlink"
 * parameter: lo_import-- e.g. "'/tmp/test.dat', 'test file'"
 *	      lo_export-- e.g. "12345, '/tmp/test.dat'"
 *	      lo_unlink-- e.g. "12345"
 ************************************************************************/
{
	int	i_oid;
	char	oid[32];
	char	filenm[256];
	char	comment[256];

	if( strcmp(prog_name,"lo_export")==0 )
	{
		if(param_lo_export(parameter, oid, filenm))
		{
			if(do_lo_export(pset, oid, filenm))
				return(0);
			else
				return(-2);
		}
		else
			return(-1);
	}
	else if( strcmp(prog_name,"lo_import")==0 )
	{
		if(param_lo_import(parameter, filenm, comment))
		{
			if(i_oid=do_lo_import(pset, filenm, comment) > 0)
			{
				sprintf(oid, "%d", i_oid);
				bind_variable(SQLOID, oid, 0);
				return(0);
			}
			else
				return(-2);
		}
		else
			return(-1);
	}
	else if( strcmp(prog_name,"lo_unlink")==0 )
	{
		if(param_lo_unlink(parameter, oid))
		{
			if(do_lo_unlink(pset, oid))
				return(0);
			else
				return(-2);
		}
		else
			return(-1);
	}
	else
		return(-1);
}


bool
do_lo_export(PSconnSet *pset, const char *loid_arg, const char *filename_arg)
/***************************************************************************
 * Write a large object(loid_arg) to a file
 **************************************************************************/
{
	PGresult	*res;
	int		status;
	bool		own_transaction = true;

	if (own_transaction)
	{
		if (!SendQuery(pset, "BEGIN"))
			return false;
	}

	status = lo_export(pset->dbset->db, atol(loid_arg), filename_arg);

	if (status != 1)
	{			/* of course this status is documented
							 * nowhere :( */
		setSQLCA(pset->sqlca, SQL_FATAL_ERROR,
			 PQerrorMessage(pset->dbset->db), 0, 0);

		if (own_transaction)
		{
			res = PQexec(pset->dbset->db, "ROLLBACK");
			PQclear(res);
		}
		return false;
	}

	if (own_transaction)
	{
		if (!SendQuery(pset, "COMMIT"))
		{
			res = PQexec(pset->dbset->db, "ROLLBACK");
			PQclear(res);
			return false;
		}
	}

	return true;
}


int  
do_lo_import(PSconnSet *pset, const char *filename_arg, const char *comment_arg)
/******************************************************************************
 * Copy large object from file to database
 *
 * return value > 0 : OID
 * return value < 0 : error
 *****************************************************************************/
{
	PGresult	*res;
	Oid		loid;
	char		buf[1024];
	unsigned int 	i;
	bool		own_transaction = true;

	/*----------- create PGBASH_DESCRIPTION table -----------*/
	if (!pset->dbset->exist_des)
	{
		if (!create_pgbash_description(pset))
			return -1;
	}

	/*----------- execute INSERT query ----------------------*/
	if (own_transaction)
	{
		if (!SendQuery(pset, "BEGIN"))
			return -1;
	}

	loid = lo_import(pset->dbset->db, filename_arg);

	if (loid == 0)   /* InvalidOid */
	{
		setSQLCA(pset->sqlca, SQL_FATAL_ERROR,
			 PQerrorMessage(pset->dbset->db), 0, 0);

		if (own_transaction)
		{
			res = PQexec(pset->dbset->db, "ROLLBACK");
			PQclear(res);
		}
		return -1;
	}

	/* insert description if given */
	if (comment_arg)
	{
		//*sprintf(buf, "INSERT INTO PGBASH_DESCRIPTION VALUES (%u, '", loid);*//
		sprintf(buf, "COMMENT ON LARGE OBJECT %u IS '", loid);

		for (i = 0; i < strlen(comment_arg); i++)
			if (comment_arg[i] == '\'')
				strcat(buf, "\\'");
			else
				strncat(buf, &comment_arg[i], 1);

		//*strcat(buf, "')");*//
		strcat(buf, "'");

		if (!SendQuery(pset, buf))
		{
			if (own_transaction)
			{
				res = PQexec(pset->dbset->db, "ROLLBACK");
				PQclear(res);
			}
			return -1;
		}
	}

	if (own_transaction)
	{
		if (!SendQuery(pset, "COMMIT"))
		{
			res = PQexec(pset->dbset->db, "ROLLBACK");
			PQclear(res);
			return -1;
		}
	}

	return loid;
}


bool
do_lo_unlink(PSconnSet *pset, const char *loid_arg)
/*********************************************************************
 * removes a large object out of the database
 ********************************************************************/
{
	PGresult	*res;
	int		status;
	Oid		loid = (Oid) atol(loid_arg);
	char		buf[256];
	bool		own_transaction = true;

	if (own_transaction)
	{
		if (!SendQuery(pset, "BEGIN"))
			return false;
	}

	status = lo_unlink(pset->dbset->db, loid);

	if (status == -1)
	{
		setSQLCA(pset->sqlca, SQL_FATAL_ERROR,
			 PQerrorMessage(pset->dbset->db), 0, 0);

		if (own_transaction)
		{
			res = PQexec(pset->dbset->db, "ROLLBACK");
			PQclear(res);
		}
		return false;
	}

	/* remove the comment as well */
	sprintf(buf, "DELETE FROM PGBASH_DESCRIPTION WHERE objoid = %u", loid);

	if (!SendQuery(pset, buf))
	{
		if (own_transaction)
		{
			res = PQexec(pset->dbset->db, "ROLLBACK");
			PQclear(res);
		}
		return false;
	}

	if (own_transaction)
	{
		if (!SendQuery(pset, "COMMIT"))
		{
			res = PQexec(pset->dbset->db, "ROLLBACK");
			PQclear(res);
			return false;
		}
	}

	return true;
}


static bool
param_lo_export(char *parameter, char *oid, char *filenm)
/**************************************************************
 *  parse parameter (parameter--> oid ,filnm)
 *************************************************************/
{
	char	*p = parameter;
	char	*p2;

	/*------- OID --------*/
	p = skipSpace(p);
	p2= skipToken(p);
	if( p2-p > 32 || p2-p == 0 )
		return(false);

	strncpy(oid, p, (p2-p));
	oid[(p2-p)] = '\0';

	/*------- ',' --------*/
	p  = skipSpace(p2);
	if( *p != ',' )
		return(false);
	p++;

	/*------- '\'' --------*/
	p  = skipSpace(p);
	if ( *p == '\0' || *p != '\'' )
		return(false);
	p++;
	p2 = strchr(p, '\'');
	if (p2 == NULL || *p2 != '\'' )
		return(false);

	/*------- FILE name --------*/
	if( p2-p > 255)
		return(false);
	strncpy(filenm, p, (p2-p));
	filenm[(p2-p)] = '\0';

	return(true);
}

static bool
param_lo_import(char *parameter, char *filenm, char *comment)
/**************************************************************
 *  parse parameter (parameter--> filnm, comment)
 *************************************************************/
{
	char	*p = parameter;
	char	*p2;

	/*-------- FILE name --------*/
	p  = skipSpace(p);
	if ( *p == '\0' || *p != '\'' )
		return(false);
	p++;
	p2 = strchr(p, '\'');
	if (p2 == NULL || *p2 != '\'' )
		return(false);
	if( p2-p > 255 || p2-p == 0 )
		return(false);

	strncpy(filenm, p, (p2-p));
	filenm[(p2-p)] = '\0';

	p2++;

	/*-------- ',' ------------*/
	p  = skipSpace(p2);
	if( *p != ',' )
		return(false);
	p++;

	/*-------- COMMENT --------*/
	p  = skipSpace(p);
	if ( *p == '\0' || *p != '\'' )
		return(false);
	p++;
	p2 = strchr(p, '\'');
	if (p2 == NULL || *p2 != '\'' )
		return(false);
	if( p2-p > 255 || p2-p == 0 )
		return(false);

	strncpy(comment, p, (p2-p));
	comment[(p2-p)] = '\0';

	return(true);
}

static bool
param_lo_unlink(char *parameter, char *oid)
/**************************************************************
 *  parse parameter (parameter--> oid)
 *************************************************************/
{
	char	*p = parameter;
	char	*p2;

	/*-------- OID --------*/
	p = skipSpace(p);
	p2= skipToken(p);
	if( p2-p > 32 || p2-p == 0 )
		return(false);

	strncpy(oid, p, (p2-p));
	oid[(p2-p)] = '\0';

	return(true);
}

static bool
create_pgbash_description(PSconnSet *pset)
/**************************************************************
 *  create PGBASH_DESCRIPTION table.
 *************************************************************/
{
	PGresult	*res;
	char		buf[256];

	/*--------- select pgbash_description -----*/
	strcpy(buf, "select count(*) from pg_class where relname='");
	strcat(buf, PGBASH_DESCRIPTION);
	strcat(buf, "'");

	res = PQexec(pset->dbset->db, buf);

	if (res == NULL)
	{
		setSQLCA(pset->sqlca, SQL_BAD_RESPONSE,
			 PQerrorMessage(pset->dbset->db), 0, 0);
		printSQLerror(get_Ferr(), pset->sqlca);
		return false;
	}
	if (PQresultStatus(res) == PGRES_TUPLES_OK &&
				atoi(PQgetvalue(res,0,0))==1)
	{
		PQclear(res);
		pset->dbset->exist_des = true;
		return true;
	}
	PQclear(res);

	/*--------- create CREATE query -----------*/
	strcpy(buf, "Create Table ");
	strcat(buf, PGBASH_DESCRIPTION);
	strcat(buf, "(objoid oid, description text)");

	/*---------- execute CREATE TABLE ---------*/
	res = PQexec(pset->dbset->db, buf);

	if (res == NULL)
	{
		setSQLCA(pset->sqlca, SQL_BAD_RESPONSE,
			 PQerrorMessage(pset->dbset->db), 0, 0);
		printSQLerror(get_Ferr(), pset->sqlca);
		return false;
	}
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		setSQLCA(pset->sqlca, SQL_BAD_RESPONSE,
			 PQerrorMessage(pset->dbset->db), 0, 0);
		printSQLerror(get_Ferr(), pset->sqlca);
		return false;
	}
	PQclear(res);

	/*--------- craete INDEX ------------------*/
	strcpy(buf, "Create Index ");
	strcat(buf, PGBASH_DESCRIPTION_OBJOID_INDEX);
	strcat(buf, " on ");
	strcat(buf, PGBASH_DESCRIPTION);
	strcat(buf, "(objoid)");

	/*---------- execute CREATE TABLE ---------*/
	res = PQexec(pset->dbset->db, buf);

	if (res == NULL)
	{
		setSQLCA(pset->sqlca, SQL_BAD_RESPONSE,
			 PQerrorMessage(pset->dbset->db), 0, 0);
		printSQLerror(get_Ferr(), pset->sqlca);
		return false;
	}
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		setSQLCA(pset->sqlca, SQL_BAD_RESPONSE,
			 PQerrorMessage(pset->dbset->db), 0, 0);
		printSQLerror(get_Ferr(), pset->sqlca);
		return false;
	}
	PQclear(res);

	pset->dbset->exist_des = true;
	return true;
}

