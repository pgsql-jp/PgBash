/*-------------------------------------------------------------------
 *
 * cgi_sub.c
 * 	"uncgi.c" subset.
 *
 * "uncgi.c" is copyrighted 1994, Steven Grimm <koreth@hyperion.com>.
 * Permission is granted to redistribute freely and use for any purpose,
 * commercial or private, so long as this copyright notice is retained
 * and the source code is included free of charge with any binary
 * distributions.
 *
 * This software was modified by SAKAIDA Masaaki -- Osaka,Japan.
 *
 * Change Logs
 * 2001.04.15: Change bind_variable() to wrap_bind_char_variable().
 * 2001.04.15: Delete read_only_check shell variables.
 * 2001.06.11: Add stock_host_name(), find_host_value() and insert_host_value().
 * 2003.10.13: duplicate entry for array operatin by T.Nomura.
 * 2011.08.10: Add the parameter 0 in bind_array_variable().
 * 2011.08.10: Delete functionality of "EXEC_SQL_PREPARE".
 * 2011.08.15: Change bind_array_variable() to wrap_bind_array_variable().
 *
 *-------------------------------------------------------------------
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "exec_sql.h"
#include "utils.h"

////#define EXEC_SQL_PREPARE_EXECUTE	// EXEC_SQL_PREPARE functionality

#ifdef  EXEC_SQL_PREPARE_EXECUTE
#define EXEC_SQL_PREPARE "EXEC_SQL_PREPARE"
#endif

#define ishex(x) \
	(((x) >= '0' && (x) <= '9') || ((x) >= 'a' && (x) <= 'f') || \
	((x) >= 'A' && (x) <= 'F'))

#define MAX_CNAME 512           /* Max. number of shell variables */

static char *Cname[MAX_CNAME];	/* Array of shell variable names */
static char *Cvalue[MAX_CNAME]; /* Array of values */
static int  M_Cname[MAX_CNAME]; /* Max Index of Cname in case of array */

static int  N_Cname  = 0;		/* No.  of shell variables  */
static int  L_Cvalue = 0;		/* Total length of values */


static void 
free_host_name()
/*****************************************************************
 * clear the host variable names and values.
 *****************************************************************/
{
	int	len;
	int	i;

	for (i=0; i<N_Cname; i++)
	{
		free(Cname[i]);
		free(Cvalue[i]);
	}
	N_Cname = 0;
	L_Cvalue = 0;
}

static char *
find_host_name(char *name, int *nlen, int *ind )
/*****************************************************************
 *   Find the host variable name.
 *
 *   Return : *value :  name of the shell variable or NULL
 *   *nlen  :  value length
 *   *ind   :  index of Cname[*], Cvalue[*], M_Cname[*].
 *****************************************************************/
{
	int	len;
	int	i;

	for (i=0; i<N_Cname; i++)
	{
		len = strlen(&Cname[i][0]);

		if (strncmp(name, &Cname[i][0], len) == 0)
		{
			if( (*(name+len) >= '0'  && *(name+len) <= '9' ) ||
			    (*(name+len) >= 'a'  && *(name+len) <= 'z' ) ||
			    (*(name+len) >= 'A'  && *(name+len) <= 'Z' ) ||
			    (*(name+len) >= '_'                        ) ||
			    ((unsigned char)(*(name+len)) >= 128 )       )
			{
				continue;
			}
			else
			{
				len = strlen(&Cvalue[i][0]);
				*nlen = len;
				*ind = i ;
				return(&Cvalue[i][0]);
			}
		}
	}
	*nlen = 0;
	*ind  = -1;
	return(NULL);
}

#ifdef EXEC_SQL_PREPARE_EXECUTE
static char *
find_host_value(char *name, int *nlen)
/*****************************************************************
 * find value of a shell variable name.
 * return len: name length.
 *****************************************************************/
{
	int	len;
	int	i;

	for (i=0; i<N_Cname; i++)
	{
		len = strlen(&Cname[i][0]);

		if (strncmp(name, &Cname[i][0], len) == 0)
		{
			if( (*(name+len) >= '0'  && *(name+len) <= '9' ) ||
			    (*(name+len) >= 'a'  && *(name+len) <= 'z' ) ||
			    (*(name+len) >= 'A'  && *(name+len) <= 'Z' ) ||
			    ((unsigned char)(*(name+len)) >= 128 )       )
			{
				continue;
			}
			else
			{
				*nlen = len;
				return(&Cvalue[i][0]);
			}
		}
	}
	*nlen = 0;
	return(NULL);
}
static char *
insert_host_value(char *name, char *value)
/*****************************************************************
 * Chnage ':' hist variable.
 *****************************************************************/
{
	char	*cvalue, *pc;
	char	*pv=value, *pv2;
	char	*p;
	int	i, j;
	int	nlen, vlen;
	int	cflag=0;

	/*------ check ':' character. -----*/
	if ((pv2=strchr(pv,':')) == NULL)
		return(value);

	/*---------------- new string --------------------*/
	cvalue = (char *)wrapMalloc( strlen(value)+ L_Cvalue + 1);
	pc = cvalue;

	strncpy(pc, pv, (pv2-pv)); 
	pc += (pv2-pv);
	pv = pv2;

	while(*pv)
	{
		if ((p=find_host_value( pv+1, &nlen )) != NULL)
		{
			vlen = strlen(p);

			strncpy(pc, p, vlen);
			pc += vlen;
			*pc = '\0';

			pv += nlen+1;
			cflag = 1;
		}
		else
		{
			*pc = *pv;
			pc++; pv++;
			*pc = '\0';
		}

		if ( (pv2=strchr(pv, ':')) != NULL)
		{
			strncpy(pc, pv, (pv2-pv));
			pc += (pv2-pv);
			*pc = '\0';
			pv = pv2;
			continue;
		}
		else
		{
			strcpy(pc, pv);
			break;
		}
	}

	if (cflag == 1)
		return(cvalue);
	else
	{
		free(cvalue);
		return(value);
	}
}
#endif

static char * 
stock_host_name(char *name, char *value, int *mcnt)
/*****************************************************************
 *  Stock a pair of name and value into Cname[*], Cvalue[*]
 *
 *  Return : *value, same as input parameter 'value'
 *  *mcnt  : normally 0  if the same name exist, it returns
 *           the sequence number 0,1,2,......
 *           This is usuful in case of array operation.
 *****************************************************************/
{
	char *p;
	int  nlen, ind;
	char *tmp;

	/*------ MAx stock size over -----*/
	if (N_Cname >= MAX_CNAME -1)
	{
		printf("<H2>Error: uncgi() : MAX_CNAME over<H2>\n");
		return(value);
	}

#ifdef EXEC_SQL_PREPARE_EXECUTE
	/*------------- stock shell variable --------------*/
	if (strncmp(name, EXEC_SQL_PREPARE, 16) == 0)
	{
		p = insert_host_value(name, value);
		return(p);
	}
	else
	{
#endif
	/*------------- stock shell variables --------------*/
	if ( (p=find_host_name(name, &nlen, &ind)) == NULL ) 
	{ 
		Cname[N_Cname] = (char *)wrapMalloc(strlen(name)+1);
		Cvalue[N_Cname] = (char *)wrapMalloc(strlen(value)+1);
		
		strcpy(&Cname[N_Cname][0], name);
		strcpy(&Cvalue[N_Cname][0], value);

		M_Cname[N_Cname] = 0;  
		*mcnt = M_Cname[N_Cname];
		N_Cname++;
		L_Cvalue += strlen(value)+1;
	} 
	else 
	{
		/* duplicate entry for array operatin 2003.10.13 T.Nomura */
		tmp = strdup(Cvalue[ind]);
		free(Cvalue[ind]);
		Cvalue[ind] = (char *)wrapMalloc(strlen(value)+1);
		free(tmp);

		strcpy(&Cvalue[ind][0], value);
		L_Cvalue += strlen(value)-strlen(tmp);
		M_Cname[ind]  ++;  	
		*mcnt = M_Cname[ind];
	}
#ifdef EXEC_SQL_PREPARE_EXECUTE
	}
#endif
	return(value);
}


static int
htoi(unsigned char *s)
/*****************************************************************
 * Convert two hex digits to a value.
 *****************************************************************/
{
	int             value;
	char            c;

	c = s[0];
	if (isupper(c))
		c = tolower(c);
	value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

	c = s[1];
	if (isupper(c))
		c = tolower(c);
	value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

	return (value);
}

static void
url_unescape(unsigned char *str)
/*****************************************************************
 * Get rid of all the URL escaping in a string.  Modify it in place,
 * since the result will always be equal in length or smaller.
 *****************************************************************/
{
	unsigned char  *dest = str;

	while (str[0])
	{
		if (str[0] == '+')
			dest[0] = ' ';
		else if (str[0] == '%' && ishex(str[1]) && ishex(str[2]))
		{
			dest[0] = (unsigned char) htoi(str + 1);
			str += 2;
		} else
			dest[0] = str[0];

		str++;
		dest++;
	}

	dest[0] = '\0';
}

static void
print_error(char *msg)
/*****************************************************************
 * Print the start of an error message.
 *****************************************************************/
{
	if (get_Content_type() == 1)
	{
		printf("<B>CGI error: ");
		printf(msg);
		printf("</B><BR>\n");
	} else
	{
		printf("CGI error: ");
		printf(msg);
		printf("\n");
	}
}

static int
stuffenv(char *var, const char delim, int *count)
/*****************************************************************
 * Stuff a URL-unescaped variable, with the prefix on its name,
 * into the environment.  Uses the "=" from the CGI arguments.
 * Putting an "=" in a field name is probably a bad idea.
 *
 * If the variable name begins with an underline, strip whitespace
 * from the start and end and normalize end-of-line characters.
 *
 *  delim = QUERY_DELIMITER('&') : "GET/POST"
 *          COOKIE_DELIMITER(';'): "HTTP_COOKIE"
 *****************************************************************/
{
	char           *c, *s, *t;
	char           *buf = NULL;
	int             despace = 0, got_cr = 0;
	int             success = 0;
	int             mcnt;

	/*----------- unescale character ---------*/
	url_unescape(var);

	buf = wrapStrdup(var);
	strcpy(buf, var);

	/*-----------------------------------------------------------
	 * If, for some reason, there wasn't an = in the query string,
	 * add one so the environment will be valid.
	 *
	 * Also, change periods to spaces so folks can get at "image"
	 * input fields from the shell, which has trouble with periods
	 * in variable names.
	 *-----------------------------------------------------------*/
	for (c = buf; *c != '\0'; c++)
	{
		if (*c == '.')
			*c = '_';
		if (*c == '=')
			break;
	}
	if (*c == '\0')
		c[1] = '\0';

	/*------------------------------------------------------------
	 * Do whitespace stripping, if applicable.  Since this can only ever
	 * shorten the value, it's safe to do in place.
	 *-----------------------------------------------------------*/
	if (despace && c[1])
	{
		for (s = c + 1; *s && isspace(*s); s++)
			;
		t = c + 1;
		while (*s)
		{
			if (*s == '\r')
			{
				got_cr = 1;
				s++;
				continue;
			}
			if (got_cr)
			{
				if (*s != '\n')
					*t++ = '\n';
				got_cr = 0;
			}
			*t++ = *s++;
		}

		/* Strip trailing whitespace if we copied anything. */
		while (t > c && isspace(*--t))
			;
		t[1] = '\0';
	}
	*c = '\0';

	/*---------------- "GET/POST" ----------------------*/
	if (delim == QUERY_DELIMITER)
	{
		char *p;

		/*------- stock a name and the value -------*/
		p = stock_host_name(buf, c + 1, &mcnt);

		/*-------- set shell variable --------*/
//printf("buf=(%s) mcnt=(%d) p=(%s)<br>\n", buf, mcnt, p);

		if(mcnt > 0)
			wrap_bind_array_variable(buf, mcnt, p, 0);   
		else
			bind_variable(buf, p, 0);   
	}
	/*---------------- HTTP_COOKIE ---------------------*/
	else if (delim == COOKIE_DELIMITER)
	{
		if( *count > 0 )
		{
			wrap_bind_array_variable(HTTP_COOKIEKEY, *count, buf, 0);
			wrap_bind_array_variable(HTTP_COOKIEVAL, *count, c+1, 0);
		}
		else
		{
			bind_variable(HTTP_COOKIEKEY, buf, 0);
			bind_variable(HTTP_COOKIEVAL, c+1, 0);
		}
	}
	/*----------------- free memory --------------------*/
	if (buf != NULL)
		free(buf);

	return (success);
}

static void
scanquery(char *q, const char delim)
/*****************************************************************
 * Scan a query string, stuffing variables into the environment.
 * This should ideally just use strtok(), but that's not available
 * everywhere.
 *
 *  delim = QUERY_DELIMITER('&') : "GET/POST"
 *          COOKIE_DELIMITER(';'): "HTTP_COOKIE"
 *****************************************************************/
{
	char           *next = q;
	int             count = 0;

	/*------ separate delimitor and stuffenv() ----*/
	do
	{
		while (*next && *next != delim)
			next++;
		if (!*next)
			next = NULL;
		else
			*next = '\0';

		/**************************/
		stuffenv(q, delim, &count);
		/**************************/
		count++;

		if (next)
		{
			*next++ = delim;
			if (delim == COOKIE_DELIMITER)
				next++;
		}
		q = next;

	} while (q != NULL);

	/*------------ set number of COOKIE ------------*/
	if (delim == COOKIE_DELIMITER)
	{
		wrap_bind_int_variable(HTTP_NCOOKIE, count);
	}
}


static char    *
postread(void)
/*****************************************************************
 * Read a POST query from standard input into a dynamic buffer.
 * Terminate it with a null character.
 *****************************************************************/
{
	char           *buf = NULL;
	int             size = 0, sofar = 0, got;

	buf = getenv("CONTENT_TYPE");

	if (buf == NULL || strcmp(buf, "application/x-www-form-urlencoded"))
	{
		print_error("CONTENT_TYPE was not passed.");
		return (NULL);
	}
	buf = getenv("CONTENT_LENGTH");

	if (buf == NULL)
	{
		print_error("CONTENT_LENGTH was not passed.");
		return (NULL);
	}
	size = atoi(buf);
	buf = wrapMalloc(size + 1);

	do
	{
		got = fread(buf + sofar, 1, size - sofar, stdin);
		sofar += got;
	} while (got && sofar < size);

	buf[sofar] = '\0';

	return (buf);
}


int
uncgi(void)
/*****************************************************************
 * Main program, optionally callable as a library function.
 *
 *  return == 0 : normal
 *           -1 : error
 *****************************************************************/
{
	char           *query, *method, *cookie;
	char           *dupquery = NULL;

//printf("QUERY_STRING=(%s)<br>\n", getenv(QUERY_STRING) );
//printf("REQUEST_METHOD=(%s)<br>\n", getenv(REQUEST_METHOD) );
//printf("HTTP_COOKIE=(%s)<br>\n", getenv(HTTP_COOKIE) );

	/*--------- method = "GET" ---------------*/
	query = getenv(QUERY_STRING);

	if (query != NULL && strlen(query))
	{
		dupquery = wrapStrdup(query);

		/*------ parse query string ------*/
		if (dupquery != NULL)
			scanquery(dupquery, QUERY_DELIMITER);

		free(dupquery);
	}
	/*--------- method = "POST" --------------*/
	method = getenv(REQUEST_METHOD);

	if (method != NULL && !strcmp(method, "POST"))
	{
		/*------ input query from stdin ---*/
		dupquery = postread();

//printf("query=(%s)<br>\n", dupquery );

		/*------ parse query string ------*/
		if (dupquery != NULL && dupquery[0] != '\0')
			scanquery(dupquery, QUERY_DELIMITER);

		free(dupquery);
	}
	/*------------- HTTP_COOKIE --------------*/
	cookie = getenv(HTTP_COOKIE);

	if (cookie != NULL)
	{
		dupquery = wrapStrdup(cookie);

		/*------ parse query string ------*/
		if (dupquery != NULL)
			scanquery(dupquery, COOKIE_DELIMITER);

		free(dupquery);
	}
	/*-------------- END ---------------------*/
	(void)free_host_name();    /* free memory */

	if (query == NULL && dupquery == NULL)
	{
		/* >print_error("Couldn't find a query to process.");<< */
		return (-1);
	}
	return (0);
}

