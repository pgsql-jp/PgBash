#ifndef EXEC_SQL_ERRNO_H
#define EXEC_SQL_ERRNO_H

/*----------- indicator NULL value -----------------*/
#define SQL_NULL                -1

/*----------- SQL ERROR code -----------------------*/
#define SQL_OK                  0
#define SQL_NOT_FOUND           100

/* first we have a set of exec_sql messages, they start at 200 */
#define SQL_SYSTEM_ERROR        -200
#define SQL_TOO_MANY_ARGUMENTS  -201
#define SQL_TOO_FEW_ARGUMENTS   -202
#define SQL_CONNECT_ERROR       -203
#define SQL_INVALID_STMT        -230
#define SQL_READONLY_SHELLVAR   -231
#define SQL_DB_NOT_OPEN         -232
#define SQL_NAME_NOT_FOUND      -233
#define SQL_NAME_ALREADY_USE    -234
#define SQL_INVALID_COMMAND     -235
#define SQL_INVALID_DATA   	    -236

/* finally the backend error messages, they start at 400 */
#define SQL_BAD_RESPONSE        -400
#define SQL_EMPTY_QUERY         -401
#define SQL_CONNECTION_BAD      -402
#define SQL_FATAL_ERROR         -403
#define SQL_NONFATAL_ERROR      -404

#endif
