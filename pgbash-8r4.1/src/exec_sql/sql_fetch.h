/*----------------------------------------------------------------
 * sql_fetch.h
 *
 * Change Logs
 * 2001.06.11: Add parse_select_fetct_into();
 *
 *-----------------------------------------------------------------
 */

#ifndef SQL_FETCH_HEADER
#define SQL_FETCH_HEADER

#include "exec_sql.h"

/*-------------------------------------------------------------------
	host variables and indicators list.
-------------------------------------------------------------------*/
typedef struct _PSfetchList
{
	char    *hostvar;/* host variable name */
	char    *indicat;/* indicator name     */

	struct _PSfetchList *next; 

} PSfetchList;


/*-----------------------------------------------------------------
 *   			Function List
 *---------------------------------------------------------------*/
extern int  exec_fetchInto(PSconnSet *pset, PGresult *res, PSfetchList *pfet0);
extern int  parse_select_fetch_into(char *stmt, PSfetchList **pfet0);
extern void free_PSfetchList(PSfetchList *pfet0);


#endif
