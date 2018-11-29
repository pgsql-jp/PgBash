/*----------------------------------------------------------------
 * sql_set.h
 *
 * Change logs
 * 2001.04.15: Change not to use set_client_encoding().
 * 2001.04.15: Add EXEC_SQL_OPTION struct.
 * 2001.06.11: Add OPTION_HEADERTR/BODYTAG.
 * 2001.06.11: Add OPTION_INPUTTAG/OPTION_INPUTSIZE.
 *
 *-----------------------------------------------------------------
 */

#ifndef SQL_SET_HEADER
#define SQL_SET_HEADER

#include "exec_sql.h"


/*-------------------------------------------------------------------
	Typedef of EXEC_SQL_OPTION_DEFAULT 
-------------------------------------------------------------------*/
typedef struct _exec_sql_option_default
{
	char    *name;	/* option name */
	char    *value;  /* option value */
	int     code;   /* option code */

} EXEC_SQL_OPTION_DEFAULT;

/*-------------------------------------------------------------------
	Typedef of 'set EXEC_SQL_OPTION' command options.
-------------------------------------------------------------------*/
#define CAPTION_LEN		256
#define TABLETAG_LEN		256
#define HEADERTR_LEN		256
#define HEADERTH_LEN		1024
#define BODYTAG_LEN		1024
#define INPUTSIZE_LEN           256

#define SEPARATOR_LEN		10
#define NULLSTRING_LEN		10
#define ZEROSTRING_LEN		10

typedef struct _EXEC_SQL_OPTION
{
	int cgi_mode;           /* 0:OFF,   1:ON */
	int echo_mode;          /* 0:OFF,   1:ON */
	int quiet_mode;         /* 0:OFF,   1:ON */
	int alignment;          /* 0:OFF,   1:ON */
	int header;             /* 0:OFF,   1:ON */
	int bottom;             /* 0:OFF,   1:ON */
	int outer_frame;        /* 0:OFF,   1:ON */
	int html;               /* 0:OFF,   1:ON */
	int expanded;           /* 0:OFF,   1:ON */

	char separator[SEPARATOR_LEN];		/* Default: '|' */
	char null_string[NULLSTRING_LEN];	/* Default: '' */
	char zero_string[ZEROSTRING_LEN];	/* Default: '' */

	char caption[CAPTION_LEN];      /* table caption */
	char table_tag[TABLETAG_LEN];   /* HTML table tag string */
	char header_tr[HEADERTR_LEN];   /* HTML header <TR> tag string */
	char header_th[HEADERTH_LEN];   /* HTML header <TH> tag string */
	char body_tag[BODYTAG_LEN];	    /* HTML body <TD> tag string */
	int input_tag;                  /* 0:OFF,   1:ON */
	char input_size[INPUTSIZE_LEN]; /* INPUT tag SIZE */

} EXEC_SQL_OPTION;

/*-----------------------------------------------------------------
 *   			Function List
 *---------------------------------------------------------------*/
extern	int set_exec_sql_options(PSconnSet *pset, char *value);
extern	int set_exec_sql_option_default(EXEC_SQL_OPTION *exec_sql_option);
extern	EXEC_SQL_OPTION  *get_exec_sql_options();
extern  EXEC_SQL_OPTION_DEFAULT exec_sql_option_default[];

extern	int	set_client_encoding(PSconnSet *pset, char *value);

/*-----------------------------------------------------------------
 *   		Global EXEC_SQL_OPTION variable
 *---------------------------------------------------------------*/
extern	EXEC_SQL_OPTION		exec_sql_option;


#endif
