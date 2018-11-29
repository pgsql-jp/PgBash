/*----------------------------------------------------------------
 *
 * sql_connect.c:
 *	The operation on the connection to the database.
 *
 * Change logs
 * 2000.03.23: Add PQenv2encoding() at exec_connect().
 *             Add getVersion(),printVersion(),printConnect().
 *             Change PGconn to PSconn at PSconnList.
 * 2001.04.06: Delete "mb.h"
 * 2001.04.15: Change return(number) in three parse_connect routines.
 * 2001.04.15: Add setSQLCA() in parser routines.
 * 2001.04.15: Change PQenv2encoding() to PSenv2encoding().
 * 2001.06.11: Fix printSQLerror(stderr,..) to printSQLerror(pset->fout,..).
 * 2001.12.06: Add "IDENTIFIED BY | USING | /" at the password syntax.
 * 2002.02.10: Add the encoding when printing connection status.
 * 2011.08.10: Change prompt_for_password() to getpass().
 *
 *
 *-----------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#include "libpq-fe.h"

#include "exec_sql.h"
#include "sql_connect.h"
#include "utils.h"
#include "sql_errno.h"

#ifdef HAVE_TERMIOS_H
#include <termios.h>
#endif

/*-----------------------------------------------------------------
 * sql_connect definition
 *---------------------------------------------------------------*/
PSconnInfo      psconninfo;	/* DB linked List Info */


/*-----------------------------------------------------------------
 * Function list
 *-----------------------------------------------------------------
 * int exec_connect()        : connect to D/B
 * int exec_disconnect()     : disconnect connect_name
 * int exec_set_connection() : set connection connect_name
 * void list_AllconnectDB()  : list all connectted DB
 *
 * int parse_connect()       : parse connect statement.
 * int parse_user_passwd()   : parse username and password.
 * int parse_disconnect()    : parse disconenect statement.
 * int parse_set_connection(): parse set_connection statement.
 * int separate_target_name(): separate target_name
 * int compose_target_name() : compose target_name
 * void list_PSconnInfo()    : list PSconnInfo
 */
static int      find_PSconnList(char *connect_name, PSconnList ** pDBL);
static int 
bind_PSconnList(char *target_name, char *connect_name,
		char *user_name, PSconn * dbsep);
static int      free_PSconnList(char *connect_name);
static void     prompt_for_password(char *password);

static char *   getVersion(PSconnSet * pset);
static void     printVersion(PSconnSet * pset, char *version);
static void     printConnect(PSconnSet * pset);
static void	PSenv2encoding(int *encoding, char *encodingchar);



/*
 * #################################################################
 * sql_connect global subroutines
 * ###############################################################
 */

int
exec_connect(PSconnSet * pset, char *target_name,
	     char *connect_name, char *user_name, char *passwd)
/*******************************************************************
 * connect to Database
 *
 *   ex.
 *	target_name = "db[@server[:port]]"
 *      connect_name= "db1"
 *      user_name   = "sakaida"
 *
 *   return( 0) : normal end
 *   return(-1) : target name syntax error
 *   return(-2) : connect DB error
 *   return(-3) : bind    DB error
 *   return(-4) : connect name already used
 ******************************************************************/
{
	PSconnList     *pDBL;
	PSconn	       *dbset;
	PGconn         *pdb;

	char           *dbname, *host, *port;
	char            dbn[TARGET_NAME_MAXLEN];
	char            hst[TARGET_NAME_MAXLEN];
	char            prt[TARGET_NAME_MAXLEN];

	char            tnm[TARGET_NAME_MAXLEN];
	char            unm[USER_NAME_MAXLEN];
	char		*version;
	char		*p, *p2;

	/*---------- separate target_name -------------------*/
	if (separate_target_name(target_name, dbn, hst, prt) < 0)
	{
		setSQLCA(pset->sqlca, SQL_INVALID_STMT,
			 getSQLerrmc("Target name '%s' failed.", target_name), 0, 0);
		if (!pset->quiet)
			printSQLerror(pset->fout, pset->sqlca);
		return (-1);
	}
	/*---------- connect to DEFAULT ---------------------*/
	dbname = host = port = NULL;

	/*---------- connect to target_name -----------------*/
	if (strcmp(target_name, DEFAULT_VALUE) != 0)
	{
		dbname = dbn;
		if (hst[0] != '\0')
			host = hst;
		if (prt[0] != '\0')
			port = prt;
	}
	/*---------- Search connect_name --------------------*/
	if (find_PSconnList(connect_name, &pDBL) == 0)
	{
		setSQLCA(pset->sqlca, SQL_NAME_ALREADY_USE,
			 getSQLerrmc("Connection name '%s' already used.", connect_name), 0, 0);
		if (!pset->quiet)
			printSQLerror(pset->fout, pset->sqlca);
		return (-4);
	}
	/*----------- Connect to Database -------------------*/
	if (user_name != NULL && user_name[0] != '\0' &&
	    strcmp(user_name, DEFAULT_VALUE) != 0)
	{
		char            password[PASSWD_MAXLEN];
		char           *pw = password;

		if (get_Content_type() == -1 && passwd[0] == '\0')
		{
		//	prompt_for_password(password);
			strncpy(password, getpass("Password:"), PASSWD_MAXLEN-1);
			password[PASSWD_MAXLEN-1] = '\0';
		}
		else
		{
			strncpy(password, passwd, PASSWD_MAXLEN-1);	/* CGI */
			password[PASSWD_MAXLEN-1] = '\0';
		}

		if (strcmp(passwd, "NULL") == 0)
			pw = NULL;

		/*********************************************/
		pdb = PQsetdbLogin(host, port, NULL, NULL,
				   dbname, user_name, pw);
		/*********************************************/
	} else
	{
		/*********************************************/
		pdb = PQsetdb(host, port, NULL, NULL, dbname);
		/*********************************************/
	}

	/*------------- Connection Status ----------------------*/
	if (PQstatus(pdb) == CONNECTION_BAD)
	{
		setSQLCA(pset->sqlca, SQL_CONNECTION_BAD,
			 PQerrorMessage(pdb), 0, 0);
		
		if (!pset->quiet)
			printSQLerror(pset->fout, pset->sqlca);

		/*************/
		PQfinish(pdb);
		/*************/
		return (-2);
	}
	/*------------- set Name --------------------------------*/
	strcpy(tnm, PQdb(pdb));

	if (PQhost(pdb) != NULL)
	{
		strcat(tnm, "@");
		strcat(tnm, PQhost(pdb));
	}
	strcat(tnm, ":");
	strcat(tnm, PQport(pdb));

	strcpy(unm, PQuser(pdb));

	/*------------- New PSconn ------------------------------*/
	dbset = wrapMalloc(sizeof(PSconn));
	dbset->db = pdb;
	PSenv2encoding(&dbset->encoding, dbset->encodingchar);
	dbset->exist_des = false;

	pset->dbset = dbset;

	version = getVersion(pset);	 /* Backend Version */
	dbset->ver_no[0] = '\0';	/* Version No.(ex.7.0.1) */
	p = version;
	while(*p != '\0')
	{
		if(*p>='0' && *p<='9')
		{
			strncpy(dbset->ver_no, p, 5);
			dbset->ver_no[5] = '\0';
			p2 = strchr(dbset->ver_no,' ');
			if( p2 != NULL )
				*p2 = '\0';
			wrap_bind_char_variable(POSTGRESQL_VERSION, dbset->ver_no);
			break;
		}
		p++;
	}

	/*------------- bind PSconnList -------------------------*/
	if (bind_PSconnList(tnm, connect_name, unm, dbset) < 0)
	{
		setSQLCA(pset->sqlca, SQL_SYSTEM_ERROR,
			 "Error: bind_PSconnSet().", 0, 0);

		if (!pset->quiet)
			printSQLerror(pset->fout, pset->sqlca);

		/*************/
		PQfinish(pdb);
		/*************/
		free(dbset);
		return (-3);
	}

	/*------------- print backend version -------------------*/
	if (!pset->quiet && !pset->notty)
	{
		printVersion(pset, version);
		printConnect(pset);
	}

	return (0);
}


int
exec_disconnect(PSconnSet * pset, char *connect_name)
/*******************************************************************
 * disconnect connect_name
 *
 *   ex.
 *      connect_name= "db1"
 *
 *   return( 0) : normal end
 *   return(-1) : connect_name not found
 *   return(-2) : CURRENT not exist
 *   return(-3) : DB not open
 ******************************************************************/
{
	PSconnList     *pDBL;

	/*-------------- DB not open -------------------------*/
	if (psconninfo.maxno == 0)
	{
		setSQLCA(pset->sqlca, SQL_DB_NOT_OPEN,
			 "Error: database not open.", 0, 0);
		if (!pset->quiet)
			printSQLerror(pset->fout, pset->sqlca);
		return (-3);
	}
	/*-------------- disconnect CURRENT  ------------------*/
	if (strcmp(connect_name, CURRENT_VALUE) == 0)
	{
		if (psconninfo.currentp != NULL)
		{
			pDBL = psconninfo.currentp;
			free_PSconnList(pDBL->connect_name);
			if (psconninfo.maxno > 0)
			{
				psconninfo.currentp = psconninfo.firstp;
				wrap_bind_char_variable(POSTGRESQL_VERSION, psconninfo.currentp->dbset->ver_no);
			}
			return (0);
		} else
		{
			setSQLCA(pset->sqlca, SQL_DB_NOT_OPEN,
				 "Error: current database not exist.", 0, 0);
			if (!pset->quiet)
				printSQLerror(pset->fout, pset->sqlca);
			return (-2);
		}
	}
	/*-------------- disconnect ALL -----------------------*/
	if (strcmp(connect_name, ALL_VALUE) == 0)
	{
		free_PSconnList(ALL_VALUE);
		wrap_bind_char_variable(POSTGRESQL_VERSION, "");
		return (0);
	}
	/*-------------- disconnect connect_name --------------*/
	if (find_PSconnList(connect_name, &pDBL) == 0)
	{
		free_PSconnList(connect_name);

		if (psconninfo.maxno > 0)
		{
			psconninfo.currentp = psconninfo.firstp;
			wrap_bind_char_variable(POSTGRESQL_VERSION, psconninfo.currentp->dbset->ver_no);
		}
		return (0);
	} else
	{
		setSQLCA(pset->sqlca, SQL_NAME_NOT_FOUND,
			 getSQLerrmc("Connection name '%s' not found.", connect_name), 0, 0);
		if (!pset->quiet)
			printSQLerror(pset->fout, pset->sqlca);
		return (-1);
	}

	return (0);
}

int
exec_set_connection(PSconnSet * pset, char *connect_name)
/*******************************************************************
 * set connection  connect_name
 *
 *   ex.
 *      connect_name= "db1"
 *
 *   return( 0) : normal end
 *   return(-1) : connect_name not found
 *   return(-3) : DB not open
 ******************************************************************/
{
	PSconnList     *pDBL;

	/*-------------- DB not open -------------------------*/
	if (psconninfo.maxno == 0)
	{
		setSQLCA(pset->sqlca, SQL_DB_NOT_OPEN,
			 "Error: database not open.", 0, 0);
		if (!pset->quiet)
			printSQLerror(pset->fout, pset->sqlca);
		return (-3);
	}
	/*------------ set connection  connect_name -----------*/
	if (find_PSconnList(connect_name, &pDBL) == 0)
	{
		pset->dbset = pDBL->dbset;
		psconninfo.currentp = pDBL;
		wrap_bind_char_variable(POSTGRESQL_VERSION, pset->dbset->ver_no);
		return (0);
	} else
	{
		setSQLCA(pset->sqlca, SQL_NAME_NOT_FOUND,
			 getSQLerrmc("Connection name '%s' not found.", connect_name), 0, 0);
		if (!pset->quiet)
			printSQLerror(pset->fout, pset->sqlca);
		return (-1);
	}

	return (0);
}

void
list_AllconnectDB(FILE * fout0, PSprintOpt * Opt, int encoding)
/*************************************************************
 *  list all connected DB
 ************************************************************/
{
	FILE            *fout = fout0;
	PSconnList      *pDBL;
	char            *current, *pcur;
	int             count = 0;

	if (get_Content_type() >= 0)
		fout = stdout;

	if (psconninfo.maxno == 0)
	{
		fprintf(fout, "# Database connection not exist.%s\n", get_CGI_BR());
		/***
		fprintf(fout, "%s\n", get_CGI_BR());
		**/
		return;
	}
	/*------------- set printOpt ---------------------*/
	if (Opt->caption == NULL)
		Opt->caption = "# List of database connection (C: current database is '*')";

	Opt->nFields = 5;
	Opt->fieldName = (char **) wrapCalloc(Opt->nFields, sizeof(char *));
	Opt->fieldName[0] = "C";
	Opt->fieldName[1] = "connect_name";
	Opt->fieldName[2] = "user_name";
	Opt->fieldName[3] = "target_name(db@server:port)";
	Opt->fieldName[4] = "client_encoding";

	Opt->nTups = 0;
	pDBL = psconninfo.firstp;
	while (pDBL != NULL)
	{
		Opt->nTups++;
		pDBL = pDBL->next;
	}

	Opt->fieldValue = (char **) wrapCalloc(Opt->nFields * (Opt->nTups + 1), sizeof(char *));
	pcur = current  = (char *) wrapMalloc(2 * (Opt->nTups + 1));

	pDBL = psconninfo.firstp;
	while (pDBL != NULL)
	{
		if (pDBL == psconninfo.currentp)
			*pcur = '*';
		else
			*pcur = ' ';
		*(pcur + 1) = '\0';

		Opt->fieldValue[count] = pcur;
		Opt->fieldValue[count + 1] = pDBL->connect_name;
		Opt->fieldValue[count + 2] = pDBL->user_name;
		Opt->fieldValue[count + 3] = pDBL->target_name;
		Opt->fieldValue[count + 4] = pDBL->dbset->encodingchar;

		count += Opt->nFields;
		pDBL = pDBL->next;
		pcur += 2;
	}

	/*------------- print ----------------------------*/
	set_printOpt(Opt);
	PSprint(fout, Opt, encoding);
	free_printOpt(Opt);

	/*------------- free memory ----------------------*/
	free(current);
}


void
list_PSconnInfo(FILE * fout)
/************************************************************
 *   list PSconnInfo (debug write)
 ***********************************************************/
{
	fprintf(fout, "# PSconnInfo List\n");
	fprintf(fout, "\n");
	fprintf(fout, "maxno   =%d\n", psconninfo.maxno);

	if (psconninfo.firstp != NULL)
		fprintf(fout, "firstp  =%lu (next=%lu)\n", (unsigned long) psconninfo.firstp, (unsigned long) psconninfo.firstp->next);
	if (psconninfo.lastp != NULL)
		fprintf(fout, "lastp   =%lu (next=%lu)\n", (unsigned long) psconninfo.lastp, (unsigned long) psconninfo.lastp->next);
	if (psconninfo.currentp != NULL)
		fprintf(fout, "currentp=%lu (next=%lu)\n", (unsigned long) psconninfo.currentp, (unsigned long) psconninfo.currentp->next);
	if (psconninfo.defaultp != NULL)
		fprintf(fout, "defaultp=%lu (next=%lu)\n", (unsigned long) psconninfo.defaultp, (unsigned long) psconninfo.defaultp->next);
	fprintf(fout, "\n");
}


int
parse_connect(PSconnSet *pset, char *connect_stmt, char *target_name,
	      char *connect_name, char *user_name, char *passwd)
/*******************************************************************
 * parse connect statement.
 *
 *  ex.
 *   input :
 *     connect_stmt="connect to admin@www:5432 as db1 user sakaida"
 *     connect_stmt="connect to DEFAULT"
 *  output:
 *     target_name ="admin@www:5432" | "_DEFAULT_"
 *     connect_name="db1"
 *     user_name   ="sakaida"
 *
 *   return( 0) : normal end
 *   return(-1) : not "connect" statement.
 *   return(-2) : target name error
 *   return(-3) : connect name error
 *   return(-4) : user name error
 *   return(-5) : end of statement error
 ******************************************************************/
{
	char           *p, *p2;
	char           *pas = NULL, *puser = NULL;

	/*------------ check SQL statement --------------*/
	if (connect_stmt == NULL)
		return (-1);

	/*------------ initial set ----------------------*/
	target_name[0] = '\0';
	connect_name[0] = '\0';
	user_name[0] = '\0';
	passwd[0] = '\0';

	/*------------ search  "connect" ----------------*/
	p = connect_stmt;
	p = skipSpace(p);
	p2 = skipToken(p);

	if (p2 - p != 7 || strUcmp(p, "CONNECT", 7) != 0)
		return (-1);

	/*------------ search  "to" ---------------------*/
	p = p2;
	p = skipSpace(p);
	p2 = skipToken(p);

	if (p2 - p != 2 || strUcmp(p, "TO", 2) != 0)
		return (-1);

	/*--------- get target_name(db@server:port) -----*/
	p = p2;
	p = skipSpace(p);
	p2 = skipToken(p);

	if (p2 - p == 0 || p2 - p > TARGET_NAME_MAXLEN - 1)
	{
		*p2 = '\0';
		setSQLCA(pset->sqlca, SQL_INVALID_STMT,
			 getSQLerrmc("Target name '%s' failed.", p), 0, 0);
		return (-2);
	}

	strncpy(target_name, p, (p2 - p));
	target_name[p2 - p] = '\0';

	if (!((target_name[0] >= 'A' && target_name[0] <= 'Z') ||
	      (target_name[0] >= 'a' && target_name[0] <= 'z')))
	{
		setSQLCA(pset->sqlca, SQL_INVALID_STMT,
			 getSQLerrmc("Target name '%s' failed.", target_name), 0, 0);
		return (-2);
	}

	/*------------- search "as" or "user" -----------*/
	p = p2;
	p = skipSpace(p);
	p2 = skipToken(p);

	/*------------- set connect_name ----------------*/
	if (p2 - p == 2 && strUcmp(p, "AS", 2) == 0)
	{
		pas = p;

		p = p2;
		p = skipSpace(p);
		p2 = skipToken(p);

		if (p2 - p == 0 || p2 - p > CONNECT_NAME_MAXLEN - 1)
		{
			*p2 = '\0';
			setSQLCA(pset->sqlca, SQL_INVALID_STMT,
			 getSQLerrmc("Connection name '%s' failed.", p), 0, 0);
			return (-3);
		}

		strncpy(connect_name, p, (p2 - p));
		connect_name[p2 - p] = '\0';

		if (checkName(connect_name) < 0)
		{
			setSQLCA(pset->sqlca, SQL_INVALID_STMT,
			 getSQLerrmc("Connection name '%s' failed.", connect_name), 0, 0);
			return (-3);
		}

		p = p2;
		p = skipSpace(p);
		p2 = skipToken(p);
	} else
		pas = NULL;

	/*----------------- set user_name ---------------*/
	if (p2 - p == 4 && strUcmp(p, "USER", 4) == 0)
	{
		if (parse_user_passwd(p, user_name, passwd) < 0 )
		{
			setSQLCA(pset->sqlca, SQL_INVALID_STMT,
			 getSQLerrmc("User name '%s' failed.", p), 0, 0);
			return (-4);
		}
	
		if (checkUserName(user_name) < 0)
		{
			setSQLCA(pset->sqlca, SQL_INVALID_STMT,
			 getSQLerrmc("User name '%s' failed.", user_name), 0, 0);
			return (-4);
		}

		puser = user_name;
	}

	/*--------------- if target is DEFAULT ----------------*/
	if (strlen(target_name) == 7 && strUcmp(target_name, "DEFAULT", 7) == 0 &&
	    pas == NULL && puser == NULL)
	{
		strcpy(target_name, DEFAULT_VALUE);
		strcpy(connect_name, DEFAULT_VALUE);
		strcpy(user_name, DEFAULT_VALUE);
	} else
	{
		/*- if connect_name is null then set default name -*/
		if (pas == NULL)
		{
			char            dbn[256], hst[256], prt[256];

			if (separate_target_name(target_name, dbn, hst, prt) == 0)
				strcpy(connect_name, dbn);
			else
				strcpy(connect_name, DEFAULT_VALUE);
		}
		/*-- if user_name is null then set default name ---*/
		if (puser == NULL)
			strcpy(user_name, DEFAULT_VALUE);
	}

	return (0);
}

int
parse_user_passwd(char *str, char *username, char *password)
/******************************************************************
 *   parse username and password.
 * 
 *  str="USER username"                        .... type1
 *  str="USER username/password"               .... type1-1
 *  str="USER username password"               .... type2
 *  str="USER username USING         password" .... type3
 *  str="USER username IDENTIFIED BY password" .... type4
 *
 *  return = 0: OK
 *  return =-1: error
 *  return =-2: username length error
 *  return =-3: password length error
 ******************************************************************
 */
{
	char	buf[USER_NAME_MAXLEN+PASSWD_MAXLEN+2];
	char	*pa[5];
	int	na[5];

	char	*p, *p2;
	int	n = 0;

	username[0] = '\0';
	password[0] = '\0';

	/*--------------- skip "USER" --------------*/
	p = str;
	p = skipSpace(p);
	p2 = skipToken(p);

	if (!(p2 - p == 4 && strUcmp(p, "USER", 4) == 0))
		return(-1);
	
	/*--------------- No. of word. -------------*/
	while(1)
	{
		p = p2;
		p = skipSpace(p);
		p2 = skipToken(p);

		if ( *p == '\0' || p2-p == 0)
			break;

		pa[n] = p;
		na[n] = p2-p;
		n++;

		if (n > 4)
			return(-1);
	}


	switch(n)
	{
	case 0:
		return(-1);
	case 1:
		/*----------------- username only ------------------*/
		if (na[0] <= USER_NAME_MAXLEN+PASSWD_MAXLEN+1 )
		{
			strncpy(buf, pa[0], na[0]);
			buf[na[0]] = '\0';
		}
		else
			return(-1);

		if ((p=strchr(buf, '/')) == NULL)
		{
			if (na[0] <= USER_NAME_MAXLEN )
			{
				strcpy(username, buf);
				return(0);
			}
			else
				return(-2);
		}
		/*--------------- username/password --------------*/
		else 
		{
			*p = '\0';

			if (strlen(buf) <= USER_NAME_MAXLEN )
				strcpy(username, buf);
			else
				return(-2);

			if (strlen(p+1) <= PASSWD_MAXLEN )
				strcpy(password, p+1);
			else
				return(-3);

			return(0);
		}
	case 2:
		/*---------------- username password --------------*/
		if (na[0] <= USER_NAME_MAXLEN )
		{
			strncpy(username, pa[0], na[0]);
			username[na[0]] = '\0';
		}
		else
			return(-2);

		if (na[1] <= PASSWD_MAXLEN )
		{
			strncpy(password, pa[1], na[1]);
			password[na[1]] = '\0';
			return(0);
		}
		else
			return(-3);
	case 3:
		/*------------- username USING password -----------*/
		if (na[1] == 5 && strUcmp(pa[1], "USING", 5)==0)
		{
			if (na[0] <= USER_NAME_MAXLEN )
			{
				strncpy(username, pa[0], na[0]);
				username[na[0]] = '\0';
			}
			else
				return(-2);

			if (na[2] <= PASSWD_MAXLEN )
			{
				strncpy(password, pa[2], na[2]);
				password[na[2]] = '\0';
			}
			else
				return(-3);

			return(0);

		}
		else
			return(-1);
	case 4:
		/*-------- username IDENTIFIED BY password -----------*/
		if ((na[1] ==10 && strUcmp(pa[1], "IDENTIFIED", 10)==0) &&
		    (na[2] == 2 && strUcmp(pa[2], "BY", 2)==0))  
		{
			if (na[0] <= USER_NAME_MAXLEN )
			{
				strncpy(username, pa[0], na[0]);
				username[na[0]] = '\0';
			}
			else
				return(-2);

			if (na[3] <= PASSWD_MAXLEN )
			{
				strncpy(password, pa[3], na[3]);
				password[na[3]] = '\0';
			}
			else
				return(-3);

			return(0);
		}
	}
	return(-1);
}


int
parse_disconnect(PSconnSet *pset, char *disconnect_stmt, char *connect_name)
/*******************************************************************
 * parse disconnect statement.
 *
 *  ex.
 *   input :
 *     disconnect_stmt="disconnect db1|CURRENT|ALL"
 *  output:
 *     connect_name="db1" | "_CURRENT_" | "_ALL_"
 *
 *   return( 0) : normal end
 *   return(-1) : not "disconnect" statement.
 *   return(-3) : connect name error
 ******************************************************************/
{
	char           *p, *p2;

	/*------------ check SQL statement --------------*/
	if (disconnect_stmt == NULL)
		return (-1);

	/*------------ initial set ----------------------*/
	connect_name[0] = '\0';

	/*------------- search "disconnect" -------------*/
	p = disconnect_stmt;
	p = skipSpace(p);
	p2 = skipToken(p);

	if (p2 - p != 10 || strUcmp(p, "DISCONNECT", 10) != 0)
		return (-1);

	/*---------------- set connect_name --------------*/
	p = p2;
	p = skipSpace(p);
	p2 = skipToken(p);

	if (p2 - p == 0 || p2 - p > CONNECT_NAME_MAXLEN - 1)
	{
		*p2 = '\0';
		setSQLCA(pset->sqlca, SQL_INVALID_STMT,
			 getSQLerrmc("Connection name '%s' failed.", p), 0, 0);
		return (-3);
	}

	strncpy(connect_name, p, (p2 - p));
	connect_name[p2 - p] = '\0';

	if (checkName(connect_name) < 0)
	{
		setSQLCA(pset->sqlca, SQL_INVALID_STMT,
			 getSQLerrmc("Connection name '%s' failed.", connect_name), 0, 0);
		return (-3);
	}

	/*---------- if connect_name is DEFAULT ----------*/
	if (strlen(connect_name) == 7 && strUcmp(connect_name, "DEFAULT", 7) == 0)
		strcpy(connect_name, DEFAULT_VALUE);
	else
		/*---------- if connect_name is CURRENT ----------*/
	if (strlen(connect_name) == 7 && strUcmp(connect_name, "CURRENT", 7) == 0)
		strcpy(connect_name, CURRENT_VALUE);
	else
		/*---------- if connect_name is ALL --------------*/
	if (strlen(connect_name) == 3 && strUcmp(connect_name, "ALL", 3) == 0)
		strcpy(connect_name, ALL_VALUE);

	return (0);
}


int
parse_set_connection(PSconnSet *pset, char *set_connect_stmt, char *connect_name)
/*******************************************************************
 * parse set connection statement.
 *
 *  ex.
 *   input :
 *     set_connect_stmt="set connection  db1|DEFAULT"
 *  output:
 *     connect_name="db1" | "_DEFAULT_"
 *
 *   return( 0) : normal end
 *   return(-1) : not "set connection" statement.
 *   return(-3) : connect name error
 ******************************************************************/
{
	char           *p, *p2;

	/*------------ check SQL statement --------------*/
	if (set_connect_stmt == NULL)
		return (-1);

	/*------------ initial set ----------------------*/
	connect_name[0] = '\0';

	/*------------- search "set" --------------------*/
	p = set_connect_stmt;
	p = skipSpace(p);
	p2 = skipToken(p);

	if (p2 - p != 3 || strUcmp(p, "SET", 3) != 0)
		return (-1);

	/*------------- search "connection" -------------*/
	p = p2;
	p = skipSpace(p);
	p2 = skipToken(p);

	if (p2 - p != 10 || strUcmp(p, "CONNECTION", 10) != 0)
		return (-1);

	/*------------- set connect_name -----------------*/
	p = p2;
	p = skipSpace(p);
	p2 = skipToken(p);

	if (p2 - p == 0 || p2 - p > CONNECT_NAME_MAXLEN - 1)
	{
		*p2 = '\0';
		setSQLCA(pset->sqlca, SQL_INVALID_STMT,
			 getSQLerrmc("Connection name '%s' failed.", p), 0, 0);
		return (-3);
	}

	strncpy(connect_name, p, (p2 - p));
	connect_name[p2 - p] = '\0';

	if (checkName(connect_name) < 0)
	{
		setSQLCA(pset->sqlca, SQL_INVALID_STMT,
			 getSQLerrmc("Connection name '%s' failed.", connect_name), 0, 0);
		return (-3);
	}

	/*--------- if connect_name is DEFAULT -----------*/
	if (strlen(connect_name) == 7 && strUcmp(connect_name, "DEFAULT", 7) == 0)
		strcpy(connect_name, DEFAULT_VALUE);

	return (0);
}


int
separate_target_name(char *target_name,
		     char *dbname, char *host, char *port)
/*******************************************************************
 *  separate the target_name to D/B name, host name and port number.
 *
 *   ex.
 *   input :
 *	target_name="admin@www:5432"
 *   output:
 *	dbnam= "admin"
 *	host ="www.psn.ne.jp"
 *	port ="5432"
 *
 *   return( 0) : normal end
 *   return(-1) : syntax error
 *   return(-2) : dbname error
 *   return(-3) : host error
 *   return(-4) : port error
 ******************************************************************/
{
	int             len;
	char           *p, *p2;

	/*--------- check target_name ------------*/
	if (target_name == NULL)
		return (-1);
	if ((len = strlen(target_name)) > TARGET_NAME_MAXLEN - 1)
		return (-1);
	if (strchr(target_name, ' ') != NULL)
		return (-1);

	/*----------- initial set ----------------*/
	dbname[0] = '\0';
	host[0] = '\0';
	port[0] = '\0';

	if (strcmp(target_name, DEFAULT_VALUE) == 0)
		return (0);

	/*------------- set dbname ---------------*/
	p = target_name;

	if ((p2 = strchr(p, '@')) != NULL ||
	    (p2 = strchr(p, ':')) != NULL ||
	    (p2 = strchr(p, '\0')) != NULL)
	{
		strncpy(dbname, p, p2 - p);
		dbname[p2 - p] = '\0';
		if (!((dbname[0] >= 'A' && dbname[0] <= 'Z') ||
		      (dbname[0] >= 'a' && dbname[0] <= 'z')))
			return (-2);
	}
	/*------------- set host -----------------*/
	if ((p = strchr(target_name, '@')) != NULL)
	{
		p++;
		if ((p2 = strchr(p, ':')) != NULL ||
		    (p2 = strchr(p, '\0')) != NULL)
		{
			strncpy(host, p, p2 - p);
			host[p2 - p] = '\0';
			if (!((host[0] >= 'A' && host[0] <= 'Z') ||
			      (host[0] >= 'a' && host[0] <= 'z') ||
			      (host[0] >= '0' && host[0] <= '9')))
				return (-3);
		}
	}
	/*------------- set port -----------------*/
	if ((p = strchr(target_name, ':')) != NULL)
	{
		p++;
		if ((p2 = strchr(p, '\0')) != NULL)
		{
			strncpy(port, p, p2 - p);
			port[p2 - p] = '\0';
			if (!(port[0] >= '0' && port[0] <= '9'))
				return (-4);
		}
	}
	return (0);
}


char           *
compose_target_name(char *dbname, char *host, char *port)
/*******************************************************************
 * compose the target_name from D/B name , host name and port number.
 *
 *   ex.
 *   input :
 *	dbname="admin"
 *	host  ="www"
 *	port  ="5432"
 *
 *   return("admin@www:5432") : normal end
 *   return("_DEFAULT_")      : DEFAULT name
 *   return(NULL)	            : error
 ******************************************************************/
{
	static char     target_name[TARGET_NAME_MAXLEN];
	int             i = 0;

	/*------------ DEFAULT name -------------*/
	if (dbname == NULL && host == NULL && port == NULL)
		return (DEFAULT_VALUE);

	if (dbname[0] == '\0' && host[0] == '\0' && port[0] == '\0')
		return (DEFAULT_VALUE);

	/*------------ dbname check -------------*/
	if (dbname == NULL)
		return (NULL);

	i = strlen(dbname);
	if (i < TARGET_NAME_MAXLEN)
		strcpy(target_name, dbname);

	/*------------ set host name ------------*/
	if (host != NULL && strlen(host) > 0)
	{
		i += strlen(host) + 1;
		if (i < TARGET_NAME_MAXLEN)
		{
			strcat(target_name, "@");
			strcat(target_name, host);
		} else
			return (NULL);
	}
	/*------------ set port number -----------*/
	if (port != NULL && strlen(port) > 0)
	{
		i += strlen(port) + 1;
		if (i < TARGET_NAME_MAXLEN)
		{
			strcat(target_name, ":");
			strcat(target_name, port);
		} else
			return (NULL);

	}
	/*------- return static char target_name -------*/
	return (target_name);
}



/*
 * ############################################################ 
 * Local subroutines 
 * ############################################################
 */

static int
find_PSconnList(char *connect_name, PSconnList ** pDBL)
/**************************************************************
 *  find connectted DB information
 *
 *  ex.
 *  INPUT : connect_name = "db1"
 *  OUTPUT: pDBL          = found PSconnList pointer
 *
 *   return( 0) : find connect_name
 *   return(-1) : DB not open
 *************************************************************/
{
	PSconnList     *pdbl;

	/*----- DB is not connectted now at once ---------*/
	if (psconninfo.maxno == 0)
		return (-1);

	/*------------ search the connect_name -----------*/
	pdbl = psconninfo.firstp;

	while (pdbl != NULL)
	{
		/*---- find the connect_name -----*/
		if (pdbl->openflag == 1 &&
		    strcmp(pdbl->connect_name, connect_name) == 0)
		{
			*pDBL = pdbl;
			return (0);
		}
		pdbl = pdbl->next;
	}

	/*-------- can not find the connect_name ---------*/
	return (-1);
}


static int
bind_PSconnList(char *target_name, char *connect_name,
		char *user_name, PSconn * dbset)
/**************************************************************
 *  bind connectted DB information
 *
 *  ex.
 *  INPUT : target_name = "dbname[@server[:port]]" | "_DEFAULT_"
 *	    connect_name= "db1"
 *	    user_name   = "sakaida"
 *          pDB          = DB pointer
 *
 *   return( 0) : fnormal end
 *   return(-1) : already exist connect_name
 *   return(-2) : malloc error
 *************************************************************/
{
	PSconnList     *pDBL, *newDBL;

	/*-------------- find connect_name -----------------*/
	if (find_PSconnList(connect_name, &pDBL) == 0)
		return (-1);

	/*---- search the disconnect flag and replace ------*/
	pDBL = psconninfo.firstp;

	while (pDBL != NULL)
	{
		/*---- find the disconnect flag ----*/
		if (pDBL->openflag == 0)
		{
			pDBL->openflag = 1;
			pDBL->target_name = wrapStrdup(target_name);
			pDBL->connect_name = wrapStrdup(connect_name);
			pDBL->user_name = wrapStrdup(user_name);
			pDBL->dbset = dbset;
			return (0);
		}
		pDBL = pDBL->next;
	}

	/*----------- malloc and set PSconnList ---------*/
	newDBL = wrapMalloc(sizeof(PSconnList));

	newDBL->openflag = 1;
	newDBL->target_name = wrapStrdup(target_name);
	newDBL->connect_name = wrapStrdup(connect_name);
	newDBL->user_name = wrapStrdup(user_name);
	newDBL->dbset = dbset;
	newDBL->next = NULL;

	/*----------------- first setup -----------------*/
	if (psconninfo.firstp == NULL)
	{
		psconninfo.maxno = 1;
		psconninfo.firstp = newDBL;
		psconninfo.lastp = newDBL;
		psconninfo.currentp = newDBL;
		psconninfo.defaultp = newDBL;
	} else
		/*-------------- add last pointer ---------------*/
	{
		psconninfo.lastp->next = newDBL;

		psconninfo.maxno++;
		psconninfo.lastp = newDBL;
		psconninfo.currentp = newDBL;
		if (strcmp(target_name, DEFAULT_VALUE) == 0)
			psconninfo.defaultp = newDBL;
	}

	return (0);
}

static int
free_PSconnList(char *connect_name)
/**************************************************************
 *  free connectted DB information
 *
 *  ex.
 *  INPUT : connect_name = "db1"
 *
 *   return( 0) : normal end
 *   return(-1) : DB not open
 *************************************************************/
{
	PSconnList     *pDBL, *prev, *next;

	/*----- DB is not connectted now at once ---------*/
	if (psconninfo.maxno == 0)
		return (-1);

	/*------------ search the connect_name -----------*/
	pDBL = prev = psconninfo.firstp;

	while (pDBL != NULL)
	{
		/*------------- NEXT pointer -------------*/
		next = pDBL->next;

		/*--- Free DB List and restructure link --*/
		if (pDBL->openflag == 1 &&
		    (strcmp(pDBL->connect_name, connect_name) == 0 ||
		     strcmp(connect_name, ALL_VALUE) == 0))
		{

			/*---- free PSconnList element ---*/
			free(pDBL->target_name);
			free(pDBL->connect_name);
			free(pDBL->user_name);
			/************************/
			PQfinish(pDBL->dbset->db);
			/************************/
			free(pDBL->dbset);

			/*----- clear PSconnInfo ----------*/
			if (psconninfo.maxno == 1)
			{
				psconninfo.firstp = NULL;
				psconninfo.lastp = NULL;
				psconninfo.currentp = NULL;
				psconninfo.defaultp = NULL;
			} else
			{
				if (pDBL == psconninfo.firstp)
					psconninfo.firstp = next;
				if (pDBL == psconninfo.lastp)
					psconninfo.lastp = prev;
				if (pDBL == psconninfo.defaultp)
					psconninfo.defaultp = NULL;
				if (pDBL == psconninfo.currentp)
					psconninfo.currentp = NULL;

				if (pDBL != prev)
					prev->next = next;
			}

			pDBL->next = NULL;
			free(pDBL);

			psconninfo.maxno--;
			if (psconninfo.maxno < 0)
				psconninfo.maxno = 0;

			if (strcmp(connect_name, ALL_VALUE) != 0)
				return (0);
		}
		prev = pDBL;
		pDBL = next;
	}

	if (strcmp(connect_name, ALL_VALUE) == 0)
		return (0);
	else
		return (-1);
}


static void
prompt_for_password(char *password)
/*******************************************************************
 * 	input password
 *******************************************************************/
{
	char            buf[512];
	int             length;

#ifdef HAVE_TERMIOS_H
	struct termios  t_orig, t;
#endif

	/*--------------- input password ----------------*/
	printf("Password: ");

#ifdef HAVE_TERMIOS_H
	tcgetattr(0, &t);
	t_orig = t;
	t.c_lflag &= ~ECHO;
	tcsetattr(0, TCSADRAIN, &t);
#endif
	fgets(password, 98, stdin);

#ifdef HAVE_TERMIOS_H
	tcsetattr(0, TCSADRAIN, &t_orig);
#endif

	length = strlen(password);

	/*------------- skip rest of the line ------------*/
	if (length > 0 && password[length - 1] != '\n')
	{
		do
		{
			fgets(buf, 510, stdin);
		} while (buf[strlen(buf) - 1] != '\n');
	}
	if (length > 0 && password[length - 1] == '\n')
		password[length - 1] = '\0';

	printf("\n\n");
}


static char *
getVersion(PSconnSet * pset)
/*******************************************************************
 *   Get BACKEND version.
 *******************************************************************/
{
#define PGVERSIONBUFSZ 129
	static	char	version[PGVERSIONBUFSZ];
	PGresult        *res;
	FILE            *fout = pset->fout;
	char            *query = "select version();";

	if (!(res = PQexec(pset->dbset->db, query)))
		return(NULL);

	if (PQresultStatus(res) == PGRES_COMMAND_OK ||
	    PQresultStatus(res) == PGRES_TUPLES_OK)
	{
		strncpy(version, PQgetvalue(res, 0, 0), PGVERSIONBUFSZ - 1);
		version[PGVERSIONBUFSZ - 1] = '\0';
		PQclear(res);
	} else
		PQclear(res);

	return(version);
}

static void
printVersion(PSconnSet * pset, char *version)
/*******************************************************************
 *   Print BACKEND version.
 *******************************************************************/
{
	FILE            *fout = pset->fout;

	fprintf(fout, "# %s%s\n", version, get_CGI_BR());
	return;
}

static void
printConnect(PSconnSet * pset)
/*******************************************************************
 *   Print "CONNECT TO CURRENT_DB" message.
 *******************************************************************/
{
	FILE           *fout = pset->fout;
	PSconnList     *pDBL = psconninfo.currentp;

	if (!pset->quiet && !pset->notty)
	{
		fprintf(fout, "# CONNECT TO  %s  ", pDBL->target_name);
		fprintf(fout, "AS  %s  ", pDBL->connect_name);
		fprintf(fout, "USER  %s %s\n", pDBL->user_name, get_CGI_BR());
		/**
		fprintf(fout, "%s\n", get_CGI_BR());
		**/
	}
}

/*=================================================================
 *  client encoding environment.
 *=================================================================
 */

#ifdef MULTIBYTE
static void
PSenv2encoding(int *encoding, char *encodingchar)
{
	char       *str;

	encoding = 0;
	str = getenv("PGCLIENTENCODING");
	
	if (str && *str != '\0')
	{
		encoding= pg_char_to_encoding(str);
		strncpy(encodingchar, str, PS_ENCODING_CHAR_LEN);
		encodingchar[PS_ENCODING_CHAR_LEN] = '\0';
	}
	else
	{
		strcpy(encodingchar, " ");
	}
}
#else
static void
PSenv2encoding(int *encoding, char *encodingchar)
{
	encoding = 0;
	strcpy(encodingchar, " ");
}
#endif
