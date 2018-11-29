/*----------------------------------------------------------------
 * utils.h
 *	  utility routines.
 *
 * IDENTIFICATION
 *	  $Header: $
 *
 *-----------------------------------------------------------------
 */

#ifndef UTILS_HEADER
#define UTILS_HEADER

#include "exec_sql.h"

/*-------------------------------------------------------------------
	The functions list.
-------------------------------------------------------------------*/
extern int  strUcmp(char *str1, const char *str2, int n);
extern int  checkName(char *name);
extern int  checkUserName(char *name);
extern char *skipSpace(char *p);
extern char *skipToken(char *p);
extern void setSQLCA(SQLCA * sqlca, int sqlcode, char *sqlerrmc, int ntuples, int nfield);
extern char *getSQLerrmc(char *fmt, char *errmc);
extern void printSQLerror(FILE * fout, SQLCA * sqlca);
extern void printStatus(FILE * fout, SQLCA * sqlca);
extern void *wrapMalloc(size_t size);
extern char *wrapStrdup(const char *str);
extern void *wrapCalloc(size_t number, size_t size);


#endif
