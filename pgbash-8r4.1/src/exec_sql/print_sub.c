/*-------------------------------------------------------------------------
 * print_sub.c
 *	  functions for pretty-printing query results
 *
 * Change logs
 * 2001.06.11: Renew this program source.
 *  (1) Add "&nbsp;" into the data part of output_row when the 
 *      value is NULL/ALL_BIT_OFF.
 *  (2) Delete "--- RECORD n ---" of the HTML TABLE in the case of 
 *      "set OPTION_HEADER=FALSE;"
 *  (3) Fix the bug that tel_no(e.g. 06-6390-9999) is a numeric type 
 *  (4) Change "<th align=xxx>" to "<th>" in the header part.
 *  (4) Add the function for parsing and setting the table tag.
 * 2001.11.23: delete blank at the last of string.
 * 2001.11.25: Modify expanded type line space.
 * 2002.02.10: Modify PSmblen(). (encoding==0||encoding==SQL_ASCII)
 * 2002.10.28: Add replace_string_noalign().
 * 2011.08.02: Delete '\n' at the cation print.
 *
 *
 *-------------------------------------------------------------------------
 */
#include <stdlib.h>
#include <string.h>

#include "libpq-fe.h"
#include "print_sub.h"
#include "sql_set.h"
#include "utils.h"

#define MAX_TABLE_TAG	256
typedef struct _TableTag
{
	char *table_tag;		/* <TABLE ..> tag */
	char *header_tr; 		/* <TR ..> all header tag */
	int  nheader_th; 		/* no of column of <TH ..> header tag */
	char *header_th[MAX_TABLE_TAG];	/* column value of <TH ..> header tag */
	int  nbody_tag; 		/* no of column of <TD ..> body tag */
	char *body_tag[MAX_TABLE_TAG];	/* column value of <TD ..> body tag */
	int  ninput_size;
	int  input_size[MAX_TABLE_TAG];
} TableTag;

static char	*HTML_SPACE_COL ="&nbsp;";


/*------------------------------------------------------------------------
 *	Functions list
 *-----------------------------------------------------------------------*/
/*
 * int alloc_printOpt(PSprintOpt *Opt, PGresult *res); 
 * int set_printOpt(PSprintOpt *Opt); 
 * void PSprint(FILE * fout, PSprintOpt * po, int encoding);
 * void free_printOpt(PSprintOpt *Opt); 
 */
static void expanded_html_print(FILE * fout, PSprintOpt * po);
static void expanded_plain_print(FILE * fout, PSprintOpt * po);
static void html_print(FILE * fout, PSprintOpt * po);
static void aligned_plain_print(FILE * fout, PSprintOpt * po, int encoding);
static void noaligned_plain_print(FILE * fout, PSprintOpt * po, int encoding);
static char *do_header(FILE * fout, PSprintOpt * po, short *fieldMax,
	  char **fieldNames, unsigned char *fieldNotNum, const int fs_len);
static char *del_last_blank(char *str);
static short check_fieldNotNum(char *value);
static int  replace_string(char *pval, char *ppval, char *fieldSep, 
							int encoding);
static int  replace_string_noalign(char *pval, char *ppval, char *fieldSep, 
							int encoding);

static void set_table_tag(char *opt, char **table_tag);
static void set_header_tr(char *opt, char **header_tr);
static void set_header_th(char *opt, int *ntag, char **tag);
static void set_body_tags(char *opt, int *ntag, char **tag);
static void set_input_size(char *opt, int *ntag, int *tag);

static int PSmblen(const unsigned char *s, int encoding);



/*#####################################################################
 *    Global print routines.
 *#####################################################################
 */

void
PSprint(FILE * fout, PSprintOpt * po, int encoding)
/*************************************************************************
 * output results of query.
 *
 * First,  call    alloc_printOpt() or set_printOpt().
 * Second, call    this PSprint().
 * Last,   call    free_printOpt().
 ************************************************************************/
{
        if (po->expanded)
	{
	 	/*-------- expanded format mode -------*/
		if (po->html3)
			expanded_html_print(fout, po);
		else
			expanded_plain_print(fout, po);
	}
	else
	{
	 	/*-------- HTML TABLE mode ------------*/
		if (po->html3)
		{
			html_print(fout, po);
		}
	 	/*-------- text TABLE mode ------------*/
		else
		{
			if( po->align)
				aligned_plain_print(fout, po, encoding);
			else
				noaligned_plain_print(fout, po, encoding);
		}
	}
}


int
alloc_printOpt(PSprintOpt * po, PGresult * res)
/*************************************************************************
 * set results to print option.
 ************************************************************************/
{
	int             i, j;
	char		*p;

	po->nFields = PQnfields(res);
	po->nTups   = PQntuples(res);

	po->fieldName  = (char **) wrapCalloc(po->nFields, sizeof(char *));
	po->fieldValue = (char **) wrapCalloc(po->nFields * (po->nTups + 1), sizeof(char *));
	po->fieldMax   = (short *) wrapCalloc(po->nFields, sizeof(short *));
	po->fieldNotNum= (unsigned char *) wrapCalloc(po->nFields, sizeof(unsigned char *));

	/*---------------- set Field Name ---------------------*/
	for (j = 0; j < po->nFields; j++)
	{
		po->fieldName[j]   = PQfname(res, j);
		po->fieldNotNum[j] = 255;

		if (!po->expanded && po->align && !po->html3)
		{
			p = del_last_blank(PQfname(res, j));
			po->fieldMax[j] = strlen(p);
		}
	}

	/*----------- set Value, NULL or ZERO string ----------*/
	for (i = 0; i < po->nTups; i++)
	{
		for (j = 0; j < po->nFields; j++)
		{
			if (PQgetisnull(res, i, j) == 0)
			{
				if (PQgetlength(res, i, j) >= 1)
				{
					po->fieldValue[i*po->nFields+j] = PQgetvalue(res, i, j);
					if (!po->expanded && po->align)
					{
						/*-- field length --*/
						if (!po->html3)
						{
							p = del_last_blank(po->fieldValue[i*po->nFields+j]);
							if ( po->fieldMax[j] < strlen(p) )
								po->fieldMax[j] = strlen(p);
						}

						/*-- field number --*/
						if (po->fieldNotNum[j] == 255)
							po->fieldNotNum[j] = check_fieldNotNum(PQgetvalue(res, i,j));
					}
				}
				else
					po->fieldValue[i*po->nFields+j] = po->zerostr;
			}
			else
				po->fieldValue[i * po->nFields + j] = po->nullstr;
		}
	}

	return (0);
}

int
set_printOpt(PSprintOpt * po)
/*************************************************************************
 * set results to print option.
 *
 * INPUT DATA:
 *	po->nFields
 *	po->nTups  
 *	po->fieldName 
 *	po->fieldValue 
 ************************************************************************/
{
	char		**fieldValue = po->fieldValue;
	int		nFields = po->nFields;
	int		nTups = po->nTups;

	char		*pval;
	int             i, j;

	/*--------------- malloc data ------------------------*/
	po->fieldMax   = (short *) wrapCalloc(po->nFields, sizeof(short *));
	po->fieldNotNum= (unsigned char *) wrapCalloc(po->nFields, sizeof(unsigned char *));

	/*---------------- set Field Name ---------------------*/
	for (j = 0; j < po->nFields; j++)
	{
		po->fieldMax[j]    = strlen(po->fieldName[j]);
		po->fieldNotNum[j] = 255;
	}

	/*----------- set Value, NULL or ZERO string ----------*/
	for (i = 0; i < po->nTups; i++)
	{
		for (j = 0; j < po->nFields; j++)
		{
			pval = fieldValue[i*nFields+j];

			if (pval != NULL)
			{
				if (!po->expanded && po->align)
				{
					/*-- field length --*/
					if(!po->html3 && 
						po->fieldMax[j] < strlen(pval))
						po->fieldMax[j] = strlen(pval);

					/*-- field number --*/
					if (!po->expanded && po->align && 
						po->fieldNotNum[j] == 255)
						po->fieldNotNum[j] = check_fieldNotNum(pval);
				}
			}
		}
	}

	return (0);
}

void
free_printOpt(PSprintOpt * po)
{
	if (po->fieldName != NULL)
		free(po->fieldName);
	if (po->fieldValue != NULL)
		free(po->fieldValue);
	if (po->fieldMax != NULL)
		free(po->fieldMax);
}


/*#####################################################################
 *    Local print routines.
 *#####################################################################
 */

static void
expanded_html_print(FILE * fout, PSprintOpt * po)
/*************************************************************************
 * expanded HTML output mode.
 ************************************************************************/
{
	TableTag	tableTag;

	int		nFields = po->nFields;
	int		nTups = po->nTups;
	char		**fieldNames = po->fieldName;

	char		*pval, *pp2;
	int		i, j;

	/*------------- check tuples and field -------------*/
	if (nFields <= 0 || nTups <= 0)
		return;

	/*------------- set PSprintOpt.tableTag ------------*/
	set_table_tag(po->table_tag, &tableTag.table_tag);
	set_header_tr(po->header_tr, &tableTag.header_tr);
	set_header_th(po->header_th, &tableTag.nheader_th, tableTag.header_th);
	set_body_tags(po->body_tag, &tableTag.nbody_tag, tableTag.body_tag);
	set_input_size(po->input_size, &tableTag.ninput_size, tableTag.input_size);

	/*------------------ print CAPTION -------------------*/
	if (po->caption)
		fprintf(fout,"%s<BR>\n", po->caption);

	/*------------------ print HTML table ----------------*/
	for (i = 0; i < nTups; i++)
	{
		/*-------------- HEADER --------------*/
		if (po->header)
			fprintf(fout,
				"%s<caption align=high>-- RECORD %d --</caption>\n",
				tableTag.table_tag, i);
		else
			fprintf(fout, "%s\n", tableTag.table_tag);

		/*-------------- BODY ----------------*/
		for (j = 0; j < nFields; j++)
		{
			/*-------- get Values ---------*/
			pval = po->fieldValue[i * nFields + j];
			
			/*--------- SPACE value --------*/
			if (!po->input_tag ||
				(po->input_tag && tableTag.input_size[j]==-1))
			{
				if( pval == NULL || *pval == '\0' ) 
					pval = HTML_SPACE_COL;
				else
				if( *pval == ' ' )
				{
					pp2 = pval+1;
					while( *pp2 == ' ' ) pp2++;
					if( *pp2 == '\0' ) 
						pval = HTML_SPACE_COL;
				}
			}

			/*----------- print body ------------*/
			if (po->input_tag)
			{
				if (tableTag.ninput_size > j && 
					tableTag.input_size[j] > 0)
					fprintf(fout, "<tr><td><b>%s</b></td><td><INPUT TYPE=text NAME='%s' VALUE='%s' SIZE=%d></td></tr>\n", fieldNames[j],  fieldNames[j],pval, tableTag.input_size[j]);
				else if (tableTag.ninput_size > j && 
					tableTag.input_size[j] == -1)
					fprintf(fout, "<tr><td><b>%s</b></td><td>%s</td></tr>\n", fieldNames[j], pval);
				else
					fprintf(fout, "<tr><td><b>%s</b></td><td><INPUT TYPE=text NAME='%s' VALUE='%s' SIZE=%d></td></tr>\n", fieldNames[j],  fieldNames[j],pval, strlen(pval)+10);
			}
			else
				fprintf(fout, "<tr><td><b>%s</b></td><td>%s</td></tr>\n", fieldNames[j], pval);

		}
		/*------------- End of Table ------------*/
		fputs("</table>\n", fout);
	}

	/*------------------ print ROW COUNT -----------------*/
	if (po->rowcount)
		fprintf(fout, "(<B>%d</B> row%s)<BR>\n", nTups, (nTups == 1) ? "" : "s");
}


static void
expanded_plain_print(FILE * fout, PSprintOpt * po)
/*************************************************************************
 * expanded text output mdeo.
 ************************************************************************/
{
	int             nFields = po->nFields;
	int             nTups = po->nTups;
	char          **fieldNames = po->fieldName;
	int             fs_len = strlen(po->fieldSep);
	int             len;
	int             fieldMaxLen = 0;

	char		*pval;
	int             i, j;

	/*------------- check tuples and field -------------*/
	if (nFields <= 0 || nTups <= 0)
		return;

	/*------------- Max. of fieldName length -----------*/
	for (j = 0; j < nFields; j++)
	{
		len = strlen(fieldNames[j]);
		if (len > fieldMaxLen)
			fieldMaxLen = len;
	}

	/*------------------ print CAPTION -------------------*/
	if (po->caption)
		fprintf(fout, "%s\n", po->caption);

	/*------------------ print table ---------------------*/
	for (i = 0; i < nTups; i++)
	{
		/*-------------- HEADER --------------*/
		if (po->header)
			fprintf(fout, "-- RECORD %d --\n", i);
		else
		{
			if (i == 0 || nFields > 1)
				fprintf(fout, "\n");
		}

		/*-------------- BODY ----------------*/
		for (j = 0; j < nFields; j++)
		{
			/*------ get Values ---------*/
			pval = po->fieldValue[i * nFields + j];

			/*----- print BODY ----------*/
			if (po->align)
				fprintf(fout, "%-*s%s %s\n",
					fieldMaxLen, fieldNames[j], po->fieldSep, pval);
			else
				fprintf(fout, "%s%s%s\n", fieldNames[j], po->fieldSep, pval);

		}
	}

	/*------------ print row count --------------------*/
	if (po->rowcount)
		fprintf(fout, "(%d row%s)\n", nTups, (nTups == 1) ? "" : "s");
}


static void
html_print(FILE * fout, PSprintOpt * po)
/*************************************************************************
 * Format results of a query for printing.
 ************************************************************************/
{
	TableTag	tableTag;

	int             nFields = po->nFields;
	int             nTups = po->nTups;
	char          **fieldNames = po->fieldName;
	unsigned char  *fieldNotNum = po->fieldNotNum;

	int             i, j;
	char		*pval, *pp2;

	/*------------- check tuples and field -------------*/
	if (nFields <= 0 || nTups <= 0)
		return;

	/*------------- set PSprintOpt.tableTag ------------*/
	set_table_tag(po->table_tag, &tableTag.table_tag);
	set_header_tr(po->header_tr, &tableTag.header_tr);
	set_header_th(po->header_th, &tableTag.nheader_th, tableTag.header_th);
	set_body_tags(po->body_tag, &tableTag.nbody_tag, tableTag.body_tag);
	set_input_size(po->input_size, &tableTag.ninput_size, tableTag.input_size);

	/*----------- print TABLE tag and CAPTION ----------*/
	if (po->caption)
		fprintf(fout, "%s<caption align=high>%s</caption>\n",
			tableTag.table_tag, po->caption);
	else
		fprintf(fout, "%s\n", tableTag.table_tag);

	/*------------------- HEADER -----------------------*/
	if (po->header)
	{
		fputs( tableTag.header_tr, fout ); /* <tr> */

		for (j = 0; j < nFields; j++)
		{
			if (tableTag.nheader_th > j)
				fprintf(fout, "%s%s</th>", tableTag.header_th[j], 
					fieldNames[j]);
			else
				fprintf(fout, "<th>%s</th>", fieldNames[j]);
		}

		fputs("</tr>\n", fout);
	} 

	/*------------------- BODY -------------------------*/
	for (i = 0; i < nTups; i++)
	{
		fputs("<tr>", fout);  

		for (j = 0; j < nFields; j++)
		{
			pval = po->fieldValue[i*nFields+j];

			/*----- fill space -----*/
			if (!po->input_tag || 
				(po->input_tag && tableTag.input_size[j]==-1))
			{
				if( pval == NULL || *pval == '\0' ) 
					pval = HTML_SPACE_COL;
				else
				if( *pval == ' ' )
				{
					pp2 = pval+1;
					while( *pp2 == ' ' ) pp2++;
					if( *pp2 == '\0' ) 
						pval = HTML_SPACE_COL;
				}
			}

			/*----- print BODY ------*/
			if (tableTag.nbody_tag > j)  /* <td align=..>*/
			{
				if (po->input_tag)
				{
					if (tableTag.ninput_size > j && 
						tableTag.input_size[j] > 0)
						fprintf(fout, "%s align=%s><INPUT TYPE=text NAME='%s' VALUE='%s' SIZE=%d></td>", 
						tableTag.body_tag[j],
						fieldNotNum[j] ? "left" : "right",
						fieldNames[j],pval,tableTag.input_size[j]);
					else if (tableTag.ninput_size > j && 
						tableTag.input_size[j] == -1)
						fprintf(fout, "%s align=%s>%s</td>", 
						tableTag.body_tag[j],
						fieldNotNum[j] ? "left" : "right", pval);
					else
						fprintf(fout, "%s align=%s><INPUT TYPE=text NAME='%s' VALUE='%s' SIZE=%d></td>", 
						tableTag.body_tag[j],
						fieldNotNum[j] ? "left" : "right",
						fieldNames[j],pval,strlen(pval)+10);
				}
				else
					fprintf(fout, "%s align=%s>%s</td>", 
						tableTag.body_tag[j],
						fieldNotNum[j] ? "left" : "right", pval);
			}
			else
			{
				if (po->input_tag)
				{
					if (tableTag.ninput_size > j && 
						tableTag.input_size[j] > 0)
						fprintf(fout, "<td align=%s><INPUT TYPE=text NAME='%s' VALUE='%s' SIZE=%d></td>",
						fieldNotNum[j] ? "left" : "right",
						fieldNames[j],pval, tableTag.input_size[j]);
					else if (tableTag.ninput_size > j && 
						tableTag.input_size[j] == -1)
						fprintf(fout, "<td align=%s>%s</td>",
						fieldNotNum[j] ? "left" : "right", pval);
					else
						fprintf(fout, "<td align=%s><INPUT TYPE=text NAME='%s' VALUE='%s' SIZE=%d></td>",
						fieldNotNum[j] ? "left" : "right",
						fieldNames[j],pval, strlen(pval)+10);
				}
				else
					fprintf(fout, "<td align=%s>%s</td>",
						fieldNotNum[j] ? "left" : "right", pval);
			}

		}
		fputs("</tr>\n", fout);  
	}

	/*------------------- TABLE end --------------------*/
	fputs("</table>\n", fout);

	/*------------ print row count --------------------*/
	if (po->rowcount)
		fprintf(fout, "(<B>%d</B> row%s)<BR>\n", nTups, (nTups == 1) ? "" : "s");
}

static void
noaligned_plain_print(FILE * fout, PSprintOpt * po, int encoding)
/*************************************************************************
 * normal text print mode.
 ************************************************************************/
{
	int             nFields = po->nFields;
	int             nTups = po->nTups;
	char          **fieldNames = po->fieldName;
	int             fs_len = strlen(po->fieldSep);

	char		*pval, *ppval;
	int             i, j;

	/*------------- check tuples and field -------------*/
	if (nFields <= 0 || nTups <= 0)
		return;

	/*---------------- print CAPTION -------------------*/
	if (po->caption)
	{
		fprintf(fout, "%s\n", po->caption);
		/*if (!po->outerframe)
			fprintf(fout, "\n");*/
	}

	/*---------------- HEADER --------------------------*/
	if (po->header)
	{
		for (j = 0; j < nFields; j++)
		{
			fprintf(fout, "%s", fieldNames[j]);
			if (j < nFields-1)
				fprintf(fout, "%s", po->fieldSep);
		}
		fputc('\n', fout);
	}

	/*---------------- BODY ----------------------------*/
	for (i = 0; i < nTups; i++)
	{
		for (j = 0; j < nFields; j++)
		{
			pval  = po->fieldValue[i*nFields+j];
			ppval = (char *)wrapMalloc(strlen(pval)+256);

			/*---- insert Backslash char. ------*/
			replace_string_noalign(pval, ppval, po->fieldSep, encoding);

			/*---------- print BODY ------------*/
			fprintf(fout, "%s", ppval);

			if (j < nFields-1)
				fputs(po->fieldSep, fout);

			free(ppval);

		}
		fputc('\n', fout);
	}

	/*------------ print row count --------------------*/
	if (po->rowcount)
		fprintf(fout, "(%d row%s)\n", nTups, (nTups == 1) ? "" : "s");
}

static void
aligned_plain_print(FILE * fout, PSprintOpt * po, int encoding)
/*************************************************************************
 * normal text print mode.
 ************************************************************************/
{
	int             nFields = po->nFields;
	int             nTups = po->nTups;
	char          **fieldNames = po->fieldName;
	unsigned char	*fieldNotNum = po->fieldNotNum;
	short		*fieldMax = po->fieldMax;
	int             fs_len = strlen(po->fieldSep);

	int		len;
	char		*p;
	char		*pval, *ppval;
	char		*border = NULL;
	int		nrep;
	int             i, j, k;

	/*------------- check tuples and field -------------*/
	if (nFields <= 0 || nTups <= 0)
		return;

	/*---------------- print CAPTION -------------------*/
	if (po->caption)
	{
		fprintf(fout, "%s\n", po->caption);
		/*if (!po->outerframe)
			fprintf(fout, "\n");*/
	}

	/*---------------- HEADER --------------------------*/
	if (po->header || po->outerframe)
		border = do_header(fout, po, 
			fieldMax, fieldNames, fieldNotNum, fs_len);

	/*---------------- BODY ----------------------------*/
	for (i = 0; i < nTups; i++)
	{

		if (po->outerframe)
			fputs(po->fieldSep, fout);

		for (j = 0; j < nFields; j++)
		{
			pval =  po->fieldValue[i*nFields+j];
			ppval = (char *)wrapMalloc(strlen(pval)+2048);

			/*-------- replace string ----------*/
			nrep = replace_string(pval, ppval, "", encoding);

			/*-------- print BODY --------------*/
			if (j < nFields-1 || po->outerframe) 
				fprintf(fout, fieldNotNum[j] ?  " %-*s " : " %*s ",
					fieldMax[j]+nrep, ppval);
			else
			{
				if (fieldNotNum[j])
					fprintf(fout, " %s ", ppval);
				else
					fprintf(fout, " %*s ",fieldMax[j]+nrep, ppval);
			}

			if (po->outerframe || j < nFields-1)
				fputs(po->fieldSep, fout);

			free(ppval);					
		}
		fputc('\n', fout);
	}

	/*---------------- print BAR(+--+) -----------------*/
	if (po->outerframe)
		fprintf(fout, "%s\n", border);

	if (border)
		free(border);

	/*------------ print row count --------------------*/
	if (po->rowcount)
		fprintf(fout, "(%d row%s)\n", nTups, (nTups == 1) ? "" : "s");
}

static char    *
do_header(FILE * fout, PSprintOpt * po, short *fieldMax,
	  char **fieldNames, unsigned char *fieldNotNum, const int fs_len)
/*************************************************************************
 *     normal text type header print
 ************************************************************************/
{
	int             nFields = po->nFields;
	int             j;
	int             tot = 0;
	int             n = 0;
	char           *p = NULL;
	char           *border = NULL;

	/*----- total fields length -----*/
	for (; n < nFields; n++)
		tot += fieldMax[n] + fs_len + 2 ;

	tot += fs_len * 2 + 2;

	/*----- make  BAR(+--+) ---------*/
	border = wrapMalloc(tot + 10);

	p = border;
	if (po->outerframe)
	{
		char           *fs = po->fieldSep;

		while (*fs++)
			*p++ = '+';
	}

	for (j = 0; j < nFields; j++)
	{
		int             len;

		for (len = fieldMax[j]+2; len--; *p++ = '-');

		if (po->outerframe || (j + 1) < nFields)
		{
			char           *fs = po->fieldSep;

			while (*fs++)
				*p++ = '+';
		}
	}
	*p = '\0';

	/*------------ print BAR(+---+) ---------*/
	if (po->header && po->outerframe)
		fprintf(fout, "%s\n", border);

	/*-------------- print field separator ----------*/
	if (po->header && po->outerframe)
		fputs(po->fieldSep, fout);

	/*-------------- print each field ---------------*/
	for (j = 0; j < nFields; j++)
	{
		char           *s = po->fieldName[j];	/* PQfname(res, j); */

		int             n = strlen(s);

		if (n > fieldMax[j])
			fieldMax[j] = n;

		if (po->header)
			fprintf(fout, fieldNotNum[j] ? " %-*s " : " %*s ", fieldMax[j], s);

		if (po->outerframe || (j + 1) < nFields)
			if (po->header)
				fputs(po->fieldSep, fout);
	}

	/*-------------- end BORDER line ----------------*/
	if (po->header)
		fprintf(fout, "\n");

	fprintf(fout, "%s\n", border);

	return border;
}

static char *
del_last_blank(char *str)
{
	int	len;
	char	*p;
	int	i;

	len = strlen(str);
	p   = str+(len-1);

	for(i=len-1; i>1; i--,p--)
	{
		if (*p == ' ')
			*p = '\0';
		else
			break;
	}
	return(str);
}


static short
check_fieldNotNum(char *value)
/*******************************************************************
 *  Field is Number or not.
 *
 *  Output:
 *  return 1 : Not Number
 *  return 0 : Number
 ******************************************************************/
{
	int	len = 0;
	char	*p;
	char	ch, ch2='\0';

	for (p = value; *p; p++)
	{
		ch = *p;

		if ( !((ch >= '0' && ch <= '9') ||
		      ch == '.' ||
		      ch == 'E' ||
		      ch == 'e' ||
		      ch == ' ' ||
		      ((ch2 == 0 || ch2 == 'E' || ch2 == 'e' ) && ch == '-') ))
		{
			return(1);
		}
		ch2 = ch;
	}

	if ( (*value == 'E' || *value == 'e' ||
	     !(ch >= '0' && ch <= '9')))
		return(1);

	return(0);
}

static int   
replace_string(char *pval, char *ppval, char *fieldSep, int encoding)
/*******************************************************************
 *  replace SEPARATOR string,'\n','\r' and '\t' to the suitable string.
 ******************************************************************/
{
	int	nrep = 0;
	char	*p, *pp2;
	int	len;

	for (p = pval, pp2=ppval; *p; 
		len = PSmblen(p, encoding), memcpy(pp2, p, len),
		pp2 += len, p += len)
	{
		switch(*p)
		{
		case '\n':
			*(pp2++) = '\\';
			*(pp2++) = 'n';
			p++;
			nrep++;
			break;
		case '\t':
			*(pp2++) = '\\';
			*(pp2++) = 't';
			p++;
			nrep++;
			break;
		case '\r':
			*(pp2++) = '\\';
			*(pp2++) = 'r';
			p++;
			nrep++;
			break;
		default:
			if (*p == *fieldSep)
			{
				*(pp2++) = '\\';
				nrep++;
			}
			break;
		}
	} 
	*pp2 = '\0';

	return(nrep);
}

static int   
replace_string_noalign(char *pval, char *ppval, char *fieldSep, int encoding)
/*******************************************************************
 *  replace SEPARATOR string,'\n','\r' and '\t' to the suitable string.
 ******************************************************************/
{
	int	nrep = 0;
	int	flag =0;
	char	*p, *pp2;
	char	*work;
	int	len;

	for (p = pval, pp2=ppval; *p; 
		len = PSmblen(p, encoding), memcpy(pp2, p, len),
		pp2 += len, p += len)
	{
		switch(*p)
		{
		case '\n':
			flag = 1;
			nrep++;
			break;
		case '\t':
			flag = 1;
			nrep++;
			break;
		case '\r':
			flag = 1;
			nrep++;
			break;
		case '"':
			flag = 1;
			*(pp2++) = '"';
			*(pp2++) = '"';
			p++;
			nrep++;
			break;
		default:
			if(*p == *fieldSep) 
			{
				flag = 1;
				nrep++;
			}
			break;
		}
	} 

	*pp2 = '\0';

	if (flag == 1)
	{
		work = (char *)malloc(strlen(ppval)+3);
		work[0] = '"';
		strcpy( work+1, ppval );

		len = strlen(work);
		work[len] = '"';
		work[len+1] = '\0';

		strcpy(ppval, work);

		free(work);
	}

	return(nrep);
}


/*#####################################################################
 *    HTML table tag routines.
 *#####################################################################
 */

static void 
set_table_tag(char *opt, char **table_tag)
/***********************************************************************
tableTag:
   *table:            <TABLE ..> tag 
***********************************************************************/
{
	static char TABLE[TABLETAG_LEN];
	static char *table_tag0 = "<table border=1>";

	char *p = opt;
	char *p2;

	/*---------- initial setteings --------*/
	*table_tag = table_tag0;

	if (opt == NULL || *opt == '\0')
		return;

	while (*p==' ') 
		p++;
	if (*p == '\0')
		return;
	
	/*----------- only table tag contents --------------*/
	if (*p != '<'  && strlen(p) < TABLETAG_LEN-18)
	{
		strcpy( TABLE, "<TABLE ");
		strcat( TABLE, p );
		strcat( TABLE, " BORDER=1>");

		*table_tag = TABLE;
		return;
	}

	/*---------- parse and set table tag --------------*/
	if (strUcmp(p,"<TABLE ", 7)==0 || strUcmp(p,"<TABLE>", 7)==0 )
	{
		if ( (p2 = strchr(p+1, '>')) == NULL )
			return;

		*table_tag = p;
	}
	return;
}

static void 
set_header_tr(char *opt, char **header_tr)
/***********************************************************************
tableTag:
   *header_tr:        header <TR> tag
***********************************************************************/
{
	static char HEADER_TR[HEADERTR_LEN];
	static char *header_tr0 = "<tr>";

	char *p = opt;
	char *p2;

	/*---------- initial setteings --------*/
	*header_tr = header_tr0;

	if (opt == NULL || *opt == '\0')
		return;

	while (*p==' ')
		p++;
	if (*p == '\0')
		return;
	
	/*----------- only table tag contents --------------*/
	if (*p != '<' && strlen(p) < HEADERTR_LEN-6 )
	{
		strcpy( HEADER_TR, "<TR ");
		strcat( HEADER_TR, p );
		strcat( HEADER_TR, ">");

		*header_tr = HEADER_TR;
		return;
	}

	/*---------- parse and set table tag --------------*/
	if (strUcmp(p,"<TR ", 4)==0 || strUcmp(p,"<TR>", 4)==0 )
	{
		if ( (p2 = strchr(p+1, '>')) == NULL )
			return;

		*header_tr = p;
	}
	return;
}


static void 
set_header_th(char *opt, int *ntag, char **tag)
/***********************************************************************
 ntag  : no of column of tags
 *tag[]: column value of tags

 example)
    opt='<TH width100>,<TH bgcolor=#DDDDDD>'
    
    nrag = 2
    tag[0]="<TH width=100>"
    tag[1]="<TH bgcolor=#DDDDDD>"
***********************************************************************/
{
	static char HEADER_TH[HEADERTH_LEN];
	char *p;
	char *p2;

	/*---------- initial setteings --------*/
	*ntag = 0;
	if (opt == NULL || *opt == '\0')
		return;

	strcpy(HEADER_TH, opt);
	p = HEADER_TH;

	while (*p==' ') 
		p++;
	if (*p == '\0' || *p != '<' )
		return;

	/*---------- parse and set table tag --------------*/
	while(*p)
	{
		if (strUcmp(p,"<TH ", 4)==0 || strUcmp(p,"<TH>", 4)==0 ||
		    strUcmp(p,"<TD ", 4)==0 || strUcmp(p,"<TD>", 4)==0 ) 
		{
			if ( (p2 = strchr(p+1, '>')) == NULL )
				return;
			tag[*ntag] = p;
			(*ntag)++;
			p2++;
			if( *p2 == '\0' )
				break;
			else
				*p2 = '\0';
			p = p2+1;

			while (*p==' ' || *p==',') 
				p++;
			if (*p == '\0' || *p != '<' )
				break;
		}
		else 
			break;
	}
	return;
}

static void 
set_body_tags(char *opt, int *ntag, char **tag)
/***********************************************************************
ntag  : no of column of tags
*tag[]: column value of tags

example)
    opt='<TD bgcolor=#EEEEE>,<TD bgcolor=#DDDDDD>'
    
    nrag = 2
    tag[0]="<TD bgcolor=#EEEEEE"
    tag[1]="<TD bgcolor=#DDDDDD"

    (attention) the last character '>' is deleted.
***********************************************************************/
{
	static char BODY_TAG[BODYTAG_LEN];
	char *p;
	char *p2;

	/*---------- initial setteings --------*/
	*ntag = 0;
	if (opt == NULL || *opt == '\0')
		return;

	strcpy(BODY_TAG, opt);
	p = BODY_TAG;

	while (*p==' ') 
		p++;
	if (*p == '\0' || *p != '<' )
		return;

	/*---------- parse and set table tag --------------*/
	while(*p)
	{
		if (strUcmp(p,"<TD ", 4)==0 || strUcmp(p,"<TD>", 4)==0 ||
		    strUcmp(p,"<TH ", 4)==0 || strUcmp(p,"<TH>", 4)==0 )
		{
			if ( (p2 = strchr(p+1, '>')) == NULL )
				return;

			tag[*ntag] = p;
			(*ntag)++;
			*p2 = '\0';
			p = p2+1;

			while (*p==' ' || *p==',') 
				p++;
			if (*p == '\0' || *p != '<' )
				break;
		}
		else 
			break;
	}
	return;
}

static void 
set_input_size(char *opt, int *ntag, int *tag)
/***********************************************************************
 ntag  : no of column of tags
 *tag[]: column value of tags

 example)
    opt='-1,0,5'
    
    nrag = 3
    tag[0]=-1 : normal print 
    tag[1]=0  : <input type=text Name=':xx' VALUE='yyy'>
    tag[2]=5  : <input type=text Name=':xx' VALUE='yyy' SIZE=5>
***********************************************************************/
{
	static	char INPUT_SIZE[INPUTSIZE_LEN];
	int	n;
	char	*p, *p2;

	/*---------- initial setteings --------*/
	*ntag = 0;
	if (opt == NULL || *opt == '\0')
		return;

	strcpy(INPUT_SIZE, opt);
	p = INPUT_SIZE;

	while (*p==' ') 
		p++;
	if (*p == '\0')
		return;

	/*---------- parse and set table tag --------------*/
	while(*p)
	{
		if ((p2=strchr(p, ',')) != NULL)
		{
			*p2 = '\0';
			n = atoi(p);

			if( n < -1 || n > 256)
				break;

			tag[*ntag] = n;
			(*ntag)++;
			p2++;
			if( *p2 == '\0' )
				break;
			p = p2;
		}
		else 
		{
			n = atoi(p);

			if( n < -1 || n > 256)
				break;

			tag[*ntag] = n;
			(*ntag)++;
			
			break;
		}
	}
	return;
}

/*======================================================================
 * returns the byte length of the word beginning s, using the
 * specified encoding. PSmblen is compatible with PostgreSQL-6.5/7.0/7.1
 *======================================================================
 */

#ifdef MULTIBYTE
static int
PSmblen(const unsigned char *s, int encoding)
{
	if (encoding == 0 || encoding == SQL_ASCII)
		return 1;
	return (pg_encoding_mblen(encoding, s));
}

#else
static int
PSmblen(const unsigned char *s, int encoding)
{
	return 1;
}
#endif


