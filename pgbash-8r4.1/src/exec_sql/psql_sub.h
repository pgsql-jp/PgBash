/*-------------------------------------------------------------------------
 *
 * psql_sub.h
 *
 * Copyright (c) 1996, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: $
 *
 *-------------------------------------------------------------------------
 */

#ifndef PSQL_SUB_HEADER
#define PSQL_SUB_HEADER

#include "exec_sql.h"

/*------------------------------------------------------------------------
 *	Functions list
 *-----------------------------------------------------------------------*/
extern bool SendQuery(PSconnSet * pset, char *query);
extern int  lo_exec(PSconnSet *pset, char *prog_name, char *parameter);


#endif
