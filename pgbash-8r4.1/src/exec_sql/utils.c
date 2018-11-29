/*----------------------------------------------------------------
 * utils.c
 *    utility routines.
 *
 * Change Logs
 * 2001.04.15: Add pgbash_version_string().
 * 2001.06.11: Add CGI print mode in printSQLerror().
 * 2002.04.06: Allow database user name as e.g. '00091'.
 *
 *-----------------------------------------------------------------
 */
/*
char *pgbash_version_string() : get PGBASH+BASH version string.
int strUcmp()       : Compare toupper(string1) and string2
int checkName()     : Check shell variable name string.
int checkUserName() : Check unix login name string.
char *skipSpace()   : skip while space/'\t'/'\n'
char *skipToken()   : skip until space/'\t'/'\n'/,
void setSQLCA()     : resister sqlerror.
char *getSQLerrmc() : get sqlerrmc strings.
void printSQLerror(): print SQL error messages.
void *wrapMalloc()  : malloc
char *wrapStrdup()  : strdup
void *wrapCalloc()  : calloc
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "exec_sql.h"
#include "utils.h"
#include "version.h"

char *
pgbash_version_string()
/*******************************************************************
 *   get (PGBASH version + BASH version)  string.
 ******************************************************************/
{
	extern  char *shell_version_string();
	static char  version[128];

	version[0] = '\0' ;

	strcpy(version, "Pgbash");
	strcat(version, " ( bash-");
	strcat(version, shell_version_string());
	strcat(version, " ) ");
	strcat(version, PGBASH_PATCH_VERSION);

	return(version);
}

int
strUcmp(char *str1, const char *str2, int n)
/*******************************************************************
 *   Compare toupper(string1) and "const char *str2"
 ******************************************************************/
{
	char            buf[32], *p1 = str1;
	int             i;

	if (n <= 0)
		return (-1);

	for (i = 0; i < n; i++)
		buf[i] = toupper(*p1++);
	return (strncmp(buf, str2, n));
}


int
checkName(char *name)
/*******************************************************************
 *   Check shell variable name string.
 *	( syntax:= [A-Za-z_][A-Za-z0-9_]* )
 *
 *   return(0) : OK!
 *   return(-1): error
 ******************************************************************/
{
	int             len, i;

	len = strlen(name);
	if (len <= 0 || len >= 255)
		return (-1);

	if (!((name[0] >= 'A' && name[0] <= 'Z') ||
	      (name[0] >= 'a' && name[0] <= 'z') ||
	      name[0] == '_'))
		return (-1);

	for (i = 1; i < len; i++)
	{
		if (!((name[i] >= 'A' && name[i] <= 'Z') ||
		      (name[i] >= 'a' && name[i] <= 'z') ||
		      (name[i] >= '0' && name[i] <= '9') ||
		      name[i] == '_'))
			return (-1);
	}
	return (0);
}


int
checkUserName(char *name)
/*******************************************************************
 *   Check unix login name string.
 *	( syntax:= [A-Za-z_][A-Za-z0-9_-]* )
 *
 *   return(0) : OK!
 *   return(-1): error
 ******************************************************************/
{
	int             len, i;

	len = strlen(name);
	if (len <= 0 || len >= 255)
		return (-1);

	/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>2002.04.06
	if (!((name[0] >= 'A' && name[0] <= 'Z') ||
	      (name[0] >= 'a' && name[0] <= 'z') ||
	      name[0] == '_'))
		return (-1);
	<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

	for (i = 0; i < len; i++)
	{
		if (!((name[i] >= 'A' && name[i] <= 'Z') ||
		      (name[i] >= 'a' && name[i] <= 'z') ||
		      (name[i] >= '0' && name[i] <= '9') ||
		      name[i] == '_' || name[i] == '-'))
			return (-1);
	}
	return (0);
}

char           *
skipSpace(char *p)
/*******************************************************************
 *   skip while p is a space/'\t'/'\n'
 ******************************************************************/
{
	char           *p2 = p;

	while (*p2 != '\0' && *p2 != ';')
	{
		if (*p2 > '\0' && *p2 <= ' ')
		{
			p2++;
			continue;
		} else
			break;
	}

	return (p2);
}

char           *
skipToken(char *p)
/*******************************************************************
 *   skip until p is space/'\t'/'\n'/,
 ******************************************************************/
{
	char           *p2 = p;

	while (*p2 != '\0' && *p2 != ';')
	{
		if (*p2 != ' ' && *p2 != ',' &&
		    *p2 != '(' && *p2 != ')' &&
		    *p2 != '\t' && *p2 != '\n')
		{
			p2++;
			continue;
		} else
			break;
	}

	return (p2);
}

void
setSQLCA(SQLCA * sqlca, int sqlcode, char *sqlerrmc,
	 int ntuples, int nfields)
/*******************************************************************
 *   resister sqlerror.
 ******************************************************************/
{
	int             len;

	sqlca->sqlcode = sqlcode;

	if (sqlerrmc != NULL && sqlerrmc[0] != '\0')
	{
		len = strlen(sqlerrmc);
		if (len > 255)
			len = 255;
		sqlca->sqlerrm.sqlerrml = strlen(sqlerrmc);
		strncpy(sqlca->sqlerrm.sqlerrmc, sqlerrmc, len);
		sqlca->sqlerrm.sqlerrmc[len] = '\0';
	} else
	{
		sqlca->sqlerrm.sqlerrml = 0;
		sqlca->sqlerrm.sqlerrmc[0] = '\0';
	}

	sqlca->sqlerrd[2] = ntuples;
	sqlca->sqlerrd[3] = nfields;
	sqlca->sqlwarn[0] = 'W';
	sqlca->sqlwarn[3] = '0';
}


char           *
getSQLerrmc(char *fmt, char *errmc)
/*******************************************************************
 *   get sqlerrmc strings.
 ******************************************************************/
{
	static char     buf[256];
	char            format[256];
	char           *p;
	int             len1, len2;

	strcpy(format, "Error: ");
	strcat(format, fmt);

	len1 = strlen(format);
	len2 = strlen(errmc);

	if (len1 + len2 > 255)
		p = (char *) wrapMalloc(len1 + len2 + 1);
	else
		p = buf;

	sprintf(p, format, errmc);

	if (len1 + len2 > 255)
	{
		strncpy(buf, p, 255);
		buf[255] = '\0';
		free(p);
	}
	return (buf);
}


void
printSQLerror(FILE * fout, SQLCA * sqlca)
/*******************************************************************
 *   print SQL error messages.
 ******************************************************************/
{
	if( *get_CGI_BR() == '\0' )
		fprintf(fout, "(%d)%s\n",
			(int) sqlca->sqlcode, sqlca->sqlerrm.sqlerrmc);
	else
		fprintf(fout, "<B>(%d)%s</B><BR>\n",
			(int) sqlca->sqlcode, sqlca->sqlerrm.sqlerrmc);
	fflush(fout);
}

#ifdef EXEC_SQL_DEBUG
void
printStatus(FILE * fout, SQLCA * sqlca)
/*******************************************************************
 *   print SQL status (SQLCA).
 ******************************************************************/
{
	fprintf(fout, "# SQL status%s\n", get_CGI_BR());
	fprintf(fout, "  %-9.9s = %-7d#OID of recent insert%s\n",
		"OID", (int) sqlca->recent_oid, get_CGI_BR());
	fprintf(fout, "  %-9.9s = %-7d#SQL error code%s\n",
		SQLCODE, (int) sqlca->sqlcode, get_CGI_BR());
	fprintf(fout, "  %-9.9s = %-7d#number of tuples%s\n",
		SQLNTUPLE, (int) sqlca->sqlerrd[2], get_CGI_BR());
	fprintf(fout, "  %-9.9s = %-7d#number of fields%s\n",
		SQLNFIELD, (int) sqlca->sqlerrd[3], get_CGI_BR());
	fprintf(fout, "  %-9.9s = %-7d#length of SQLERRMC%s\n",
		SQLERRML, sqlca->sqlerrm.sqlerrml, get_CGI_BR());
	fprintf(fout, "  %-9.9s = %s%s\n",
		SQLERRMC, sqlca->sqlerrm.sqlerrmc, get_CGI_BR());
}
#endif

void           *
wrapMalloc(size_t size)
{
	void	*p;

	if ((p = malloc(size)) == NULL)
	{
		fprintf(get_Ferr(), "SYSTEM ERROR: malloc() : memory allocation%s\n", get_CGI_BR());
		exit(1);
	} else
		return (p);
}

char           *
wrapStrdup(const char *str)
{
	char	*p;

	if ((p = strdup(str)) == NULL)
	{
		fprintf(get_Ferr(), "SYSTEM ERROR: strdup() : memory allocation%s\n", get_CGI_BR());
		exit(1);
	} else
		return (p);
}

void           *
wrapCalloc(size_t number, size_t size)
{
	void	*p;

	if ((p = calloc(number, size)) == NULL)
	{
		fprintf(get_Ferr(), "SYSTEM ERROR: calloc() : memory allocation%s\n", get_CGI_BR());
		exit(1);
	} else
		return (p);
}

