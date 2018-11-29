/*----------------------------------------------------------------
 *
 * print_sub.h
 *	  functions for pretty-printing query results
 *
 * Change Log
 * 2001.06.11: Add 'TableTag' struct.
 * 2001.06.11: Add header_tr,header_th,body_tag into PSprintOpt struct.
 * 2001.06.11: Add input_tag,input_size
 *
 *-----------------------------------------------------------------
 */

#ifndef PRINT_SUB_HEADER
#define PRINT_SUB_HEADER

#ifndef bool
#define bool	char
#endif

/*-------------------------------------------------------------------
	The typedef of printing
-------------------------------------------------------------------*/
typedef struct _PSprintOpt
{
	bool            html3;		/* output html  tables          */
	bool            expanded;	/* set expand type              */
					
	char            *caption;	/* Table Title                  */
	bool            outerframe;	/* print table outer frame      */
	bool            header;		/* print output table headings  */
	bool            rowcount;	/* print output table row count */
	bool            align;		/* fill align the fields        */
	char            *nullstr;	/* NULL string                  */
	char            *zerostr;	/* ZERO string                  */
	char            *fieldSep;	/* field separator              */

	char            *table_tag;	/* HTML table tag               */
	char            *header_tr;	/* HTML header <TR> tag         */
	char            *header_th;	/* HTML header <TH> tag         */
	char            *body_tag;	/* HTML body <TD> tag           */
	bool            input_tag;  /* HTML input tag flag(ON=1,OFF=0)*/
	char            *input_size;/* HTML INPUT tag size          */

	int             nFields;	/* number of Fields		        */
	int             nTups;		/* number of Tuples		        */
	char            **fieldName;/* field name(null terminated)  */
	char            **fieldValue;/* all field's value		    */
	short           *fieldMax;	/* field length                 */
	unsigned char   *fieldNotNum;/* field is Number or not      */

}  PSprintOpt;


/*-------------------------------------------------------------------
 *	  Functions list
 *-----------------------------------------------------------------*/
extern void	PSprint(FILE * fout, PSprintOpt * Opt, int encoding);
extern int  alloc_printOpt(PSprintOpt * Opt, PGresult * res);
extern int  set_printOpt(PSprintOpt * Opt);
extern void free_printOpt(PSprintOpt * Opt);


#endif
