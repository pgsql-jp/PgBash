/*----------------------------------------------------------------
 *
 * exec_sql_main.c
 *    A direct/embedded SQL bash built-in command for postgreSQL.
 *    This routine is called by "exec_sql.def".
 *
 * Change logs
 * 2000.03.22: Add 'encoding' at list_AllconnectDB().
 * 2000.03.23: Move printVersion(),printConnect() to "sql_connect.c".       
 *             Change pset-db to pset->dbset->db.
 * 2000.04.21: Change COPY statement to EXEC_SQL -y "COPY .."
 * 2000.05.10: Add WITH NULL AS 'X' to COPY statement.
 * 2001.04.10: Modify the style of exec_sql_main().
 * 2001.04.15: Delete exec_sql_example[].
 * 2001.04.15: Add set_exec_sql_options() call.
 * 2001.04.15: Fix execCopy().
 * 2001.04.15: Delete set_connection/disconnect message.
 * 2001.04.15: Add lo_import/lo_export/lo_unlink.
 * 2001.06.11: Add SQL_WORD(SQL reserved word) shell variable.
 * 2001.06.11: Add 'normal_option_end:' label.
 * 2001.06.11: Add EXEC_SQL_OPTION struct.
 * 2001.11.24: Add sigsetjmp(),siglongjmp() to cancell query.
 * 2001.11.26: Add SQL_HELP version.
 * 2011.07.15: Delete SQL_HELP.
 * 2011.07.20: Delete pgbash_copy functionality.
 *
 *
 *-----------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>

#include "libpq-fe.h"

#include "exec_sql.h"
#include "sql_connect.h"
#include "sql_set.h"
#include "psql_sub.h"
#include "utils.h"
#include "sql_errno.h"
#include "version.h"

#ifdef __CYGWIN32__
#include <getopt.h>
#endif


#define SQL_WORD "SQL_WORD"  /* the shell variable name of SQL reserved word.*/
#define SQL_WORD_LEN    16

#define DEFAULT_FIELD_SEP	"|"
#define TAB_FIELD_SEP_STRING	"\\t"
#define TAB_FIELD_SEP		"\t"

#define DEFAULT_NULL_STRING	" "
#define DEFAULT_ZERO_STRING	" "


/*------------------------------------------------------------------
 *  Functions list
 *------------------------------------------------------------------
*extern int exec_sql_main(Option * option, char *singleQuery);
*/
static void	initPSconnSet(char *singleQuery,Option *option,
	 EXEC_SQL_OPTION *saved_option, PSconnSet *pset);
static void usage(FILE * fout, char *optarg);
static void printStatusSHELLvar(FILE * fout);
static void safe_write_stderr(const char *s);
static void handle_sigint(int sig);
static int	bind_SQLCA(SQLCA * sqlca);
static int	print_SQLCA(FILE *fout);
static void	get_sql_word(char *singleQuery, char *sql_word);

/*------------------------------------------------------------------
 *  Cancel connection pointer. (connection to try cancel on)
 *----------------------------------------------------------------*/
static PGconn  *cancelConn = NULL; 

/*------------------------------------------------------------------
 * SQLCA variables visible to the programs.
 *----------------------------------------------------------------*/
static struct sqlca sqlca_init =
{
	{'S', 'Q', 'L', 'C', 'A', ' ', ' ', ' '},
	sizeof(struct sqlca),
	0,
	{0, {0}},
	{'N', 'O', 'T', ' ', 'S', 'E', 'T', ' '},
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
};

static struct sqlca sqlca0 =
{
	{'S', 'Q', 'L', 'C', 'A', ' ', ' ', ' '},
	sizeof(struct sqlca),
	0,
	{0, {0}},
	{'N', 'O', 'T', ' ', 'S', 'E', 'T', ' '},
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
};

/*
 * ################################################################# 
 * The exec_sql main routine for PostgreSQL.
 * #################################################################
 */

static sigjmp_buf  exec_sql_main_jump;

int
exec_sql_main(Option * option, char *singleQuery)
/****************************************************************
 * The exec_sql main routine for PostgreSQL.
 *
 * return code
 *	r=  0 : normal end
 *	r= -1 : error
 ****************************************************************/
{
	EXEC_SQL_OPTION	*saved_option;
	PSconnSet	pset;	

	char	target_name[TARGET_NAME_MAXLEN];
	char	connect_name[CONNECT_NAME_MAXLEN];
	char	user_name[USER_NAME_MAXLEN];
	char	passwd[PASSWD_MAXLEN];
	char	sql_word[SQL_WORD_LEN];

	int		ret;
	bool	success = true;

	/*-------------------------------------------------------
	 *		initial set.
	 *-----------------------------------------------------*/
	memset(&pset, 0, sizeof(PSconnSet));

	saved_option = get_exec_sql_options();
	initPSconnSet(singleQuery, option, saved_option, &pset );
	sql_word[0] = '\0';

	/*-------------------------------------------------------
	 *        Interactive defaults(cntl-C=>cancel) -
	 *------------------------------------------------------*/
	if (!isatty(0) || !isatty(1))
		pset.notty = 1;	/* Noninteractive */
	else
		signal(SIGINT, handle_sigint);

    /******************************************************************
     *        Return from siglongjmp() of handle_sigint().
     ******************************************************************/
    if (sigsetjmp(exec_sql_main_jump,1) == 0 ) 
    {
	/* ===================================================== 
	 * HELP (or Usage)
	 * ===================================================*/
	if (option->hflg.flg)
	{
		usage(pset.fout, (char *) option->hflg.opt);
		goto normal_option_end;
	}

	/* ===================================================== 
	 * display VERSION
	 * ====================================================*/
	if (option->vflg)
	{
		fprintf(pset.fout, "# %s %s\n", pgbash_version_string(), get_CGI_BR());
		goto normal_option_end;
	}

	/* ===================================================== 
	 * print SQL status
	 * ====================================================*/
	if (option->sflg && singleQuery == NULL)
	{
		printStatusSHELLvar(pset.fout);
		goto normal_option_end;
	}

	/* ===================================================== 
	 * list all connected databases.
	 * ====================================================*/
	if (option->mflg)
	{
		int	encoding;
		if(pset.dbset == NULL)
			encoding = 0;
		else
			encoding = pset.dbset->encoding;

		list_AllconnectDB(pset.fout, &(pset.opt), encoding);

		goto normal_option_end;
	}

	/* ===================================================== 
	 * Set EXEC_SQL options 
	 * ====================================================*/
	ret = set_exec_sql_options(&pset, singleQuery); 

	if (ret == 0)
	{
		strcpy(sql_word, "SET");
		goto normal_option_end;
	}
	else if(ret < -1)      
		goto syntax_error;

	/*------------------------------------------------------
	 * clear SQLCA for executing SQL.
	 *-----------------------------------------------------*/
	memcpy((char *) &sqlca0, (char *) &sqlca_init, sizeof(sqlca0));
	bind_SQLCA(pset.sqlca); 


	/* ===================================================== 
	 * Connect to Database 
	 * ====================================================*/
	ret = parse_connect(&pset, singleQuery, target_name, connect_name, 
						user_name, passwd);

	if (ret == 0)
	{
		strcpy(sql_word, "CONNECT");

		if (exec_connect(&pset, target_name, connect_name, user_name, passwd) == 0)
			goto normal_end;
		else
			goto exec_error;
	}
	else if (ret < -1)
		goto syntax_error;

	/* ===================================================== 
	 * DisConnect connect-name
	 * ====================================================*/
	ret = parse_disconnect(&pset, singleQuery, connect_name);

	if (ret == 0)
	{
		strcpy(sql_word, "DISCONNECT");
		if (exec_disconnect(&pset, connect_name) == 0)
		{
			if (!pset.quiet && !pset.notty)
				fprintf(pset.fout, "# DISCONNECT  %s%s\n", 
						connect_name, get_CGI_BR());
			goto normal_end;
		} else
			goto exec_error;
	}
	else if (ret < -1)
		goto syntax_error;

	/* ===================================================== 
	 * Set Connection connect_name
	 * ====================================================*/
	ret = parse_set_connection(&pset, singleQuery, connect_name);

	if (ret == 0)
	{
		strcpy(sql_word, "SET");
		if (exec_set_connection(&pset, connect_name) == 0)
		{
			if (!pset.quiet && !pset.notty)
				fprintf(pset.fout, "# SET CONNECTION %s%s\n",
						connect_name, get_CGI_BR());
			goto normal_end;
		} else
			goto exec_error;
	}
	else if (ret < -1)
		goto syntax_error;

	/******************************************************** 
	 * Connect to Database automatically
	 ********************************************************/
	if (psconninfo.maxno == 0)
	{
		parse_connect(&pset, "connect to default", target_name, 
					connect_name, user_name, passwd);

		if (exec_connect(&pset, target_name, connect_name, 
					user_name, passwd) != 0)
			goto exec_error;
	}

	/******************************************************** 
	 * check CURRENT connection 
	 ********************************************************/
	if (psconninfo.currentp != 0)
	{
		pset.dbset = psconninfo.currentp->dbset;
		cancelConn = pset.dbset->db;
	} 
	else
	{
		setSQLCA(pset.sqlca, SQL_CONNECT_ERROR,
		       "Error: Current database not found. ", 0, 0);
		goto exec_error;
	}

	/* ===================================================== 
	 * '-d'option (set connection) 
	 * ====================================================*/
	if (option->dflg.flg)
	{
		PSconnList     *work = psconninfo.currentp;

		if (strUcmp(option->dflg.opt, "DEFAULT", strlen(option->dflg.opt)) == 0)
			ret = exec_set_connection(&pset, DEFAULT_VALUE);
		else
			ret = exec_set_connection(&pset, option->dflg.opt);

		if (ret == 0)
		{
			pset.dbset = psconninfo.currentp->dbset;
			cancelConn = pset.dbset->db;
			psconninfo.currentp = work;
			wrap_bind_char_variable(POSTGRESQL_VERSION, work->dbset->ver_no);
		} else
			goto exec_error;
	}

	/* ===================================================== 
	 * execute program(e.g. lo_export)
	 *
	 * option->xflg.opt : prog_name  (e.g. "lo_export")
	 * singleQuery      : parameters.(e.g. "12345,'/tmp/test.dat'")
	 * =====================================================*/
	if (option->xflg.flg && option->xflg.opt && singleQuery)
	{
		ret = lo_exec(&pset, option->xflg.opt, singleQuery);

		if (ret == 0)
		{
			strcpy(sql_word, "LARGE_OBJECT");
			goto normal_end;
		}
		else if (ret < -1)
			goto syntax_error;
	}

	/* ===================================================== 
	 * send Query and execute Query
	 * ====================================================*/
	if (singleQuery)
	{
		if (SendQuery(&pset, singleQuery))
		{
			get_sql_word(singleQuery, sql_word);
			goto normal_end;
		}
		else
			goto exec_error;
	}

	/* ===================================================== 
	 * The end of process.
	 * ====================================================*/

syntax_error:
	/*------------ display Syntax Error Message --------------*/
	if (!pset.quiet)
		printSQLerror(pset.fout, pset.sqlca);

exec_error:
	success = false;

normal_end:
	/*------------ write shell variables. --------------------*/
	bind_SQLCA(pset.sqlca);
	wrap_bind_char_variable(SQL_WORD, sql_word);

	if (option->sflg && singleQuery != NULL)
		printStatusSHELLvar(pset.fout);

    }  /*<<<<<<<<<<<<<<<<<<<<<<<<<<< end of sigsetjmp() <<<<<<*/

normal_option_end:
	/*-------------- set signal default ----------------------*/
	signal(SIGINT,SIG_DFL);

	if (success)
		return (0);
	else
		return (-1);
}


/*
 * ################################################################# 
 * The local subroutines for exec_sql.
 * #################################################################
 */

static void
initPSconnSet(char *singleQuery, Option *option, EXEC_SQL_OPTION *saved_option,
		PSconnSet *pset)
{
	/*-------------------------------------------------------
	 *		initial set.
	 *-------------------------------------------------------
	 */
	pset->fout = stdout;
	pset->sqlca = &sqlca0;

	/*------------ print form --------------*/
	pset->opt.html3 = 0;
	pset->opt.align = 1;
	pset->opt.header = 1;
	pset->opt.rowcount = 1;
	pset->opt.caption = NULL;
	pset->opt.outerframe = 0;

	pset->opt.fieldSep= DEFAULT_FIELD_SEP;
	pset->opt.nullstr = DEFAULT_NULL_STRING;
	pset->opt.zerostr = DEFAULT_ZERO_STRING;

	pset->opt.table_tag = NULL;
	pset->opt.header_tr = saved_option->header_tr;
	pset->opt.header_th = saved_option->header_th;
	pset->opt.body_tag = saved_option->body_tag;
	pset->opt.input_tag = saved_option->input_tag;
	pset->opt.input_size = saved_option->input_size;

	pset->opt.nFields = 0;
	pset->opt.nTups = 0;
	pset->opt.fieldName = NULL;
	pset->opt.fieldValue = NULL;
	pset->opt.fieldMax = NULL;
	pset->opt.fieldNotNum = NULL;

	/*----- echo query sent to backend ------*/
	if (option->eflg)
		pset->echoQuery = 1;

	/*------------ run quietly --------------*/
	if (option->qflg)	
		pset->quiet = 1;

	/*-------- print format options ---------*/
	if (option->Hflg)/* HTML table output */
		pset->opt.html3 = 1;

	if (option->Xflg)	/* expanded output */
		pset->opt.expanded = 1;

	if (option->Cflg.flg)	/* Caption */
		pset->opt.caption = option->Cflg.opt;

	if (option->Oflg.flg)	/* Table_TAG option */
		pset->opt.table_tag = option->Oflg.opt;
	if (option->Qflg.flg)	/* Header <TR> tag */
		pset->opt.header_tr = option->Qflg.opt;
	if (option->Rflg.flg)	/* Header <TH> tag */
		pset->opt.header_th = option->Rflg.opt;
	if (option->Dflg.flg)	/* Body <TD> tag */
		pset->opt.body_tag = option->Dflg.opt;
	if (option->Iflg)	/* Body <TD> tag */
		pset->opt.input_tag = option->Iflg;

	if (option->Lflg)	/* tern on printing of outer frame */
		pset->opt.outerframe = 1;

	if (option->Tflg)	/* tern off printing of header */
		pset->opt.header = 0;
	if (option->Bflg)	/* tern off printing of row count */
		pset->opt.rowcount = 0;
	if (option->Aflg)	/* tern off alignment */
		pset->opt.align = 0;

	if (option->Sflg.flg)	/* set the field separator */
	{
		pset->opt.fieldSep = option->Sflg.opt;
		if (strcmp(option->Sflg.opt, TAB_FIELD_SEP_STRING) == 0)
			pset->opt.fieldSep = TAB_FIELD_SEP; 
	} 

	if (option->Nflg.flg)	/* set NULL string  */
		pset->opt.nullstr = option->Nflg.opt;

	if (option->Zflg.flg)	/* set ZERO string  */
		pset->opt.zerostr = option->Zflg.opt;
}


static void
usage(FILE * fout, char *optarg)
/*******************************************************************
 *	The usage for the application program.
 *******************************************************************/
{
	int             i;

	/*------------ display usage ----------------*/
	if (optarg == NULL || optarg[0] == '\0')
	{
		fprintf(fout, "# HELP: 'help exec_sql'%s\n", get_CGI_BR());
	} 
	/*------------ Exec_sql options ------------------*/
	else if (strUcmp(optarg, "OP", 2) == 0)	
	{
		if (get_Content_type() == 1)
			fprintf(fout, "<TABLE><TR><TD><PRE>");

		i = 0;
		fprintf(fout, "# Exec_sql options:\n");

		while (exec_sql_doc[i] != NULL)
			fprintf(fout, "    %s\n", exec_sql_doc[i++]);

		if (get_Content_type() == 1)
			fprintf(fout, "</PRE></TD></TR></TABLE>");
	} 
	/*------------ SQL error code -----------------*/
	else if (strUcmp(optarg, "ER", 2) == 0)	
	{
		if (get_Content_type() == 1)
			fprintf(fout, "<TABLE><TR><TD><PRE>");

		fprintf(fout, "# SQL error code:\n");

		for (i = 0; i < 100; i++)
		{
			if (exec_sql_code[i].name == NULL)
				break;

			fprintf(fout, "    %-22.22s : %4d : %s \n",
				exec_sql_code[i].name, exec_sql_code[i].code, exec_sql_code[i].comment);
		}

		if (get_Content_type() == 1)
			fprintf(fout, "</PRE></TD></TR></TABLE>");
	}

	return;
}


static void
printStatusSHELLvar(FILE * fout)
/*******************************************************************
 *   print SQL status of bash shell variable.
 ******************************************************************/
{
	if (get_Content_type() == 1)
		fprintf(fout, "<PRE>");

	fprintf(fout, "# Status after executing SQL\n");

	print_SQLCA(fout);

	if (get_Content_type() == 1)
		fprintf(fout, "</PRE>");
}

static void
safe_write_stderr(const char *s)
/*******************************************************************
 *   A safe output.
 *******************************************************************/
{
	char           *buf;

	if (get_Content_type() == 1)
	{
		buf = wrapMalloc(strlen(s) + 5);
		strcpy(buf, s);
		strcat(buf, get_CGI_BR());
	} else
		buf = (char *) s;

#ifdef WIN32
	fputs(buf, get_Ferr());
#else
	write(fileno(get_Ferr()), buf, strlen(buf));
#endif
	if (get_Content_type() == 1)
		free(buf);
}

static void
handle_sigint(int sig)
/*******************************************************************
 *   signal handler routine. (Interactive mode only)
 *******************************************************************/
{
	/*----- accept signal if no connection -----*/
	if (cancelConn == NULL)
	{
		safe_write_stderr("\n>>> CANCEL request >>> \n");
	}
	/*----- Try to send cancel request ---------*/
	else if (PQrequestCancel(cancelConn))
	{
		safe_write_stderr("\n>>> CANCEL request sent >>>\n");
	} else
	{
		safe_write_stderr("\n>>> CANCEL request >>> \n");
		safe_write_stderr("Cannot send cancel request:\n");
		safe_write_stderr(PQerrorMessage(cancelConn));
	}

   siglongjmp(exec_sql_main_jump,1);  /*>>>> go to exec_sql_main >>>>*/
}


static int
bind_SQLCA(SQLCA * sqlca)
/*******************************************************************
 *   bind SQLCA variables.
 *******************************************************************/
{

	if (wrap_bind_int_variable(SQLCODE, (int) sqlca->sqlcode) < 0)
		return (-1);

	if (wrap_bind_int_variable(SQLERRML, (int) sqlca->sqlerrm.sqlerrml) < 0)
		return (-1);

	if (wrap_bind_char_variable(SQLERRMC, sqlca->sqlerrm.sqlerrmc) < 0)
		return (-1);

	if (wrap_bind_int_variable(SQLERRD2, (int) sqlca->sqlerrd[2]) < 0)
		return (-1);
	if (wrap_bind_int_variable(SQLNTUPLE, (int) sqlca->sqlerrd[2]) < 0)
		return (-1);

	if (wrap_bind_int_variable(SQLERRD3, (int) sqlca->sqlerrd[3]) < 0)
		return (-1);
	if (wrap_bind_int_variable(SQLNFIELD, (int) sqlca->sqlerrd[3]) < 0)
		return (-1);

	return (0);
}


static int
print_SQLCA(FILE *fout)
/*******************************************************************
 *   print SQLCA shell variables.
 *******************************************************************/
{
	char	value[256];

	if (wrap_find_variable(SQLOID, value) == 0 && value[0] != '\0')
		fprintf(fout, "  %-9.9s = %-10.10s(OID of recent insert)\n", SQLOID, value);

	if (wrap_find_variable(SQLCODE, value) == 0)
		fprintf(fout, "  %-9.9s = %-10.10s(SQL error code)\n", SQLCODE, value);

	if (wrap_find_variable(SQLNTUPLE, value) == 0)    
		fprintf(fout, "  %-9.9s = %-10.10s(number of tuples)\n", SQLNTUPLE, value);

	if (wrap_find_variable(SQLNFIELD, value) == 0)    
		fprintf(fout, "  %-9.9s = %-10.10s(number of fields)\n", SQLNFIELD, value);

	if (wrap_find_variable(SQLERRML, value) == 0)    
		fprintf(fout, "  %-9.9s = %-10.10s(length of SQLERRMC)\n", SQLERRML, value);

	if (wrap_find_variable(SQLERRMC, value) == 0)    
		fprintf(fout, "  %-9.9s = %s\n", SQLERRMC, value);

	return(0);
}

static void
get_sql_word(char *singleQuery, char *sql_word)
{
	char *p;

	strncpy(sql_word, singleQuery, SQL_WORD_LEN-1); 
	sql_word[SQL_WORD_LEN-1]='\0';

	if ( (p=strchr(sql_word,' ')) != NULL )
		*p = '\0';

	p=sql_word;
	while(*p)
	{
		*p = toupper(*p);
		p++;
	}
}
