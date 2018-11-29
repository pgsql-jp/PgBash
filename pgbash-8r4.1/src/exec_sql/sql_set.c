/*----------------------------------------------------------------
 *
 * sql_set.c:
 *	parse and set SET EXEC_SQL_OPTION and set
 *      CLIENT_ENCODING to pset.dbset->encoding
 *
 * Change logs
 * 2001.04.16: Add EXEC_SQL_OPTION. 
 * 2001.06.11: Add uncgi() in CGI mode.
 * 2001.06.11: Change 'TRUE' to 'ON', 'FALSE' to 'OFF'.
 * 2001.06.11: Add OPTION_HEADERTR/HEADERTH/BODYTAG.
 * 2001.06.11: Add OPTION_INPUTTAG/OPTION_INPUTSIZE.
 * 2002.02.10: Add encoding_char in set_client_encoding().
 * 2011.08.10: Add the parameter 0 in bind_variable().
 *
 *-----------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "libpq-fe.h"
#include "pg_config.h"

#include "exec_sql.h"
#include "sql_set.h"
#include "sql_errno.h"
#include "utils.h"

/*-----------------------------------------------------------------
 * EXEC_SQL_OPTION number
 *-----------------------------------------------------------------
 */
#define N_ECHO     		11
#define N_QUIET     		12
#define N_HEADER             	13
#define N_BOTTOM     		14
#define N_ALIGNMENT  		15
#define N_FRAME			16
#define N_HTML			18
#define N_EXPANDED		19

#define N_SEPARATOR		101
#define N_NULLSTRING		102
#define N_ZEROSTRING		103
#define N_CAPTION		104
#define N_TABLETAG		105
#define N_HEADERTR   	 	106
#define N_HEADERTH    		107
#define N_BODYTAG		108
#define N_INPUTTAG		109
#define N_INPUTSIZE		110

/*-------------------------------------------------------------------
 *    set EXEC_SQL_OPTION DEFAULT;
 *    set EXEC_SQL_OPTION CGI;
 *    set <option_name>[=value];
 *------------------------------------------------------------------*/
EXEC_SQL_OPTION_DEFAULT   exec_sql_option_default[] =
{
	{"OPTION_ECHO",     "OFF", N_ECHO},
	{"OPTION_QUIET",    "OFF", N_QUIET},
	{"OPTION_HEADER",   "ON",  N_HEADER},
	{"OPTION_BOTTOM",   "ON",  N_BOTTOM},
	{"OPTION_ALIGNMENT","ON",  N_ALIGNMENT},
	{"OPTION_FRAME",    "OFF", N_FRAME},
	{"OPTION_HTML",     "OFF", N_HTML},
	{"OPTION_EXPANDED", "OFF", N_EXPANDED},

	{"OPTION_SEPARATOR",  "", N_SEPARATOR},
	{"OPTION_NULLSTRING", "", N_NULLSTRING},
	{"OPTION_ZEROSTRING", "", N_ZEROSTRING},
	{"OPTION_CAPTION",    "", N_CAPTION},
	{"OPTION_TABLETAG",   "", N_TABLETAG},
	{"OPTION_HEADERTR",   "", N_HEADERTR},
	{"OPTION_HEADERTH",   "", N_HEADERTH},
	{"OPTION_BODYTAG",    "", N_BODYTAG},
	{"OPTION_INPUTTAG","OFF", N_INPUTTAG},
	{"OPTION_INPUTSIZE",  "", N_INPUTSIZE},

	{(char *) NULL, (char *) NULL, (int)NULL}
};


/*-----------------------------------------------------------------
 * EXEC_SQL_OPTION declaration.
 *---------------------------------------------------------------*/
EXEC_SQL_OPTION  exec_sql_option;

/*-----------------------------------------------------------------
 * Function list
 *-----------------------------------------------------------------
 * int set_exec_sql_options(PSconnSet *pset, char *value)
 * int set_exec_sql_option_default(EXEC_SQL_OPTION *exec_sql_option)
 */
EXEC_SQL_OPTION *get_exec_sql_options() { return(&exec_sql_option); }


/*
 * #################################################################
 * sql_set global subroutines
 * ###############################################################
 */

int
set_exec_sql_options(PSconnSet *pset, char *value)
/*******************************************************************
 * parse SET EXEC_SQL_OPTION values ..  
 *
 *  ex.                                 
 *  set EXEC_SQL_OPTION DEFAULT;
 *  set EXEC_SQL_OPTION CGI;
 *                                       Default Value
 *  set OPTION_ECHO[=ON/OFF];          : OFF
 *  set OPTION_QUIET[=ON/OFF];         : OFF
 *  set OPTION_HEADER[=ON/OFF];        : ON
 *  set OPTION_BOTTOM[=ON/OFF];        : ON
 *  set OPTION_ALIGNMENT[=ON/OFF];     : ON
 *  set OPTION_FRAME[=ON/OFF];         : OFF
 *  set OPTION_HTML[=ON/OFF];          : OFF 
 *  set OPTION_EXPANDED[=ON/OFF];      : OFF 
 *  set OPTION_SEPARATOR='+';          : '|'
 *  set OPTION_NULLSTRING='NULL';      : ''
 *  set OPTION_ZEROSTRING='-0-';       : ''
 *  set OPTION_CAPTION='*TITLE*';      : ''
 *  set OPTION_TABLETAG="BGCOLOR=#FF0000 SIZE=4 BORDER=4"; 
 *                 or  ="<TABLE BGCOLOR=#FF0000 SIZE=4 BORDER=4>"; 
 *  set OPTION_HEADERTR="<TR ..>"; 
 *  set OPTION_HEADERTH="<TH ..>,<TH>,<TH>,.."; 
 *  set OPTION_BODYTAG="<TD ..>,<TD>,<TD>,.."; 
 *  set OPTION_INPUTTAG[=ON/OFF];      : OFF 
 *
 *
 *   return( 0) : normal end
 *   return(-1) : not "set EXEC_SQL_OPTION .." statement.
 *   return(-2) : syntax error
 ******************************************************************/
{
	int	option_no = -1;
	int 	ival;
	char    bval[10];
	char	buf[256];
	int	flag;
	int	len;
	int	i;

	char    *p, *p2, *p3;

	/*------------ check SQL statement --------------*/
	if (value == NULL)
		return (-1);

	/*------------ search  "set" --------------------*/
	p = value;
	p = skipSpace(p);
	p2 = skipToken(p);

	if (p2 - p != 3 || strUcmp(p, "SET", 3) != 0)
		return (-1);

	/*------- search  "EXEC_SQL_OPTION" ------------*/
	p3 = strchr(p2, '\0');
	while(*(--p3)==' ') *p3 = '\0';
	if( *p3 == ';' )
		*p3 = '\0';
	while(*(--p3)==' ') *p3 = '\0';

	p = p2;
	p = skipSpace(p);
	p2 = skipToken(p);
	if( (p3=strchr(p,'=')) != NULL)
	{
		if( p3 < p2 )
			p2 = p3;
	}

	if(p2-p == 15 && strUcmp(p,"EXEC_SQL_OPTION",15)==0)
	{
		p= skipSpace(p2);
		p2=skipToken(p);

		/*----------------- DEFAULT --------------------*/
		if ((p2-p==7 && strUcmp(p,"DEFAULT",7 )==0) ||
		    (p2-p==8 && strUcmp(p,"DEFAULT;",8)==0) ) 
		{
			memset(&exec_sql_option, 0, sizeof(EXEC_SQL_OPTION));
			exec_sql_option.header = 1;
			exec_sql_option.bottom = 1;
			exec_sql_option.alignment = 1;
			i = 0;
			while(exec_sql_option_default[i].name != NULL) 
			{ 
				bind_variable(exec_sql_option_default[i].name, exec_sql_option_default[i].value, 0);
				i++;
			} 
			return(0);
		}
		/*----------------- CGI ------------------------*/
		else if ((p2-p==3 && strUcmp(p,"CGI",3 )==0) ||
		         (p2-p==4 && strUcmp(p,"CGI;",4)==0) )
		{
			exec_sql_option.cgi_mode = 1;
			exec_sql_option.html = 1;
			bind_variable("OPTION_HTML", "ON", 0);
			/******/
			uncgi();  /* set DATA received from WWW */
			/******/

			return(0);
		}
		else
		{
			setSQLCA(pset->sqlca, SQL_INVALID_STMT,
				getSQLerrmc("Option name '%s' failed.", p), 0, 0);
			return(-2);
		}
	}

	/*------- search  "OPTION_xxxxxx" -------------*/
	i = 0;
	while(exec_sql_option_default[i].name != NULL)
	{
		len = strlen(exec_sql_option_default[i].name);
		if ( p2-p==len &&
			strUcmp(p,exec_sql_option_default[i].name,len)==0)
		{
			option_no = i;
			break;
		}
		i++;
	}

	if (option_no == -1)
		return(-1);

	/*------- set last string into '\0'-------------*/
	if( *p2 != '\0' )
	{
		/*------- search  "=" ------------------*/
		if ( *p2 == '=' )
		{
			p2++;
		}
		else if ( *p2 == ' ' )
		{
			p = skipSpace(p2);
			if ( *p != '=' )
			{
				setSQLCA(pset->sqlca, SQL_INVALID_STMT,
					getSQLerrmc("Value '%s' failed.", p), 0, 0);
				return (-2);
			}
			if (*p == '=' )
				p2 = p+1;
		}
		else
		{
			setSQLCA(pset->sqlca, SQL_INVALID_STMT,
				getSQLerrmc("Value '%s' failed.", p), 0, 0);
			return(-2);
		}
		flag=1;

		/*-------- serch singl quote -----------*/
		p = p2;
		p = skipSpace(p);
		if (*p=='\'')
			p++;
		p2 = strchr(p, '\0');
		if (*(--p2)=='\'' ) 
			*p2 = '\0';
		else
			++p2;
	}
	else 
		flag = 0;
	
	/*------------ check & setting of value ------------*/
	ival = -1;

	if( flag==0 || *p == '\0' || 
		strUcmp(p,"TRUE",4)==0 || strUcmp(p,"ON",2)==0)
	{
		ival = 1;
		strcpy(bval, "ON");
	}
	else if( strUcmp(p,"FALSE",5)==0 || strUcmp(p,"OFF",3)==0)
	{
		ival = 0;
		strcpy(bval, "OFF");
	}

	buf[0] = '\0';
	if(flag == 1)
		strncpy(buf, p, 255);

	/*==================================================
	 *   setting of EXEC_SQL_OPTION variables.
	 *==================================================
	 */
	switch(exec_sql_option_default[option_no].code)
	{
	case N_ECHO: 
		if(ival == -1) goto err_return;
		exec_sql_option.echo_mode = ival;
		bind_variable(exec_sql_option_default[option_no].name, bval, 0);
		break;

	case N_QUIET: 
		if(ival == -1) goto err_return;
		exec_sql_option.quiet_mode = ival;
		bind_variable(exec_sql_option_default[option_no].name, bval, 0);
		break;

	case N_HEADER: 
		if(ival == -1) goto err_return;
		exec_sql_option.header = ival;
		bind_variable(exec_sql_option_default[option_no].name, bval, 0);
		break;

	case N_BOTTOM: 
		if(ival == -1) goto err_return;
		exec_sql_option.bottom = ival;
		bind_variable(exec_sql_option_default[option_no].name, bval, 0);
		break;

	case N_ALIGNMENT: 
		if(ival == -1) goto err_return;
		exec_sql_option.alignment = ival;
		bind_variable(exec_sql_option_default[option_no].name, bval, 0);
		break;

	case N_FRAME: 
		if(ival == -1) goto err_return;
		exec_sql_option.outer_frame = ival;
		bind_variable(exec_sql_option_default[option_no].name, bval, 0);
		break;

	case N_HTML: 
		if(ival == -1) goto err_return;
		exec_sql_option.html = ival;
		bind_variable(exec_sql_option_default[option_no].name, bval, 0);
		break;

	case N_EXPANDED: 
		if(ival == -1) goto err_return;
		exec_sql_option.expanded = ival;
		bind_variable(exec_sql_option_default[option_no].name, bval, 0);
		break;


	case N_SEPARATOR: 
		strncpy(exec_sql_option.separator, buf, SEPARATOR_LEN-1);
		bind_variable(exec_sql_option_default[option_no].name, exec_sql_option.separator, 0);
		break;

	case N_NULLSTRING:
		strncpy(exec_sql_option.null_string, buf, NULLSTRING_LEN-1);
		bind_variable(exec_sql_option_default[option_no].name, exec_sql_option.null_string, 0);
		break;

	case N_ZEROSTRING: 
		strncpy(exec_sql_option.zero_string, buf, ZEROSTRING_LEN-1);
		bind_variable(exec_sql_option_default[option_no].name, exec_sql_option.zero_string, 0);
		break;

	case N_CAPTION:
		strncpy(exec_sql_option.caption, buf, CAPTION_LEN-1);
		bind_variable(exec_sql_option_default[option_no].name, exec_sql_option.caption, 0);
		break;

	case N_TABLETAG:
		strncpy(exec_sql_option.table_tag, buf, TABLETAG_LEN-1);
		bind_variable(exec_sql_option_default[option_no].name, exec_sql_option.table_tag, 0);
		break;

	case N_HEADERTR:
		strncpy(exec_sql_option.header_tr, buf, HEADERTR_LEN-1);
		bind_variable(exec_sql_option_default[option_no].name, exec_sql_option.header_tr, 0);
		break;

	case N_HEADERTH:
		strncpy(exec_sql_option.header_th, buf, HEADERTH_LEN-1);
		bind_variable(exec_sql_option_default[option_no].name, exec_sql_option.header_th, 0);
		break;

	case N_BODYTAG:
		strncpy(exec_sql_option.body_tag, buf, BODYTAG_LEN-1);
		bind_variable(exec_sql_option_default[option_no].name, exec_sql_option.body_tag, 0);
		break;

	case N_INPUTTAG: 
		if(ival == -1) goto err_return;
		exec_sql_option.input_tag = ival;
		bind_variable(exec_sql_option_default[option_no].name, bval, 0);
		break;

	case N_INPUTSIZE:
		strncpy(exec_sql_option.input_size, buf, INPUTSIZE_LEN-1);
		bind_variable(exec_sql_option_default[option_no].name, exec_sql_option.input_size, 0);
		break;

	default:
		goto err_return;
		break;
	}

	return (0);

err_return:
	setSQLCA(pset->sqlca, SQL_INVALID_STMT,
		getSQLerrmc("Value '%s' failed.", p), 0, 0);
	return(-2);
}


int 
set_exec_sql_option_default(EXEC_SQL_OPTION *exec_sql_option)
{
	memset(exec_sql_option, 0, sizeof(EXEC_SQL_OPTION));
	exec_sql_option->header = 1;
	exec_sql_option->bottom = 1;
	exec_sql_option->alignment = 1;
	return(0);
}



int
set_client_encoding(PSconnSet * pset, char *value)
/*******************************************************************
 * parse SET CLIENT_ENCODING {TO | = } 'values' | DEFAULTS
 *
 *  ex.
 *   input : set client_encoding to 'SJIS';
 *
 *   return( 0) : normal end
 *   return(-1) : syntax error
 *   return(-2) : max string size over
 ******************************************************************/
{
	char           *p, *p2, *p3;
	char		encode[50];
	int		encoding = SQL_ASCII;
	char	       *pw;

	/*------------ check SQL statement --------------*/
	if (value == NULL)
		return (-1);

	/*------------ search  "set" --------------------*/
	p = value;
	p = skipSpace(p);
	p2 = skipToken(p);

	if (p2 - p != 3 || strUcmp(p, "SET", 3) != 0)
		return (-1);

	/*------- search  "CLIENT_ENCODING" -------------*/
	p = p2;
	p = skipSpace(p);
	p2 = skipToken(p);
	p3 = strchr(p,'='); if (p3 && (p3 < p2)) p2 = p3;

	if (p2 - p != 15 || strUcmp(p, "CLIENT_ENCODING", 15) != 0)
		return (-1);

	/*------- search  "TO" or "=" -------------------*/
	p = p2;
	p = skipSpace(p);
	if ( *p == '=' ) 
		p2 = p+1;
	else
		p2 = skipToken(p);

	if (!( (p2 - p == 1 && strncmp(p, "=" , 1) == 0) ||
	       (p2 - p == 2 && strUcmp(p, "TO", 2) == 0) )  )
		return (-1);

	/*-------- get CODE ('SJIS' or DEFAULT ) --------*/
	p = p2;
	p = skipSpace(p);

	if (strlen(p) > PS_ENCODING_CHAR_LEN+2)
		return (-2);

	if( *p == '\'' )
		strncpy( encode, p+1, PS_ENCODING_CHAR_LEN); 
	else
		strncpy( encode, p  , PS_ENCODING_CHAR_LEN); 

	encode[PS_ENCODING_CHAR_LEN] = '\0';

	pw = strchr( encode, '\'' ); if( pw != NULL ) *pw = '\0';

	/*--------- get encoding value ------------------*/
	if( strcmp( encode, "DEFAULT" ) != 0 )
	{
#ifdef MULTIBYTE
	        encoding = pg_char_to_encoding(encode);
#else
	        encoding = SQL_ASCII;
#endif
	}

	/*--------- set encoding to DB pointer ----------*/ 
	pset->dbset->encoding = encoding;
	strncpy(pset->dbset->encodingchar, encode, PS_ENCODING_CHAR_LEN);
	pset->dbset->encodingchar[PS_ENCODING_CHAR_LEN] = '\0';

	return (0);
}

