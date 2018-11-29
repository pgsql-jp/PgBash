/*----------------------------------------------------------------
 *
 * exec_sql_init.c
 *    Initialize global variables and shell variables.
 *    This exec_sql_init() is only once called by 
 *		initialize_shell_variables() in (bash)variables.c .
 *
 * Change Logs
 * 2001.04.15: Delete SQL_INITIALIZED_FLAG shell variable.
 * 2001.04.15: Modify PGBASH+BASH version print.
 * 2011.08.06: Change the welcome messages.
 *
 *-----------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "exec_sql.h"
#include "sql_connect.h"
#include "sql_set.h"

/*-------------------------------------------------------------------
 *    SQL error code
 *------------------------------------------------------------------*/
EXEC_SQL_CODE   exec_sql_code[] =
{
	{"SQL_OK", 0, "normal end."},
	{"SQL_NOT_FOUND", 100, "EOF(End Of File)."},

	{"SQL_OUT_OF_MEMORY",      -12, "out of memory."},

	{"SQL_SYSTEM_ERROR",      -200, "system error."},
	{"SQL_TOO_MANY_ARGUMENTS",-201, "too many arguments in fetch_stmt."},
	{"SQL_TOO_FEW_ARGUMENTS", -202, "too few  arguments in fetch_stmt."},
	{"SQL_CONNECT_ERROR",     -203, "database connection error."},
	{"SQL_INT_FORMAT",        -204, "int format error."},
	{"SQL_UINT_FORMAT",       -205, "uint format error."},
	{"SQL_FLOAT_FORMAT",      -206, "float format error."},
	{"SQL_NUMERIC_FORMAT",    -207, "numeric format error."},
	{"SQL_INTERVAL_FORMAT",   -208, "interval format error."},
	{"SQL_DATE_FORMAT",       -209, "date format error."},
	{"SQL_TIMESTAMP_FORMAT",  -210, "timestamp format error."},
	{"SQL_CONVERT_BOOL",      -211, "convert bool error."},
	{"SQL_EMPTY",             -212, "empty."},
	{"SQL_MISSING_INDICATOR", -213, "missing_indicator."},
	{"SQL_NO_ARRAY",          -214, "no array."},
	{"SQL_DATA_NOT_ARRAY",    -215, "data not array."},
	{"SQL_ARRAY_INSERT",      -216, "array insert error."},
	{"SQL_NO_CONN",           -220, "no connection."},
	{"SQL_NOT_CONN",          -221, "can not connection."},
	{"SQL_INVALID_STMT",      -230, "invalid statements."},
	{"SQL_READONLY_SHELLVAR", -231, "can not set read-only shell variable."},
	{"SQL_DB_NOT_OPEN",       -232, "database not open."},
	{"SQL_CNAME_NOT_FOUND",   -233, "connect-name not found."},
	{"SQL_CNAME_ALREADY_USE", -234, "connect-name already exist."},
	{"SQL_INVALID_COMMAND",   -235, "invalid command."},
	{"SQL_INVALID_DATA",      -236, "invalid data."},

	{"SQL_UNKNOWN_DESCRIPTOR",-240, "unknown descriptor."},
	{"SQL_INVALID_DESC_INDEX",-241, "invalid descriptor index."},
	{"SQL_UNKNOWN_DESC_ITEM", -242, "unknown descriptor item."},
	{"SQL_VAR_NOT_NUMERIC",   -243, "variable not numeric."},
	{"SQL_VAR_NOT_CHAR",      -244, "variable not charater."},

	{"SQL_BAD_RESPONSE",      -400, "bad response(backend maybe died).\""},
	{"SQL_EMPTY_QUERY",       -401, "empty query (backend lost query)."},
	{"SQL_CONNECTION_BAD",    -402, "connection bad(disconnect backend)\""},
	{"SQL_FATAL_ERROR",       -403, "query fatal error   (SQL error on backend)\""},
	{"SQL_NONFATAL_ERROR",    -404, "query nonfatal error(SQL error on backend)\""},

	{"SQL_NULL", -1, "indicator is NULL."},

	{(char *) NULL, (int) NULL, (char *) NULL}
};


/*
 * ########################################################################### 
 * exec_sql initialization routine.
 * ###########################################################################
 */

int
exec_sql_init(int interactive_shell)
/*****************************************************************************
 *   The exec_sql buil-in command.
 *****************************************************************************/
{
	int             i;

	/*------------------------------------------------------------------
	 *      display Welcome messages
	 *------------------------------------------------------------------
	 */
	if (interactive_shell)
	{
		fprintf(stderr, "Welcome to %s\n", pgbash_version_string());
		fprintf(stderr, "\n");
		fprintf(stderr, "  Type '?'  for help with pgbash commands.\n");
		fprintf(stderr, "  Type 'exit' or 'Ctrl+D' to terminate Pgbash.\n");
		fprintf(stderr, "\n");
	}

	/*------------------------------------------------------------------
	 *     Initialize of ERRORCODE/EXEC_SQL_OPTION 
	 *------------------------------------------------------------------
	 */
	/*------------ clear PSconnSet structure -----------------*/
	psconninfo.maxno = 0;

	/*---------- write SQL ERROR CODE to shell var. -----------*/
	for (i = 0; i < 100; i++)
	{
		if (exec_sql_code[i].name == NULL)
			break;

		if ((wrap_bind_int_variable(exec_sql_code[i].name, exec_sql_code[i].code)) < 0)
			return (-1);

	}

	/*------ set EXEC_SQL_OPTION shellvariable to DEFAULT -------*/
	for (i = 0; i < 100; i++)
	{
		if (exec_sql_option_default[i].name == NULL)
			break;

		if ((wrap_bind_char_variable(exec_sql_option_default[i].name, 
					exec_sql_option_default[i].value)) < 0)
			return (-1);
	}

	/*--------- set EXEC_SQL_OPTION variable to DEFAULT ----------*/
	set_exec_sql_option_default(&exec_sql_option);

	return (0);
}
