/*----------------------------------------------------------------
 * sql_connect.h
 *
 * Change logs
 * 2000.03.23: Add "encoding","ver_no" to PSconnList struct.
 * 2011.08.10: Change TARGET_NAME_MAXLEN(129) to 256
 * 2011.08.10: Change CONNECT_NAME_MAXLEN(19) to 128
 * 2011.08.10: Change USER_NAME_MAXLEN(9)     to 256
 * 2011.08.10: Change PASSWD_MAXLEN(17)       to 256
 *
 *-----------------------------------------------------------------
 */

#ifndef SQL_CONNECT_HEADER
#define SQL_CONNECT_HEADER

#include "exec_sql.h"

/*-----------------------------------------------------------------
 *   connct to TARGET_NAME as CONNCT_NAME user USER_NAME [PASSWD]
 *   set connection CONNCT_NAME
 *   disconnct CONNCT_NAME
 *---------------------------------------------------------------*/
#define TARGET_NAME_MAXLEN	256
#define CONNECT_NAME_MAXLEN	128	
#define USER_NAME_MAXLEN  	256
#define PASSWD_MAXLEN	  	256

/*-------------------------------------------------------------------
	The connected database information list.
-------------------------------------------------------------------*/
typedef struct _PSconnList
{
	char    openflag;       /* connect=1  disconnect=0 */
	char    *target_name;   /* "dbname@host:port"  */
	char    *connect_name;  /* connect name and 'SHELL VAR' name */
	char    *user_name;
	PSconn  *dbset;         /* connect information */

	struct _PSconnList *next;
} PSconnList;

typedef struct _PSconnInfo
{
	int         maxno;
	PSconnList  *firstp;
	PSconnList  *lastp;
	PSconnList  *currentp;
	PSconnList  *defaultp;

} PSconnInfo;

extern PSconnInfo psconninfo;

/*-----------------------------------------------------------------
 *   			Function List
 *---------------------------------------------------------------*/
extern int  exec_connect(PSconnSet * pset, char *target_name, 
                   char *connect_name, char *user_name, char *passwd);
extern int  exec_disconnect(PSconnSet * pset, char *connect_name);
extern int  exec_set_connection(PSconnSet * pset, char *connect_name);
extern void list_AllconnectDB(FILE * fout, PSprintOpt * po, int encoding);

extern int  parse_connect(PSconnSet *pset, char *connect_stmt, char *target_name,
                       char *connect_name, char *user_name, char *passwd);
extern int  parse_disconnect(PSconnSet *pset, char *disconnect_stmt, char *connect_name);
extern int  parse_set_connection(PSconnSet *pset, char *set_connect_stmt, char *connect_name);
extern int  separate_target_name(char *target_name, char *dbname, char *host, char *port);
extern char *compose_target_name(char *dbname, char *host, char *port);
extern void list_PSconnInfo(FILE * fout);


#endif
