/* 
   Copyright (c) 2000-2001 Perry Rapp

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use, copy,
   modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

/*==========================================================
 * lloptions.c -- Read options from config file (& db user options)
 *   added in 3.0.6 by Perry Rapp
 *========================================================*/

#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif
#include "llstdlib.h"
#include "gedcom.h"
#include "lloptions.h"


/*********************************************
 * external variables
 *********************************************/

extern STRING qSopt2long;

/*********************************************
 * local function prototypes
 *********************************************/

/* alphabetical */
static void copy_process(STRING dest, STRING src);
static void create_notification_list_if_needed(void);
static void delete_notification_list_if_needed(void);
static void free_optable(TABLE * ptab);
static BOOLEAN load_config_file(STRING file, STRING * pmsg);

/*********************************************
 * local variables
 *********************************************/

/* table holding option values, both keys & values in heap */
static TABLE f_rpt=0;
static TABLE f_db=0;
static TABLE f_global=0;
static TABLE f_fallback=0;
static LIST f_notifications=0;

/*********************************************
 * local function definitions
 * body of module
 *********************************************/

/*==========================================
 * copy_process -- copy config value line,
 *  converting any escape characters
 *  This handles \n, \t, and \\
 *  We do not trim out backslashes, unless they 
 *  are part of escape sequences. (This is mostly
 *  because backslashes are so prevalent in
 *  MS-Windows paths.)
 * The output (dest) is no longer than the input (src).
 * Created: 2001/11/09, Perry Rapp
 *========================================*/
static void
copy_process (STRING dest, STRING src)
{
	STRING q=dest,p=src;
	char c;
	while ((*q++ = c = *p++)) {
		if (c == '\\') {
			if (!(c = *p++)) {
				*q = 0;
				break;
			}
			if (c == 'n')
				q[-1] = '\n';
			else if (c == 't')
				q[-1] = '\t';
			else if (c == '\\')
				q[-1] = '\\';
			else
				--p;
		}
	}
}
/*==========================================
 * load_config_file -- read options in config file
 *  and load into table (f_global)
 * Created: 2001/02/04, Perry Rapp
 *========================================*/
static BOOLEAN
load_config_file (STRING file, STRING * pmsg)
{
	FILE * fp = 0;
	STRING ptr, val;
	STRING oldval=NULL;
	BOOLEAN there, failed, noesc;
	char buffer[MAXLINELEN],valbuf[MAXLINELEN];
	fp = fopen(file, LLREADTEXT);
	if (!fp)
		return TRUE; /* no config file, that is ok */
	/* read thru config file til done (or error) */
	while (fgets(buffer, sizeof(buffer), fp)) {
		noesc = FALSE;
		if (buffer[0] == '#')
			continue; /* ignore lines starting with # */
		if (!feof(fp) && buffer[strlen(buffer)-1] != '\n') {
			/* bail out if line too long */
			break;
		}
		chomp(buffer); /* trim any trailing CR or LF */
		/* find =, which separates key from value */
		for (ptr = buffer; *ptr && *ptr!='='; ptr++)
			;
		if (*ptr != '=' || ptr==buffer)
			continue; /* ignore lines without = or key */
		*ptr=0; /* zero-terminate key */
		if (ptr[-1] == ':') {
			noesc = TRUE; /* := means don't do backslash escapes */
			ptr[-1] = 0;
		}
		/* overwrite any previous value */
		oldval = valueofbool_str(f_global, buffer, &there);
		if (there) {
			ASSERT(oldval); /* no nulls in f_global*/
			stdfree(oldval);
		}
		/* advance over separator to value */
		ptr++;
		/*
		process the value into valbuf
		this handles escapes (eg, "\n")
		the output (valbuf) is no longer than the input (ptr)
		*/
		if (noesc)
			llstrncpy(valbuf, ptr, sizeof(valbuf));
		else
			copy_process(valbuf, ptr);
		val = valbuf;
		insert_table_str(f_global, strsave(buffer), strsave(val));
	}
	failed = !feof(fp);
	fclose(fp);
	if (failed) {
		/* error is in heap */
		*pmsg = strsave(_(qSopt2long));
		return FALSE;
	}
	return TRUE;
}
/*=================================
 * create_notification_list --
 *===============================*/
static void
create_notification_list_if_needed (void)
{
	if (!f_notifications) {
		f_notifications = create_list();
	}
}
/*=================================
 * delete_notification_list -- 
 *===============================*/
static void
delete_notification_list_if_needed (void)
{
	if (f_notifications) {
		make_list_empty(f_notifications);
		remove_list(f_notifications, 0);
		f_notifications = 0;
	}
}
/*=================================
 * init_lifelines_options -- Initialize LifeLines
 *  before db opened
 * STRING * pmsg: heap-alloc'd error string if fails
 *===============================*/
BOOLEAN
load_global_options (STRING configfile, STRING * pmsg)
{
	*pmsg = NULL;
	term_lloptions(); /* clear if exists */
	f_global= create_table();
	create_notification_list_if_needed();
	if (!load_config_file(configfile, pmsg))
		return FALSE;
	return TRUE;
}
/*=================================
 * set_db_options -- Store db options from caller
 * Created: 2002/06/16, Perry Rapp
 *===============================*/
void
set_db_options (TABLE dbopts)
{
	ASSERT(dbopts);
	free_optable(&f_db);
	f_db = create_table();
	copy_table(dbopts, f_db, FREEBOTH);
}
/*=================================
 * get_db_options -- Copy db options to caller's table
 * Created: 2002/06/16, Perry Rapp
 *===============================*/
void
get_db_options (TABLE dbopts)
{
	if (!f_db)
		f_db = create_table();
	copy_table(f_db, dbopts, FREEBOTH);
}
/*==========================================
 * free_optable -- free a table if it exists
 *========================================*/
static void
free_optable (TABLE * ptab)
{
	if (*ptab) {
		remove_table(*ptab, FREEBOTH);
		*ptab = 0;
	}
}
/*==========================================
 * term_lloptions -- deallocate structures
 * used by lloptions at program termination
 * Safe to be called more than once
 * Created: 2001/04/30, Matt Emmerton
 *========================================*/
void
term_lloptions (void)
{
	free_optable(&f_rpt);
	free_optable(&f_db);
	free_optable(&f_global);
	free_optable(&f_fallback);
	delete_notification_list_if_needed();
}
/*===============================================
 * getoptstr -- get an option (from db or from global)
 * Example: 
 *  str = getoptstr("HDR_SUBM", "1 SUBM");
 * Created: 2001/11/22, Perry Rapp
 *=============================================*/
STRING
getoptstr (STRING optname, STRING defval)
{
	STRING str = 0;
	if (!str && f_db)
		str = valueof_str(f_db, optname);
	if (!str && f_global)
		str = valueof_str(f_global, optname);
	if (!str && f_fallback)
		str = valueof_str(f_fallback, optname);
	if (!str)
		str = defval;
	return str;
}
/*===============================================
 * getoptstr_rpt -- get an option (from report, or db, or global)
 * Example: 
 *  str = getoptstr("HDR_SUBM", "1 SUBM");
 * Created: 2002/06/16, Perry Rapp
 *=============================================*/
STRING
getoptstr_rpt (STRING optname, STRING defval)
{
	STRING str = 0;
	if (!str && f_rpt)
		str = valueof_str(f_rpt, optname);
	if (!str)
		str = defval;
	return str;
}
/*===============================================
 * getoptstr_dbonly -- get an option (but only look at db options)
 * Example: 
 *  str = getoptstr_dbonly("codeset", 0);
 * Created: 2002/06/16, Perry Rapp
 *=============================================*/
STRING
getoptstr_dbonly (STRING optname, STRING defval)
{
	STRING str = 0;
	if (f_db)
		str = valueof_str(f_db, optname);
	if (!str)
		str = defval;
	return str;
}
/*===============================================
 * getoptint -- get a numerical option
 *  First tries user option table (looks up optname)
 *  Then tries config option table
 *  Finally defaults to defval
 * Example: 
	if (getoptint("FullReportCallStack", 0) > 0)
 * Created: 2001/11/22, Perry Rapp
 *=============================================*/
INT
getoptint (STRING optname, INT defval)
{
	STRING str = getoptstr(optname, 0);
	return str ? atoi(str) : defval;
}
/*===============================================
 * setoptstr_fallback -- Set option fallback value
 *  newval must be in heap!
 * Created: 2001/02/04, Perry Rapp
 *=============================================*/
void
setoptstr_fallback (STRING optname, STRING newval)
{
	if (!f_fallback)
		f_fallback = create_table();
	replace_table_str(f_fallback, strsave(optname), newval, FREEBOTH);
}
/*===============================================
 * register_notify -- Put notification on the callback list
 * Created: 2002/06/16, Perry Rapp
 *=============================================*/
void
register_notify (options_notify_fnc fncptr)
{
	create_notification_list_if_needed();
	enqueue_list(f_notifications, (VPTR)fncptr);
}
/*===============================================
 * unregister_notify -- Take notification off the callback list
 * Created: 2002/06/16, Perry Rapp
 *=============================================*/
void
unregister_notify (options_notify_fnc fncptr)
{
	/* Our lists don't have remove from middle, so we just make a new copy */
	LIST lold = 0;
	if (!f_notifications || is_empty_list(f_notifications))
		return;
	lold = f_notifications;
	f_notifications = 0;
	create_notification_list_if_needed();
	while (!is_empty_list(lold)) {
		VPTR vptr = pop_list(lold);
		if ((VPTR)fncptr != vptr)
			enqueue_list(f_notifications, (VPTR)fncptr);
	}
	make_list_empty(lold);
	remove_list(lold, 0);
}