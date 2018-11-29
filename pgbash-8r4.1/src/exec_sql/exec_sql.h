/*----------------------------------------------------------------
 *
 * exec_sql.h
 *    A direct/embedded SQL bash built-in command for postgreSQL.
 *
 * Change logs
 * 2000.03.22: Add encoding to PSconnSet struct. 
 * 2000.03.23: Add PSconn struct and Change PSconnSet struct.
 * 2001.04.15: Change not to use PSconn.encoding.
 * 2001.04.15: Add EXEC_SQL_OPTION typedef struct.
 * 2001.04.15: Delete -l,-p,-r,-y option flags.
 * 2001.04.15: Delete bind_variable() routine definitions.
 * 2001.06.11: Add Qflg, Rflg, Dflg 
 * 2001.11.24: Delete "postgres.h", and add boolean type.
 * 2002.02.10: Add the encoding_character in PSconn.
 *
 *-----------------------------------------------------------------
 */

#ifndef EXEC_SQL_HEADER
#define EXEC_SQL_HEADER

#include <stdio.h>
#include "libpq-fe.h"
#include "print_sub.h"
#include "sqlca.h"

/*------------------------------------------------------------------
 *  boolean type and value.
 *----------------------------------------------------------------*/
#ifndef bool
#define bool	char
#endif
#ifndef true
#define true	((char)1)
#endif
#ifndef false
#define false	((char)0)
#endif

/*------------------------------------------------------------------
 *  initial values.
 *----------------------------------------------------------------*/
#define EXEC_SQL_MAXOPTION	32
#define COPYBUFSIZ		8192

#define DEFAULT_VALUE		"_DEFAULT_"
#define CURRENT_VALUE		"_CURRENT_"
#define ALL_VALUE			"_ALL_"

#define QUERY_DELIMITER		'&'
#define COOKIE_DELIMITER	';'

/*------------------------------------------------------------------
 *  shell variable name.
 *----------------------------------------------------------------*/
#define POSTGRESQL_VERSION	"POSTGRESQL_VERSION"
#define SQLOID 			"SQLOID"
#define SQLCODE			"SQLCODE"
#define SQLERRML		"SQLERRML"
#define SQLERRMC		"SQLERRMC"
#define SQLERRD2		"SQLERRD2"
#define SQLERRD3		"SQLERRD3"
#define SQLCGI			"SQLCGI"
#define SQLNFIELD		"SQLNFIELD"
#define SQLNTUPLE		"SQLNTUPLE"
#define SQLFIELDNAME	"SQLFIELDNAME"	/* array variable */

#define QUERY_STRING	"QUERY_STRING"
#define REQUEST_METHOD	"REQUEST_METHOD"

#define HTTP_COOKIE		"HTTP_COOKIE"
#define HTTP_COOKIEKEY	"HTTP_COOKIEKEY"	/* array variable */
#define HTTP_COOKIEVAL	"HTTP_COOKIEVAL"	/* array variable */
#define HTTP_NCOOKIE	"HTTP_NCOOKIE"

#define EXEC_SQL_USAGE "\n\
exec_sql [option] [\"SQL\"]\n\
"

#ifndef SQL_ASCII
#define SQL_ASCII 0
#endif


/*-------------------------------------------------------------------
	Typedef of comman_line exec_sql options.
-------------------------------------------------------------------*/
typedef struct _Option
{
	bool	iflg;	/* set CGI mode and set data */

	struct {
		bool	flg;
		char	*opt;	/* help */
	} hflg;

	bool	vflg;	/* print version */

	struct {
		bool	flg;
		char	*opt;	/* connect_name */
	} dflg;

	bool	eflg;	/* echo queries sent to the backend */
	bool	qflg;	/* tern off output messages      */

	bool	Hflg;	/* HTML  table type output */
	bool	Xflg;	/* set EXPANDED type       */

	struct {
		bool	flg;	/* HTML/PLAIN caption flag */
		char	*opt;	/* HTML/PLAIN caption */
	} Cflg;

	struct {
		bool	flg;	/* HTML table options flag */
		char	*opt;	/* HTML table options */
	} Oflg;

	struct {
		bool	flg;	/* HTML <TR> header tag flag */
		char	*opt;	/* HTML <TR> header tag options */
	} Qflg;

	struct {
		bool	flg;	/* HTML <TH>,.. header tag flag */
		char	*opt;	/* HTML <TH>,.. header tag options */
	} Rflg;

	struct {
		bool	flg;	/* HTML <TD>,.. header tag flag */
		char	*opt;	/* HTML <TD>,.. header tag options */
	} Dflg;

	bool	Iflg;	/* turn on INPUT tag       */
	bool	Lflg;	/* turn on  printing of outer frame */
	bool	Tflg;	/* turn off printing of table heading */
	bool	Bflg;	/* turn off printing of row count */

	struct {
		bool	flg;	/* field separator flag */
		char	*opt;	/* field separator (default is '|') */
	} Sflg;

	struct {
		bool	flg;	/* NULL string     flag */
		char	*opt;	/* NULL string (default is '-N-') */
	} Nflg;

	struct {
		bool	flg;	/* BIT 0 string     flag */
		char	*opt;	/* BIT 0 string (default is '-0-') */
	} Zflg;

	bool	Aflg;	/* turn off alignment            */
	bool	mflg;	/* list connected databases list */
	bool	sflg;	/* print SQL status (shell variable) */

	struct {
		bool	flg;	
		char	*opt;	/* execute program(e.g. lo_export) */
	} xflg;

} Option;


/*-------------------------------------------------------------------
	Typedef of the DB's setting for PostgreSQL
-------------------------------------------------------------------*/
typedef struct sqlca SQLCA;

#define		PS_ENCODING_CHAR_LEN	16

typedef struct _PSconn
{
	PGconn      *db;		/* connection to backend */
	int         encoding;	/* client encoding(No.)  */
	char        encodingchar[PS_ENCODING_CHAR_LEN+1];	/* client encoding(char) */
	char        ver_no[6];	/* Backend Version Number(ex.7.0.1) */
	bool        exist_des;	/* 'pgbash_description' exist or not */

} PSconn;

typedef struct _PSconnSet
{
	PSconn      *dbset;	/* connection information          */
	FILE        *fout;	/* where to send the query results */
	PSprintOpt  opt;	/* options to be passed to PQprint */
	bool        notty;	/* input or output is not a tty    */
	bool        echoQuery;	/* echo the query before sending it */
	bool        quiet;	/* run quietly, no messages, no promt */
	SQLCA       *sqlca;	/* sql communication are           */

} PSconnSet;

/*-------------------------------------------------------------------
	Typedef of SQL error code
-------------------------------------------------------------------*/
typedef struct _exec_sql_code
{
	char    *name;	/* error code name */
	int     code;	/* error code      */
	char    *comment;/* error code comment */

} EXEC_SQL_CODE;

/*-------------------------------------------------------------------
 *	  external functions
 *-----------------------------------------------------------------*/
extern int  exec_sql_init(int interactive_shell);
extern int  exec_sql_main(Option * option, char *singleQuery);
extern int  get_Content_type();
extern char *get_CGI_BR();
extern FILE *get_Ferr();
extern char *pgbash_version_string();

/*-------------------------------------------------------------------
 *	  external variables
 *-----------------------------------------------------------------*/
extern PSconnSet      psconnset;
extern int            uncgi(void);
extern EXEC_SQL_CODE  exec_sql_code[];
extern char           *exec_sql_doc[];


#endif
