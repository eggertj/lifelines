/* 
   Copyright (c) 1991-1999 Thomas T. Wetmore IV

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
/*=============================================================
 * xreffile.c -- Handle the xref file
 * Copyright(c) 1991-94 by T.T. Wetmore IV; all rights reserved
 *   2.3.4 - 24 Jun 93    2.3.5 - 02 Sep 93
 *   3.0.0 - 02 May 94    3.0.2 - 10 Nov 94
 *===========================================================*/

#include "sys_inc.h"
#include "llstdlib.h"
#include "btree.h"
#include "table.h"
#include "translat.h"
#include "gedcom.h"

extern BTREE BTR;


/*===================================================================
 * First five words in xrefs file are number of INDI, FAM, EVEN, SOUR
 *   and other keys in file; remaining words are keys, in respective
 *   order, for the records; first in each group is next unused key;
 *   rest are keys of deleted records
 * nixrefs==1 means there are no deleted INDI keys
 * nixrefs==2 means there is one deleted INDI key (ixrefs[1])
 *=================================================================*/

/*==================================== 
 * deleteset -- set of deleted records
 *  NB: storage order is IFESX
 *  whereas canonical order is IFSEX
 *==================================*/
struct deleteset_s
{
	INT n; /* num keys + 1, ie, starts at 1 */
	INT * recs;
	INT max;
	char ctype;
};
typedef struct deleteset_s *DELETESET;


static BOOLEAN readxrefs(void);
static void readrecs(DELETESET set);
static STRING getxref(DELETESET set);
static void addxref(INT key, DELETESET set);
static void growxrefs(DELETESET set);

/* INDI, FAM, EVEN, SOUR, other sets */
static struct deleteset_s irecs, frecs, srecs, erecs, xrecs;

static FILE *xreffp=0;	/* open xref file pointer */

/*==================================== 
 * initdset -- Initialize a delete set
 *==================================*/
static void
initdset (DELETESET set, char ctype)
{
	set->ctype = ctype;
	set->max = 0;
	set->n = 1;
	set->recs = 0;
}
/*=================================== 
 * initdsets -- Initialize delete sets
 *=================================*/
static void
initdsets ()
{
	initdset(&irecs, 'I');
	initdset(&frecs, 'F');
	initdset(&srecs, 'S');
	initdset(&erecs, 'E');
	initdset(&xrecs, 'X');
}
/*============================== 
 * initxref -- Create xrefs file
 *============================*/
void
initxref (void)
{
	char scratch[100];
	INT i = 1, j;
	initdsets();
	ASSERT(!xreffp);
	sprintf(scratch, "%s/xrefs", BTR->b_basedir);
	ASSERT(xreffp = fopen(scratch, LLWRITEBINARY));
	for (j = 0; j < 10; j++) {
		ASSERT(fwrite(&i, sizeof(INT), 1, xreffp) == 1);
	}
	fclose(xreffp); xreffp=0;
}
/*============================
 * openxref -- Open xrefs file
 *==========================*/
BOOLEAN
openxref (void)
{
	char scratch[100];
	initdsets();
	ASSERT(!xreffp);
	sprintf(scratch, "%s/xrefs", BTR->b_basedir);
	if (!(xreffp = fopen(scratch, LLREADBINARYUPDATE))) return FALSE;
	return readxrefs();
}
/*==============================
 * closexref -- Close xrefs file
 *============================*/
void
closexref (void)
{
	if (xreffp) {
		fclose(xreffp); xreffp = 0;
	}
}
/*=========================================
 * getxref -- Return new keynum for type
 *  from deleted list if available, or else
 *  a new highnumber
 *  generic for all 5 types
 *=======================================*/
static STRING
getxref (DELETESET set)
{
	INT keynum;
	static unsigned char scratch[12];
	ASSERT(xreffp && set->n >= 1);
	keynum = (set->n == 1) ? set->recs[0]++ : set->recs[--(set->n)];
	ASSERT(writexrefs());
	sprintf(scratch, "@%c%d@", set->ctype, keynum);
	return scratch;
}
/*===================================================
 * get?xref -- Wrappers for each type to getxref (qv)
 *  5 symmetric versions
 *=================================================*/
STRING getixref (void) { return getxref(&irecs); }
STRING getfxref (void) { return getxref(&frecs); }
STRING getsxref (void) { return getxref(&srecs); }
STRING getexref (void) { return getxref(&erecs); }
STRING getxxref (void) { return getxref(&xrecs); }
/*======================================
 * sortxref -- Sort xrefs after reading
 *====================================*/
static void
sortxref (DELETESET set)
{
	/*
	TO DO - call qsort instead
	and also flag sorted file by changing file structure
	*/
	/*
	sort from high to low, so lowest at top of
	array, ready to be handed out

	they should normally already be sorted, 
	so use watchtful bubble-sort for O(n)
	*/
	INT i,j, temp, ct;
	for (i=1; i<set->n; i++) {
		ct=0;
		for (j=i+1; j<set->n; j++) {
			if (set->recs[i] < set->recs[j]) {
				ct++;
				temp = set->recs[j];
				set->recs[j] = set->recs[i];
				set->recs[i] = temp;
			}
			if (i==1 && !ct) return; /* already sorted */
		}
	}
}
/*======================================
 * sortxrefs -- Sort xrefs after reading
 *====================================*/
static void
sortxrefs (void)
{
	sortxref(&irecs);
	sortxref(&frecs);
	sortxref(&srecs);
	sortxref(&erecs);
	sortxref(&xrecs);
}
/*=============================
 * readxrefs -- Read xrefs file
 *  storage order: IFESX
 *===========================*/
static BOOLEAN
readxrefs (void)
{
	ASSERT(xreffp);
	ASSERT(fread(&irecs.n, sizeof(INT), 1, xreffp) == 1);
	ASSERT(fread(&frecs.n, sizeof(INT), 1, xreffp) == 1);
	ASSERT(fread(&erecs.n, sizeof(INT), 1, xreffp) == 1);
	ASSERT(fread(&srecs.n, sizeof(INT), 1, xreffp) == 1);
	ASSERT(fread(&xrecs.n, sizeof(INT), 1, xreffp) == 1);
	ASSERT(irecs.n > 0);
	ASSERT(frecs.n > 0);
	ASSERT(erecs.n > 0);
	ASSERT(srecs.n > 0);
	ASSERT(xrecs.n > 0);
	if (irecs.n > irecs.max) growxrefs(&irecs);
	if (frecs.n > frecs.max) growxrefs(&frecs);
	if (erecs.n > erecs.max) growxrefs(&erecs);
	if (srecs.n > srecs.max) growxrefs(&srecs);
	if (xrecs.n > xrecs.max) growxrefs(&xrecs);
	readrecs(&irecs);
	readrecs(&frecs);
	readrecs(&erecs);
	readrecs(&srecs);
	readrecs(&xrecs);
	sortxrefs();
	return TRUE;
}
/*=========================================
 * readrecs -- Read in one array of records
 *=======================================*/
static void
readrecs (DELETESET set)
{
	ASSERT((INT)fread(set->recs, sizeof(INT), set->n, xreffp) == set->n);
}
/*================================
 * writexrefs -- Write xrefs file.
 *  storage order: IFESX
 *==============================*/
BOOLEAN
writexrefs (void)
{
	ASSERT(xreffp);
	rewind(xreffp);
	ASSERT(fwrite(&irecs.n, sizeof(INT), 1, xreffp) == 1);
	ASSERT(fwrite(&frecs.n, sizeof(INT), 1, xreffp) == 1);
	ASSERT(fwrite(&erecs.n, sizeof(INT), 1, xreffp) == 1);
	ASSERT(fwrite(&srecs.n, sizeof(INT), 1, xreffp) == 1);
	ASSERT(fwrite(&xrecs.n, sizeof(INT), 1, xreffp) == 1);
	ASSERT((INT)fwrite(irecs.recs, sizeof(INT), irecs.n, xreffp) == irecs.n);
	ASSERT((INT)fwrite(frecs.recs, sizeof(INT), frecs.n, xreffp) == frecs.n);
	ASSERT((INT)fwrite(erecs.recs, sizeof(INT), erecs.n, xreffp) == erecs.n);
	ASSERT((INT)fwrite(srecs.recs, sizeof(INT), srecs.n, xreffp) == srecs.n);
	ASSERT((INT)fwrite(xrecs.recs, sizeof(INT), xrecs.n, xreffp) == xrecs.n);
	fflush(xreffp);
	return TRUE;
}
/*=====================================
 * addxref -- Add deleted key to xrefs.
 *  generic for all types
 *===================================*/
static void
addxref (INT key, DELETESET set)
{
	INT lo,hi,md, i;
	if (key <= 0 || !xreffp || (set->n) < 1) FATAL();
	if (set->n >= set->max)
		growxrefs(set);
	ASSERT(set->n < set->max);
	lo=1;
	hi=(set->n)-1;
	/* binary search to find where to insert key */
	while (lo<=hi) {
		md = (lo + hi)/2;
		if (key>(set->recs)[md])
			hi=--md;
		else if (key<(set->recs)[md])
			lo=++md;
		else
			FATAL(); /* deleting a deleted record! */
	}
	/* key replaces xrefs[lo] - push lo+ up */
	for (i=lo; i<set->n; i++)
		(set->recs)[i+1] = (set->recs)[i];
	(set->recs)[lo] = key;
	(set->n)++;
	ASSERT(writexrefs());
}
/*===================================================
 * add?xref -- Wrappers for each type to addxref (qv)
 *  5 symmetric versions
 *=================================================*/
void addixref (INT key) { addxref(key, &irecs); }
void addfxref (INT key) { addxref(key, &frecs); }
void addsxref (INT key) { addxref(key, &srecs); }
void addexref (INT key) { addxref(key, &erecs); }
void addxxref (INT key) { addxref(key, &xrecs); }
/*==========================================
 * growxrefs -- Grow memory for xrefs array.
 *  generic for all types
 *========================================*/
static void
growxrefs (DELETESET set)
{
	INT i, m = set->max, *newp;
	if (set->n < 500)
		set->max = set->n + 10;
	else
		set->max = set->n + 100;
	newp = (INT *) stdalloc((set->max)*sizeof(INT));
	if (m) {
		for (i = 0; i < set->n; i++)
			newp[i] = set->recs[i];
		stdfree(set->recs);
	}
	set->recs = newp;
}
/*==========================================================
 * num_????s -- Return number of type of things in database.
 *  5 symmetric versions
 *========================================================*/
INT num_set (DELETESET set)
{
	return set->recs[0] - set->n;
}
INT num_indis (void) { return num_set(&irecs); }
INT num_fams (void) { return num_set(&frecs); }
INT num_sours (void) { return num_set(&srecs); }
INT num_evens (void) { return num_set(&erecs); }
INT num_othrs (void) { return num_set(&xrecs); }
/*================================================
 * newixref -- Return original or next ixref value
 * xrefp = key of the individual
 * flag = use the current key
 *  returns static buffer
 *==============================================*/
STRING
newxref (STRING xrefp, BOOLEAN flag, DELETESET set)
{
	INT keynum;
	BOOLEAN changed;
	static unsigned char scratch[12];
	if(flag) {
		keynum = atoi(xrefp+1);
		changed = ((set->n != 1) || (keynum >= set->recs[0]));
		if(set->n != 1)
			set->n = 1;	/* forget about deleted entries */
		if(keynum >= set->recs[0])
			set->recs[0] = keynum+1;	/* next available */
		if(changed)
			ASSERT(writexrefs());
		sprintf(scratch, "@%s@", xrefp);
		return(scratch);
	}
	return(getxref(set));
}
/*================================================
 * newixref -- Return original or next ixref value
 * xrefp = key of the individual
 * flag = use the current key
 *==============================================*/
STRING
newixref (STRING xrefp, BOOLEAN flag)
{
	return newxref(xrefp, flag, &irecs);
}
/*================================================
 * newfxref -- Return original or next fxref value
 * xrefp = key of the individual
 * flag = use the current key
 *==============================================*/
STRING
newfxref (STRING xrefp, BOOLEAN flag)
{
	return newxref(xrefp, flag, &frecs);
}
/*================================================
 * newsxref -- Return original or next sxref value
 * xrefp = key of the individual
 * flag = use the current key
 *==============================================*/
STRING
newsxref (STRING xrefp, BOOLEAN flag)
{
	return newxref(xrefp, flag, &srecs);
}
/*================================================
 * newexref -- Return original or next exref value
 * xrefp = key of the individual
 * flag = use the current key
 *==============================================*/
STRING
newexref (STRING xrefp, BOOLEAN flag)
{
	return newxref(xrefp, flag, &erecs);
}
/*================================================
 * newxxref -- Return original or next xxref value
 * xrefp = key of the individual
 * flag = use the current key
 *==============================================*/
STRING
newxxref (STRING xrefp, BOOLEAN flag)
{
	return newxref(xrefp, flag, &xrecs);
}
/*================================================
 * xref_isvalid_impl -- is this a valid whatever ?
 *  generic for all 5 types
 * (internal use)
 *==============================================*/
static BOOLEAN
xref_isvalid_impl (DELETESET set, INT keynum)
{
	INT lo,hi,md;
	if (set->n == set->recs[0]) return FALSE; /* no valids */
	if (set->n == 1) return TRUE; /* all valid */
	/* binary search deleteds */
	lo=1;
	hi=(set->n)-1;
	while (lo<=hi) {
		md = (lo + hi)/2;
		if (keynum>(set->recs)[md])
			hi=--md;
		else if (keynum<(set->recs)[md])
			lo=++md;
		else
			return FALSE;
	}
	return TRUE;
}
/*=========================================================
 * xref_next_impl -- Return next valid of some type after i
 *  returns 0 if none found
 *  generic for all 5 types
 *  this could be more efficient (after first one work
 *  thru tree)
 *=======================================================*/
static INT
xref_next_impl (DELETESET set, INT i)
{
	if (set->n == set->recs[0]) return 0; /* no valids */
	while (++i < set->recs[0])
	{
		if (xref_isvalid_impl(set, i)) return i;
	}
	return 0;
}
/*==========================================================
 * xref_prev_impl -- Return prev valid of some type before i
 *  returns 0 if none found
 *  generic for all 5 types
 *========================================================*/
static INT
xref_prev_impl (DELETESET set, INT i)
{
	if (set->n == set->recs[0]) return 0; /* no valids */
	while (--i)
	{
		if (xref_isvalid_impl(set, i)) return i;
	}
	return 0;
}
/*===============================================
 * xref_next? -- Return next valid indi/? after i
 *  returns 0 if none found
 *  5 symmetric versions
 *=============================================*/
INT xref_nexti (INT i) { return xref_next_impl(&irecs, i); }
INT xref_nextf (INT i) { return xref_next_impl(&frecs, i); }
INT xref_nexts (INT i) { return xref_next_impl(&srecs, i); }
INT xref_nexte (INT i) { return xref_next_impl(&erecs, i); }
INT xref_nextx (INT i) { return xref_next_impl(&xrecs, i); }
INT xref_next (char ntype, INT i)
{
	switch(ntype) {
	case 'I': return xref_nexti(i);
	case 'F': return xref_nextf(i);
	case 'S': return xref_nexts(i);
	case 'E': return xref_nexte(i);
	case 'X': return xref_nextx(i);
	}
	ASSERT(0); return 0;
}
/*================================================
 * xref_prev? -- Return prev valid indi/? before i
 *  returns 0 if none found
 *  5 symmetric versions
 *==============================================*/
INT xref_previ (INT i) { return xref_prev_impl(&irecs, i); }
INT xref_prevf (INT i) { return xref_prev_impl(&frecs, i); }
INT xref_prevs (INT i) { return xref_prev_impl(&srecs, i); }
INT xref_preve (INT i) { return xref_prev_impl(&erecs, i); }
INT xref_prevx (INT i) { return xref_prev_impl(&xrecs, i); }
INT xref_prev (char ntype, INT i)
{
	switch(ntype) {
	case 'I': return xref_previ(i);
	case 'F': return xref_prevf(i);
	case 'S': return xref_prevs(i);
	case 'E': return xref_preve(i);
	case 'X': return xref_prevx(i);
	}
	ASSERT(0); return 0;
}
/*=========================================
 * xref_first? -- Return first valid indi/?
 *  returns 0 if none found
 *  5 symmetric versions
 *=======================================*/
INT xref_firsti (void) { return xref_nexti(0); }
INT xref_firstf (void) { return xref_nextf(0); }
INT xref_firsts (void) { return xref_nexts(0); }
INT xref_firste (void) { return xref_nexte(0); }
INT xref_firstx (void) { return xref_nextx(0); }
/*=======================================
 * xref_last? -- Return last valid indi/?
 *  returns 0 if none found
 *  5 symmetric versions
 *=====================================*/
INT xref_last (DELETESET set)
{
	return xref_prev_impl(set, set->recs[0]);
}
INT xref_lasti (void) { return xref_last(&irecs); }
INT xref_lastf (void) { return xref_last(&frecs); }
INT xref_lasts (void) { return xref_last(&srecs); }
INT xref_laste (void) { return xref_last(&erecs); }
INT xref_lastx (void) { return xref_last(&xrecs); }
