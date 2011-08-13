/*
 * Copyright (c) 2002-2005 The TenDRA Project <http://www.tendra.org/>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of The TenDRA Project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific, prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
 * IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id$
 */
/*
    Copyright (c) 1993 Open Software Foundation, Inc.


    All Rights Reserved


    Permission to use, copy, modify, and distribute this software
    and its documentation for any purpose and without fee is hereby
    granted, provided that the above copyright notice appears in all
    copies and that both the copyright notice and this permission
    notice appear in supporting documentation.


    OSF DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING
    ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE.


    IN NO EVENT SHALL OSF BE LIABLE FOR ANY SPECIAL, INDIRECT, OR
    CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
    LOSS OF USE, DATA OR PROFITS, WHETHER IN ACTION OF CONTRACT,
    NEGLIGENCE, OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
    WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/*
    		 Crown Copyright (c) 1997

    This TenDRA(r) Computer Program is subject to Copyright
    owned by the United Kingdom Secretary of State for Defence
    acting through the Defence Evaluation and Research Agency
    (DERA).  It is made available to Recipients with a
    royalty-free licence for its use, reproduction, transfer
    to other parties and amendment for any purpose not excluding
    product development provided that any such use et cetera
    shall be deemed to be acceptance of the following conditions:-

        (1) Its Recipients shall ensure that this Notice is
        reproduced upon any copies or amended versions of it;

        (2) Any amended version of it shall be clearly marked to
        show both the nature of and the organisation responsible
        for the relevant amendment or amendments;

        (3) Its onward transfer from a recipient to another
        party shall be deemed to be that party's acceptance of
        these conditions;

        (4) DERA gives no warranty or assurance as to its
        quality or suitability for any purpose and DERA accepts
        no liability whatsoever in relation to any use to which
        it may be put.
*/



/**********************************************************************
$Author: release $
$Date: 1998/02/04 15:48:42 $
$Revision: 1.3 $
$Log: diagout.c,v $
 * Revision 1.3  1998/02/04  15:48:42  release
 * Added OSF copyright message.
 *
 * Revision 1.2  1998/02/04  10:43:52  release
 * Changes during testing.
 *
 * Revision 1.1.1.1  1998/01/17  15:55:56  release
 * First version to be checked into rolling release.
 *
 * Revision 1.2  1996/10/04  16:00:10  pwe
 * add banners and mod for PWE ownership
 *
**********************************************************************/


#include "config.h"
#include "cross_config.h"
#include <time.h>

#if defined(CROSS_INCLUDE)
#include CROSS_INCLUDE/dbxstclass.h>
#include CROSS_INCLUDE/sys/debug.h>
#elif defined(__AIX)
#include <dbxstclass.h>
#include <sys/debug.h>
#endif

#include "common_types.h"
#include "memtdf.h"
#include "installglob.h"
#include "li_types.h"
#include "diagtypes.h"
#include "toktypes.h"
#include "diag_fns.h"
#include "diagglob.h"
#include "mark_scope.h"
#include "externs.h"		/* for f_floating() */
#include "translat.h"
#include "machine.h"
#include "szs_als.h"
#include "install_fns.h"
#include "xalloc.h"

#include "maxminmacs.h"
#include "proc.h"

#include "comment.h"
#include "myassert.h"
#include "stack.h"
#include "diagout.h"



/* configuration options */

/* maximum .bb/.eb nesting depth in output, negative value implies no limit */
#ifndef MAX_BLOCK_DEPTH
#define MAX_BLOCK_DEPTH		1	/* more than one seems to upset dbx */
#endif




/* special line number codes */
#define UNKNOWN			(-9999)
#define NOT_IN_PROC		(-9998)


/* TypeId INTEGER number for standard TDF shapes */
#define TYPEID_SCHAR		1
#define TYPEID_UCHAR		2
#define TYPEID_SWORD		3
#define TYPEID_UWORD		4
#define TYPEID_SLONG		5
#define TYPEID_ULONG		6
#define TYPEID_SHREAL		7
#define TYPEID_REAL		8
#define TYPEID_DOUBLE		9
#define TYPEID_VOID		10
#define TYPEID_TOKEN		11



/* tdfstring -> nul terminated C string (char *) */
#define CSTRING(tdfstring)	((tdfstring).ints.chars)

/* tdf exp -> C int */
#define EXPINT(exp)		(ASSERT(name(exp) == val_tag), no(exp))

/* tdf nat -> C int */
#define NATINT(n)		((n).nat_val.small_nat)



/*
 * Query and set "been_outed" field, which may hold
 * stabstring TypeNo if type has already been output.
 */

#define	CLR_OUTED_NO(dt)	((dt) ->been_outed = (OUTPUT_REC)0)
#define	SET_OUTED_NO(dt,i)	((dt) ->been_outed = (OUTPUT_REC)(i))
#define	OUTED_NO(dt)		((int)(dt) ->been_outed)
#define	IS_OUTED(dt)		((dt) ->been_outed != 0)


/*
 * Params for stab_relativeline().
 */

static const char line_stab[] = ".line";	/* line number */
static const char bf_stab[] = ".bf";	/* begin function */
static const char ef_stab[] = ".ef";	/* end function */
static const char bb_stab[] = ".bb";	/* begin block */
static const char eb_stab[] = ".eb";	/* end block */


/*
 * Location in the source program
 */

static int current_fileno;	/* file number of last file output */
static int current_lineno;	/* the last line number output */
static int files_stabbed;	/* number of .file or .bi output */
static int first_fileno;

static int current_procstart_lineno;	/* line proc started on */

static int block_depth;		/* .bb/.eb nesting depth */

static filename *fds;		/* known files numbered from 0 */
static int mainfile_fd = -1;	/* index into fds of 'main source file' */
static int szfds = 0;		/* space malloc'd for fds */
static int nofds = 0;		/* how many are known */


/*
 * Struct/union type information.
 * (See function OUTPUT_DIAG_TAGS)
 */

static diag_tagdef **su_diags = (diag_tagdef **)0;
static int no_of_sus = 0;
static int leng_sus = 0;

/*
 * Typedef information
 * (See function OUTPUT_GLOBALS_TAB)
 */

static diag_descriptor **typedef_diags = (diag_descriptor **)0;
static int no_of_typedefs = 0;
static int leng_typedefs = 0;

/*
 * Shapes we need, set in init_diag.
 */

static shape voidsh;		/* shape for void */
static shape tokensh;		/* shape for TDF tokenised type */


/*
 * Static procs.
 */

static diag_descriptor *find_dd(exp);
static int find_file(char *);
static void stab_relativeline(const char *);
static void stab_begin_block(void);
static void stab_end_block(void);
static void stabn(int, int);
static void stab_file(int);
static void diagbr_open(int);
static void diagbr_close(int);
static int next_typen(void);
static int TypeNo_of_shape(shape);
static bool eq_sutype(diag_type, diag_type);
static bool eq_typedef_type(diag_descriptor *,diag_descriptor *);
static int size_dt(diag_type);
static void out_dt_TypeDef_no_recurse(diag_type);
static void out_dt_TypeDef(diag_type);
static void out_dt_NewTypeId(diag_type, int);
static void out_dt_TypeId(diag_type);
static void stab_internal_types(diag_type, bool);
static void stab_types(void);

static void number_and_stab_basicshapes(void);
static void number_structs_and_unions(void);
static void stab_structs_and_unions(void);
static void number_typedefs(void);
static void stab_typedefs(void);
static void stab_basicshape(shape,char * ,int,int);


/*(See diag_config.h)
 * remember a filename so that find_file() can map onto filename number
 */
void INSPECT_FILENAME(filename f)
{
  FULLCOMMENT2("INSPECT_FILENAME %d: '%s'", nofds,(int)CSTRING(f->file));

  if (fds == (filename *)0)
  {
    fds = (filename *)xmalloc((szfds += 5)* sizeof(filename));
  }
  else if (nofds >= szfds)
  {
    fds = (filename *)xrealloc(fds,(szfds += 5)* sizeof(filename));
  }

  fds[nofds] = f;

  /*
   * The following is taken from trans386/dwarf/dwarf_out.c.
   * It works out the main source filename. There must be a better way. +++
   */
  if (mainfile_fd < 0)
  {
    /* use heuristic to see if this is 'main source file' */
    char *lastdot = strrchr(CSTRING(f->file), '.');

    /* XXX.h not considered 'main source file' */
    if (lastdot != 0 && lastdot[1]!= 'h')
      mainfile_fd = nofds;
  }

  nofds++;
  return;
}

/*(See diag_config.h)
 * outputs structs & unions from global level types
 */
void OUTPUT_DIAG_TAGS(void)
{
  diag_tagdef **di = unit_ind_diagtags;
  unsigned int n = unit_no_of_diagtags;
  int i;
  int il = no_of_sus; /* There should be no clashes within a single unit so we save a bit of time here */

  for (i = 0; i < n; ++i)
  {
    diag_type d = di[i] ->d_type;

    switch (d->key)
    {
     case DIAG_TYPE_STRUCT:
     case DIAG_TYPE_UNION:
      {
	int j;

	for (j = 0; j < il; j++)
	{
	  /* Check we have not done already */
	  if (eq_sutype(d, su_diags[j] ->d_type))
	  {
	    break;
	  }
	}
	if (j != il)
	{
	  break;
	}
	if (no_of_sus >= leng_sus)
	{
	  if (su_diags == (diag_tagdef **)0)
	  {
	    su_diags= (diag_tagdef**)xmalloc((leng_sus+=20)*sizeof(diag_tagdef*));
	  }
	  else
	  {
	    su_diags= (diag_tagdef**)xrealloc(su_diags,(leng_sus+=20)*sizeof(diag_tagdef*));
	  }
	}
	/* Set up the appropriate member of the su_diags array */
	su_diags[no_of_sus++] = di[i];
	break;
      }
    default:
      break;
    }
  }
  return;
}
/*
 * Collects information
 */
void OUTPUT_GLOBALS_TAB(void)
{
  diag_descriptor * unit_typedef_array = unit_diagvar_tab.array;
  unsigned int no_of_typedefs_in_unit =unit_diagvar_tab.lastused;
  int i;
  int il = no_of_typedefs;

  for (i=0; i<=no_of_typedefs_in_unit; i++)
  {
    diag_descriptor * dd = unit_typedef_array + i;

    switch (dd->key)
    {
     case DIAG_TYPEDEF_KEY:
      {
	int j;

	/* Check that we have not done already */
	for (j=0; j < il; j++)
	{
	  if (eq_typedef_type(dd,typedef_diags[j]))
	  {
	    break;
	  }
	}
	if (j != il)
	{
	  break;
	}

	/* We have a new one */
	if (no_of_typedefs >= leng_typedefs)
	{
	  if (typedef_diags == (diag_descriptor **)0)
	  {
	    typedef_diags = (diag_descriptor **)xmalloc((leng_typedefs+=20)*sizeof(diag_descriptor *));
	  }
	  else
	  {
	    typedef_diags = (diag_descriptor **)xrealloc(typedef_diags,(leng_typedefs+=20)*sizeof(diag_descriptor *));
	  }
	}
	typedef_diags[no_of_typedefs++] =dd;
      }
      break;
     default:
      break;
    }
  }
}


static void number_and_stab_basicshapes(void)
{
#if defined(__AIX) || defined(CROSS_INCLUDE)
  /* NOTE: char is unsigned char */
  stab_basicshape(scharsh ,"signed char"   , TYPEID_SCHAR , TP_SCHAR);
  stab_basicshape(ucharsh ,"char"          , TYPEID_UCHAR , TP_CHAR);
  stab_basicshape(swordsh ,"short"         , TYPEID_SWORD , TP_SHORT);
  stab_basicshape(uwordsh ,"unsigned short", TYPEID_UWORD , TP_USHORT);
  stab_basicshape(slongsh ,"int"           , TYPEID_SLONG , TP_LONG);
  stab_basicshape(ulongsh ,"unsigned int"  , TYPEID_ULONG , TP_ULONG);
  stab_basicshape(shrealsh,"float"         , TYPEID_SHREAL, TP_FLOAT);
  stab_basicshape(realsh  ,"double"        , TYPEID_REAL  , TP_DOUBLE);
  stab_basicshape(doublesh,"long double"   , TYPEID_DOUBLE, TP_LDOUBLE);
  stab_basicshape(voidsh  ,"void"          , TYPEID_VOID  , TP_VOID);

#if 0
  /* this work fine with dbx and gdb, the type is given as "TOKENISED_TYPE" */
  stab_basicshape(tokensh, "TOKENISED_TYPE", TYPEID_TOKEN, TP_VOID);
#else
  /*
   * ... but this is better
   * dbx gives type as "@TOKENISED_TYPE", gdb "struct <unknown>",
   * when value is printed, dbx says "[typeref]", gdb "<incomplete type>"
   */
  {
    int n = next_typen();	/* use up the typen TYPEID_TOKEN */

    ASSERT(TYPEID_TOKEN == n);

    /*
     * TypeId:	    INTEGER = TypeDef	"New type number described by TypeDef"
     * TypeDef:	    o NAME ;		"Opaque type"
     */
    fprintf(as_file, "\t.stabx\t\":t%d=oTOKENISED_TYPE;", TYPEID_TOKEN);
    fprintf(as_file, "\",%d,%d,%d\n", 0, C_DECL, 0);
  }
#endif
#endif
}

static void number_structs_and_unions(void)
{
  int i;

  for (i = 0; i < no_of_sus; ++i)
  {
    SET_OUTED_NO(su_diags[i] ->d_type, next_typen());
  }
}
static void number_typedefs(void)
{
  int i;

  for (i=0;i<no_of_typedefs; i++)
  {
    if (!IS_OUTED(typedef_diags[i] ->data.typ.new_type))
    {
      SET_OUTED_NO(typedef_diags[i] ->data.typ.new_type,next_typen());
    }
  }
}

static void stab_structs_and_unions(void)
{
#if defined(__AIX) || defined(CROSS_INCLUDE)
  int i;

  for (i = 0; i < no_of_sus; ++i)
  {
    int non;
    diag_type dt = su_diags[i] ->d_type;
    char *nme = (dt->key == DIAG_TYPE_STRUCT)
			? CSTRING(dt->data.t_struct.nme)
			: CSTRING(dt->data.t_union.nme);

    COMMENT2("su_diags: is_struct=%d nme='%s'", dt->key == DIAG_TYPE_STRUCT,(int)nme);

    stab_internal_types(dt, 0);

    if (nme == (char *)0 || *nme == (char)0)
    {
      /*
       * Output unnamed object here for 2 reasons:
       *	1) gdb (but not dbx) is confused by Typeid which includes a
       *	   full TypeDef and treats it as an anonymous struct.
       *	   It prefers TypeDef to refer to a previous TypeId INTEGER.
       *	2) It avoids very long stabstrings from unnamed object
       *	   being expanded on the fly.
       */
      fprintf(as_file, "\t.stabx\t\":T");	/* Unnamed object */
    }
    else
    {
      fprintf(as_file, "\t.stabx\t\"%s:T", nme);
    }

    non = OUTED_NO(dt);
    CLR_OUTED_NO(dt);			/* avoid identity */
    out_dt_NewTypeId(dt, non);

    fprintf(as_file, "\",%d,%d,%d\n", 0, C_DECL, 0);
  }
#endif
}
static void stab_typedefs(void)
{
#if defined(__AIX) || defined(CROSS_INCLUDE)
  int i;
  for (i=0;i<no_of_typedefs;i++)
  {
    diag_descriptor * dd = typedef_diags[i];
    int non;
    diag_type dt = dd->data.typ.new_type;
    stab_internal_types(dt,0);
    ASSERT(CSTRING(dd->data.typ.nme)[0]!=0);/* Not an empty string */

    fprintf(as_file, "\t.stabx\t\"%s:t", CSTRING(dd->data.typ.nme));

    non = OUTED_NO(dt);
    CLR_OUTED_NO(dt);			/* avoid identity */
    out_dt_NewTypeId(dt, non);

    fprintf(as_file, "\",%d,%d,%d\n", 0, C_DECL, 0);
  }
#endif
}

/*
 * init_diag - used by translate_capsule() to initialise diagnostics.
 */
void init_diag(void)
{
  /*
   * Initialisation of diagnostics
   */
  current_fileno = UNKNOWN;
  current_procstart_lineno = NOT_IN_PROC;
  current_lineno = NOT_IN_PROC;
  files_stabbed = 0;
  first_fileno = -100;
  stab_file(mainfile_fd);
  voidsh = getshape(0, 0, 0, TOP_ALIGN, TOP_SZ, tophd);/* very dubious */
  tokensh = getshape(0, 0, 0, TOP_ALIGN, TOP_SZ, bothd);/* very dubious */
  stab_types();
  return;
}


/*
 * Correct the name for non externs (eg C static) to keep dbx happy.
 * gdb happily uses the stab info to override the assembler label,
 * but dbx does not.  local_prefix names (eg S.999) upset dbx,
 * partcularly on stack tracebacks from 'where'.  If we find that
 * the name is already in use, eg possibly from nested C static
 * we change the name to "S.NNN.name" to add meaning, but stay unique.
 */
void fixup_name(exp global, dec * top_def, dec * crt_def)
{
  diag_descriptor *dd;
  dec *d;
  char *id = crt_def->dec_u.dec_val.dec_id;	/* assembler label */
  char *nm;			/* real name, or uniqued real name */

  /*
   * Externs have correct name currently.
   * Any name not starting with local_prefix is special, leave it.
   */
  if (crt_def->dec_u.dec_val.extnamed ||
      id[0]!= local_prefix[0] || id[1]!= local_prefix[1])
  {
    return;			/* externs have correct name currently */
  }

  dd = find_dd(global);

  if (dd == (diag_descriptor *)0)
  {
    /* eg string constant or currently static within proc */
    FULLCOMMENT("correct_name: no descriptor");
    return;
  }

  nm = CSTRING(dd->data.id.nme);

  /* search def chain to see if name is already used as assembler label */
  for (d = top_def; d != crt_def && d != (dec *)0; d = d->def_next)
  {
    if (strcmp(nm, d->dec_u.dec_val.dec_id) == 0)
    {

      /*
       * Name already used, create new name to add meaning but stay unique:
       * "S.NNNN.name".
       */
      int len1 = strlen(id);
      int len2 = strlen(nm);
      char *newid;

      newid = (char *)xmalloc(len1 + 1 + len2 + 1);
      strcpy(newid, id);
      newid[len1] = '.';
      strcpy(&newid[len1 + 1], nm);
      nm = newid;
      break;
    }
  }

  crt_def->dec_u.dec_val.dec_id = nm;	/* change label to real identifier
					 * name */
}


/*
 * output_diag - used by make_code() to implement "diagnose_tag".
 */
void output_diag(diag_info * d, int proc_no, exp e)
{
  exp id;

  FULLCOMMENT1("output_diag: key=%d", d->key);

  if (d->key == DIAG_INFO_SOURCE)
  {
    sourcemark *s = &d->data.source.beg;
    int f = find_file(CSTRING(s->file->file));

    stabn(f, NATINT(s->line_no));
    return;
  }

  if (d->key != DIAG_INFO_ID)
  {
    FULLCOMMENT1("output_diag: unsupported key=%d", d->key);
    return;
  }

  id = son(d->data.id_scope.access);

  FULLCOMMENT3("output_diag: DIAG_INFO_ID %s isglob(id) =%d no(id) =%d",
	      (int)CSTRING(d->data.id_scope.nme), isglob(id), no(id));

  /* can't output global values as local names */
  if (isglob(id))
  {
    return;
  }

  ASSERT(name(id) == ident_tag);

  mark_scope(e);
  FULLCOMMENT1("output_diag: DIAG_INFO_ID mark_scope props(e) =%#x", props(e));

  if (props(e) & 0x80)
  {
    diagbr_open(current_fileno);
    stabn(current_fileno, current_lineno + 1);	/* don't have proper lineno */
  }

  /* dbx is upset if a function does not have a begin block, make sure it does */
  if (!isparam(id) && block_depth == 0)
  {
    stab_begin_block();
  }
  stab_local(CSTRING(d->data.id_scope.nme), d->data.id_scope.typ,
	     id, 0, current_fileno);

  if (isparam(id) && last_caller_param(id))
  {
    if (block_depth == 0)
    {
      stab_begin_block();
    }
#if 0
    stabn(current_fileno, current_lineno + 1);	/* don't have proper lineno */
#endif
  }
  return;
}


/*
 * output_end_scope - used by make_code() to make end of diagnose_tag scope.
 */
void output_end_scope(diag_info * d, exp e)
{
  FULLCOMMENT1("output_end_scope key=%d", d->key);
  if (d->key == DIAG_INFO_SOURCE)
  {
    sourcemark *s = &d->data.source.end;
    int f = find_file(CSTRING(s->file->file));
    int lno = NATINT(s->line_no);

    stabn(f, lno);
    return;
  }
  if (d->key == DIAG_INFO_ID && props(e) & 0x80)
  {
    diagbr_close(current_fileno);
    return;
  }
}
/*
 * find_dd
 */
static diag_descriptor *find_dd(exp e)
{
  if (diag_def==NULL)
  {
    return NULL;
  }
  return diag_def->dec_u.dec_val.diag_info;
}




/*
 * get filename number
 */
static int find_file(char *f)
{
  int i;

  for (i = 0; i < nofds; i++)
  {
    if (strcmp(f, CSTRING(fds[i] ->file)) == 0)
    {
      return i;
    }
  }
  return 0;
}



/*
 * ============================================================================
 *		Procs to generate line number related stabs
 */


/*
 * output directive using proc start relative linenos
 */
static void stab_relativeline(const char *directive)
{
  int lineinproc = current_lineno - current_procstart_lineno;

  /* avoid nonsense line nos from unusual header or #line usage */
  if (lineinproc >= 0)
  {
    fprintf(as_file, "\t%s\t%d\n", directive, lineinproc + 1);
  }
  else if (directive != line_stab)
  {
    fprintf(as_file, "\t%s\t%d\n", directive, 1);	/* must put out
							 * something to match
							 * begin/end */
  }
  return;
}


/*
 * output begin block directive
 */
static void stab_begin_block(void)
{
#if 0
  if (MAX_BLOCK_DEPTH < 0 || block_depth < MAX_BLOCK_DEPTH)
#endif
  {
    stab_relativeline(bb_stab);
  }
  block_depth++;
  return;
}

/*
 * output end block directive
 */
static void stab_end_block(void)
{
  if (block_depth >= 0)
  {
#if 0
    if (MAX_BLOCK_DEPTH < 0 || block_depth <= MAX_BLOCK_DEPTH)
#endif
    {
      stab_relativeline(eb_stab);
    }
    block_depth--;
  }
  return;
}


/*
 * Generate stabs for current file and line number.
 * Note that line number stabs are relative to start of current proc.
 */
static void stabn(int findex, int lno)
{
  if (findex == current_fileno && lno == current_lineno)
  {
    return;
  }
  if (findex != current_fileno)
  {
    stab_file(findex);
  }
  if (current_procstart_lineno == NOT_IN_PROC)
  {
    return;
  }

  current_lineno = lno;
  stab_relativeline(line_stab);
  return;
}
/*
 * stab_end_file ties up lose ends
 */
void stab_end_file(void)
{
  if (current_fileno != first_fileno)
  {
    COMMENT("stab_end_file: close the last include file with  a .ei");
    /* close the last include file */
    fprintf(as_file, "\t.ei\t\"%s\"\n", CSTRING(fds[current_fileno] ->file));
  }
  return;
}

/*
 * output file name if changed
 */
static void stab_file(int findex)
{
  bool stabbed = 0;

  if ((findex == current_fileno) || (findex < 0) || (findex >= szfds))
  {
    return;
  }

  if (files_stabbed == 0)
  {
    /* .file */
    fprintf(as_file, "\n\t.file\t\"%s\"\n", CSTRING(fds[findex] ->file));
    stabbed = 1;
    first_fileno = findex;
  }
  else
  {
    /* .bi (begin include) if appropriate */

    /*
     * +++ .bi/.ei causes dbx to core dump, see IBM problem fix (APAR)
     * +++ IX26109 bosadt: .bi/.ei placement doesn't match xlc, debuggers suffer
     * +++ so we leave them commented out currently.
     */

    /* first end previous .bi if needed */
    if (current_fileno != first_fileno)
    {
      fprintf(as_file, "\t.ei\t\"%s\"\n", CSTRING(fds[current_fileno] ->file));
    }

    /* .bi if not same as original .file */
    if (findex != first_fileno)
    {
      fprintf(as_file, "\n\t.bi\t\"%s\"\n", CSTRING(fds[findex] ->file));
      stabbed = 1;
    }
    else
    {
      /* output a comment to indicate back to .file level */
      fprintf(as_file, "\n#\t.file\t\"%s\"\n", CSTRING(fds[findex] ->file));
    }
  }

  files_stabbed++;

  if (stabbed)
  {
    /* output TDF file machine */
    char *mach = CSTRING(fds[findex] ->machine);
    time_t t = NATINT(fds[findex] ->date);

    /* output machine name if given */
    if (mach != 0 && *mach != '\0')
    {
      fprintf(as_file, "#\tMachine: \"%s\"\n", mach);
    }

    /* output TDF file time (ctime appends extra '\n') */
    if (t != 0)
    {
      fprintf(as_file, "#\tSource file date: %s\n", ctime(&t));
    }
  }
  current_fileno = findex;
  return;
}


/*
 * start of a new lex level
 */
static void diagbr_open(int findex)
{
  stab_file(findex);
  stab_begin_block();
  return;
}

/*
 * end of a lex level
 */
static void diagbr_close(int findex)
{
  stab_file(findex);
  stab_end_block();
  return;
}



/*
 * ============================================================================
 *		Procs to generate type or object related stabs
 */


/*
 * get the next type number to be used as a stabstring TypeId
 */
static int next_typen(void)
{
  static int typeno = 0;	/* types are numbered from 1 */

  return ++typeno;
}


/*
 * return standard TypeNo output by stab_types() for simple shapes
 */
static int TypeNo_of_shape(shape s)
{
  /* tokensh is special to this module, cannot use name(tokensh) */
  if (s == tokensh)
  {
    return TYPEID_TOKEN;
  }

  switch (name(s))
  {
   case bothd:		/*FALLTHROUGH*/
   case tophd:		return TYPEID_VOID;
   case scharhd:	return TYPEID_SCHAR;
   case ucharhd:       	return TYPEID_UCHAR;
   case swordhd:       	return TYPEID_SWORD;
   case uwordhd:       	return TYPEID_UWORD;
   case sizehd:		/*FALLTHROUGH*/
   case slonghd:       	return TYPEID_SLONG;
   case ulonghd:	return TYPEID_ULONG;
   case shrealhd:	return TYPEID_SHREAL;
   case realhd:		return TYPEID_REAL;
   case doublehd:	return TYPEID_DOUBLE;

   default:
    {
      ASSERT(0);		/* fail if debugging */
      return TYPEID_VOID;	/* return something that will work */
    }
    /* NOTREACHED */
  }
}


/*
 * Are two struct/union types structurally equivalent?
 * That is, the same stab TypeDef would be generated.
 */
static bool eq_sutype(diag_type a, diag_type b)
{
  diag_field_list fa;
  diag_field_list fb;
  int j;

  if (a == b)
  {
    return 1;
  }
  if (a->key != b->key)
  {
    return 0;
  }
  if (a->key != DIAG_TYPE_STRUCT && a->key != DIAG_TYPE_UNION)
  {
    return 0;
  }

  if (strcmp(CSTRING(a->data.t_struct.nme), CSTRING(b->data.t_struct.nme)))
  {
    return 0;
  }
  fa = a->data.t_struct.fields;
  fb = b->data.t_struct.fields;
  if (fa->lastused != fb->lastused)
  {
    return 0;
  }
  for (j = fa->lastused - 1; j >= 0; j--)
  {
    diag_field sfa = (fa->array)[j];
    diag_field sfb = (fb->array)[j];

    if (strcmp(CSTRING(sfa->field_name), CSTRING(sfb->field_name)))
    {
      return 0;
    }
  }
  return eq_shape(a->data.t_struct.tdf_shape, b->data.t_struct.tdf_shape);
}
static bool eq_typedef_type(diag_descriptor * a, diag_descriptor * b)
{
  if (a==b)
  {
    return 1;
  }

  if (strcmp(CSTRING(a->data.typ.nme),CSTRING(b->data.typ.nme)) ==0)
  {
    return 1;
  }
  return 0;
}



/*
 * size in bits of object represented by dt, or negative for unknown size
 */
static int size_dt(diag_type dt)
{
  switch (dt->key)
  {
   case DIAG_TYPE_PTR:
    {
      return 32;
    }
   case DIAG_TYPE_ARRAY:
    {
      int stride = EXPINT(dt->data.array.stride);
      int lwb = EXPINT(dt->data.array.lower_b);
      int upb = EXPINT(dt->data.array.upper_b);
      int nelements = upb - lwb + 1;

      if (nelements < 0)
      {
	return 0;		/* avoid negative size from "super-flat" arrays */
      }

      if (stride > 0)
      {
	ASSERT(stride >= size_dt(dt->data.array.element_type));
	return nelements * stride;
      }
      else
      {
	/* stride not expected to be <= 0, but just in case ... */
	return nelements * size_dt(dt->data.array.element_type);
      }
      /* NOTREACHED */
    }
   case DIAG_TYPE_STRUCT:
    {
      return shape_size(dt->data.t_struct.tdf_shape);
    }
   case DIAG_TYPE_UNION:
    {
      return shape_size(dt->data.t_union.tdf_shape);
    }
   case DIAG_TYPE_ENUM:
    {
      return size_dt(dt->data.t_enum.base_type);
    }
   case DIAG_TYPE_VARIETY:
    {
      return shape_size(dt->data.var);
    }
   case DIAG_TYPE_PROC:
    {
      return 32;		/* ptr to proc */
    }
   case DIAG_TYPE_LOC:
    {
      return size_dt(dt->data.loc.object);
    }
   case DIAG_TYPE_FLOAT:
    {
      return shape_size(f_floating(dt->data.f_var));	/* shape for f_var */
    }
   case DIAG_TYPE_NULL:
    {
      return 0;			/* void has no size */
    }
   case DIAG_TYPE_BITFIELD:
    {
      return NATINT(dt->data.bitfield.no_of_bits);
    }
   case DIAG_TYPE_UNINIT:	/* from tokenised type */
   case DIAG_TYPE_INITED:
    {
      return -1;		/* +++ we should try to do better somehow */
    }
   default:
    {
      ASSERT(0);		/* fail if in debug mode */
      return -1;
    }
  }
}


/*
 * output TypeDef, except as a INTEGER TypeId to avoid recursion
 */
static void out_dt_TypeDef_no_recurse(diag_type dt)
{
  switch (dt->key)
  {
   case DIAG_TYPE_PTR:
    {
      /* TypeDef:		* TypeId	"Pointer of type TypeId" */
      fprintf(as_file, "*");
      out_dt_TypeId(dt->data.ptr.object);
      break;
    }
   case DIAG_TYPE_ARRAY:
    {
      int stride = EXPINT(dt->data.array.stride);
      int lwb = EXPINT(dt->data.array.lower_b);
      int upb = EXPINT(dt->data.array.upper_b);
      diag_type index_type = dt->data.array.index_type;
      diag_type element_type = dt->data.array.element_type;

      ASSERT(stride >= size_dt(element_type));

#if 0
      /* +++ maybe this works better thab Packed array with dbx/gdb, try it */
      if (stride == 1 && lwb == 0 && upb < 32)
      {
	/* represent as bitfield */
	fprintf(as_file, "r%d;0;4294967295;", TYPEID_SLONG);
	break;
      }
#endif

      /*
       * TypeDef:		 Array
       * Array:		a TypeDef ; TypeId		"Array"
       *		|	P TypeDef ; TypeId		"Packed array"
       *		|	A TypeId			"Open array of TypeId"
       *	where	TypeDef:	Subrange
       * Subrange:	r TypeId ; Bound ; Bound
       * Bound:		INTEGER				"Constant bound"
       */
      if (stride == 1)
      {
	fprintf(as_file, "Pr");		/* Packed Array - should never happen for C */
      }
      else
      {
	fprintf(as_file, "ar");
      }
      out_dt_TypeId(index_type);
      fprintf(as_file, ";%d;%d;", lwb, upb);
      out_dt_TypeId(element_type);
      break;
    }
   case DIAG_TYPE_STRUCT:
   case DIAG_TYPE_UNION:
    {
      int i;
      char su;
      diag_field_list fields;
      shape s;

      /* TypeDef:	Record */

      if (dt->key == DIAG_TYPE_STRUCT)
      {
	fields = dt->data.t_struct.fields;
	s = dt->data.t_struct.tdf_shape;

	/* Record:	s NumBytes FieldList ;		"Structure or record definition" */
	su = 's';
      }
      else
      {				/* dt->key == DIAG_TYPE_UNION */
	fields = (diag_field_list)dt->data.t_union.fields;
	s = dt->data.t_union.tdf_shape;
	/* Record:	u NumBytes FieldList ;		"Union" */
	su = 'u';
      }

      /* NumBytes:	INTEGER */
      fprintf(as_file, "%c%d", su, shape_size(s) / 8);

      /* FieldList:	Field | FieldList Field */
      for (i = fields->lastused - 1; i >= 0; i--)
      {
	int size;
	diag_field sf = (fields->array)[i];
	int offset = EXPINT(sf->where);

	/* Field:	NAME : TypeId , BitOffset , NumBits ; */
	ASSERT(CSTRING(sf->field_name)[0]!=0);
	fprintf(as_file, "%s:", CSTRING(sf->field_name));
	out_dt_TypeId(sf->field_type);

	size = size_dt(sf->field_type);

#ifdef DO_ASSERT
	/* check object size <= field size */
	if (size > 0)
	{
	  int next_start;
	  int sizetonext;

	  if (dt->key == DIAG_TYPE_UNION || i == 0)
	  {
	    next_start = shape_size(s);
	  }
	  else
	  {
	    next_start = EXPINT(((fields->array)[i - 1]) ->where);
	  }
	  sizetonext = next_start - offset;

	  ASSERT(size <= sizetonext);
	}
#endif

	if (size < 0)		/* eg from tokenised type */
	{
	  /* guess: space to next field, or end */
	  int next_start;

	  if (dt->key == DIAG_TYPE_UNION || i == 0)
	    next_start = shape_size(s);
	  else
	    next_start = EXPINT(((fields->array)[i - 1]) ->where);
	  size = next_start - offset;
	}

	if (size < 0)
	{
	  size = 32;		/* desperate guess */
	}

	/*
	 * BitOffset:	INTEGER		"Offset in bits from beginning of structure"
	 * NumBits:	INTEGER		"Number of bits in item"
	 */
	fprintf(as_file, ",%d,%d;", offset, size);	/* bitoff,bitsz */
      }
      fprintf(as_file, ";");
      break;
    }

  case DIAG_TYPE_ENUM:
    {
#if 1
      /* simply output the base integer type */
      out_dt_TypeDef(dt->data.t_enum.base_type);
#else

      /*
       * +++ currently tdfc (Jan 93) does not generate DIAG_TYPE_ENUM
       * +++ enable and test this when DIAG_TYPE_ENUM is generated
       */

      /*
       * TypeDef:	e EnumList ;		"Enumerated type (default size, 32 bits)"
       * EnumList:	Enum | EnumList Enum
       * Enum:		NAME : OrdValue ,	"Enumerated scalar description"
       * OrdValue:	INTEGER			"Associated numeric value"
       */
      enum_values_list enumvals = dt->data.t_enum.values;
      enum_values enumarr = *(enumvals->array);
      int nvals = enumvals->len;
      int i;

      ASSERT(size_dt(dt->data.t_enum.base_type) == 32);

      fprintf(as_file, "e");
      for (i = 0; i < nvals; i++)
      {
	fprintf(as_file, "%s:%d,", CSTRING(enumarr[i].nme), EXPINT(enumarr[i].val));
      }
      fprintf(as_file, ";");
#endif
      break;
    }

  case DIAG_TYPE_VARIETY:
    {
      int i = TypeNo_of_shape(dt->data.var);

      /*
       * TypeDef:	INTEGER		"Type number of a previously defined type"
       */
      fprintf(as_file, "%d", i);
      SET_OUTED_NO(dt, i);
      break;
    }

  case DIAG_TYPE_PROC:
    {
      diag_type result_type = dt->data.proc.result_type;
      int non;

      /*
       * In general DIAG_TYPE_PROC in C terms means a pointer to a proc:
       * TypeDef:		* TypeId	"Pointer of type TypeId"
       */

      fprintf(as_file, "*");

      /* TypeId:		INTEGER = TypeDef */
      /* we need a new typeno to match the TypeId syntax */
      non = next_typen();
      fprintf(as_file, "%d=", non);

      /*
       * TypeDef:	ProcedureType	"For function types rather than declarations"
       * ProcedureType:	f TypeId ;	"Function returning type TypeId"
       * +++ generate parameter info as well, though IBM stabsyntax says this is for Modula-2:
       * +++ ProcedureType:	f TypeId , NumParams ; TParamList ;
       *				"Function of N parameters returning type TypeId"
       */
      fprintf(as_file, "f");
      out_dt_TypeId(result_type);
      break;
    }

  case DIAG_TYPE_LOC:
    {
      /* simply output type of object */

      /* +++ use data.loc.qualifier.is_const and data.loc.qualifier.is_volatile if useful */
      out_dt_TypeDef(dt->data.loc.object);
      break;
    }

  case DIAG_TYPE_FLOAT:
    {
      /* simply output standard type number */
      int i = TypeNo_of_shape(f_floating(dt->data.f_var));	/* shape for f_var */

      /*
       * TypeDef:		INTEGER		"Type number of a previously defined type"
       */
      fprintf(as_file, "%d", i);
      SET_OUTED_NO(dt, i);
      break;
    }

  case DIAG_TYPE_NULL:
    {

      /*
       * TypeDef:		INTEGER		"Type number of a previously defined type"
       */
      fprintf(as_file, "%d", TYPEID_VOID);	/* use "void" for the null
						 * type */
      SET_OUTED_NO(dt, TYPEID_VOID);
      break;
    }

  case DIAG_TYPE_BITFIELD:
    {
#if 1

      /*
       * Rely on struct field processing to provide the bitfield size,
       * which works very well for C.  Simply output the base type.
       */
      out_dt_TypeDef(dt->data.bitfield.result_type);
#else
      /* +++ this is wrong, TypeId where TypeDef expected, fix and try again */
      /* this breaks gdb 4.9, and dbx ignores TypeAttr */

      /*
       * TypeId:	INTEGER = TypeAttrs TypeDef
       *					"New type with special type attributes"
       * TypeAttrs:	@ TypeAttrList ;	"Any additional information; ignored by dbx"
       * TypeAttrList:	TypeAttrList ; @ TypeAttr | TypeAttr
       * TypeAttr:	s INTEGER		"Size in bits"
       */
      fprintf(as_file, "@s%d;", NATINT(dt->data.bitfield.no_of_bits));
      out_dt_TypeDef(dt->data.bitfield.result_type);
#endif
      break;
    }

  case DIAG_TYPE_UNINIT:	/* from tokenised type */
  case DIAG_TYPE_INITED:
    {

      /*
       * TypeDef:		INTEGER		"Type number of a previously defined type"
       */
      fprintf(as_file, "%d", TYPEID_TOKEN);
      break;
    }
  default:
    {
      /* nothing expected now, but maybe there will be extensions */
      ASSERT(0);		/* fail if in debug mode */
      /* We must output something here to satisfy the syntax */

      /*
       * TypeDef:		INTEGER		"Type number of a previously defined type"
       */
      fprintf(as_file, "%d", TYPEID_VOID);
      break;
    }
  }
}


/*
 * generate TypeDef stabstring
 */
static void out_dt_TypeDef(diag_type dt)
{
  if (IS_OUTED(dt))
  {

    /*
     * TypeDef:		INTEGER		"Type number of a previously defined type"
     */
    fprintf(as_file, "%d", OUTED_NO(dt));
  }
  else
  {
    /* any of the other expansions of TypeDef */
    out_dt_TypeDef_no_recurse(dt);
  }
}


/*
 * generate TypeId stabstring defining new type number
 * TypeId:	INTEGER = TypeDef	"New type number described by TypeDef"
 */
static void out_dt_NewTypeId(diag_type dt, int non)
{
  fprintf(as_file, "%d=", non);

  if (IS_OUTED(dt))
  {
    out_dt_TypeDef(dt);
  }
  else
  {
    /*
     * We have to take care to handle recursive type correctly. Record the new
     * type number so indirect refs (by PTR of PROC) do not lead to infinite
     * recursion.  But we need to avoid simply identifying whole type to
     * itself; out_dt_TypeDef_no_recurse() avoids this problem.
     */
    SET_OUTED_NO(dt, non);	/* record typeno for future use */
    out_dt_TypeDef_no_recurse(dt);
  }
}


/*
 * generate TypeId stabstring
 */
static void out_dt_TypeId(diag_type dt)
{
  if (IS_OUTED(dt))
  {

    /*
     * TypeId:		INTEGER		"Type number of previously defined type"
     */
    fprintf(as_file, "%d", OUTED_NO(dt));
    return;
  }

  switch (dt->key)
  {
  case DIAG_TYPE_VARIETY:
  case DIAG_TYPE_FLOAT:
  case DIAG_TYPE_NULL:
  case DIAG_TYPE_UNINIT:	/* from tokenised type */
  case DIAG_TYPE_INITED:	/* from tokenised type */
    {

      /*
       * Simple types have preallocated TypeId INTEGER, out_dt_TypeDef()
       * or for these cases out_dt_TypeDef_no_recurse() will output it.
       */

      /*
       * TypeId:		INTEGER		"Type number of previously defined type"
       */
      out_dt_TypeDef_no_recurse(dt);	/* for these keys, we assume TypeDef
					 * expands to INTEGER */
      break;
    }

  case DIAG_TYPE_LOC:
    {

      /* look down to object, so maybe avoiding allocating another TypeId INTEGER */
      out_dt_TypeId(dt->data.loc.object);
      break;
    }

  default:
    {
      /* no existing TypeNo that we know of, stabsyntax requires a new one */

      /*
       * TypeId:	INTEGER = TypeDef	"New type number described by TypeDef"
       */
      out_dt_NewTypeId(dt, next_typen());
      break;
    }
  }
}

/*
 * Output .bs (begin static block) which must precede stab_global
 * for tags which have data initialisers.  If such stabs are output
 * outside .bs/.be gdb and dbx cannot locate tag.
 */
void stab_bs(char *sectname)
{
  fprintf(as_file, "\t.bs\t%s\n", sectname);
}

/*
 * Output .es (end static block).
 */
void stab_es(char *sectname)
{
  fprintf(as_file, "\t.es\n");
}


/*
 * Produce diagnostic for ident_tag variable "id" defined by "global";
 * called from translat().  "ext" tells whether "id" is "static".
 */
void stab_global(exp global, char *id, bool ext)
{
#if defined(__AIX) || defined(CROSS_INCLUDE)
  diag_descriptor *dd = find_dd(global);

  if (dd == (diag_descriptor *)0)
    return;

  /* +++ inefficient */
  stab_file(find_file(CSTRING(dd->data.id.whence.file->file)));

  /*
   * Stabstring:	NAME : Class	"Name of object followed by object classification"
   * Class:		Variable	"Variable in program"
   * Variable:		G TypeId	"Global (external) variable of type TypeId"
   *		|	S TypeId	"Module variable of type TypeId (C static global)"
   */
  ASSERT(CSTRING(dd->data.id.nme)[0]!=0);
  fprintf(as_file, "\t.stabx\t\"%s:%c",
	  CSTRING(dd->data.id.nme),
	 (ext ? 'G' : 'S'));
  out_dt_TypeId(dd->data.id.new_type);
  fprintf(as_file, "\",%s,%d,%d\n",
	  id,
	 (ext ? C_GSYM : C_STSYM),
	  0);
#endif
}


/*
 * switch to correct file prior to proc prelude
 */
void stab_proc1(exp proc, char *id, bool ext)
{
  diag_descriptor *dd = find_dd(proc);

  block_depth = 0;

  if (dd == (diag_descriptor *)0)
  {
    COMMENT("stab_proc1: no descriptor");	/* should never happen */
    current_procstart_lineno = NOT_IN_PROC;
    current_lineno = NOT_IN_PROC;
    return;
  }

  current_procstart_lineno = NATINT(dd->data.id.whence.line_no);
  current_lineno = current_procstart_lineno;

  /* +++ inefficient */
  stab_file(find_file(CSTRING(dd->data.id.whence.file->file)));
}


/*
 * stap proc, after label defined
 */
void stab_proc2(exp proc, char *id, bool ext)
{
  diag_descriptor *dd = find_dd(proc);
  char *nm;
  diag_type dt;

  if (dd == (diag_descriptor *)0)
  {
    COMMENT("stab_proc2: no descriptor");	/* should never happen */
    return;
  }

  dt = dd->data.id.new_type;
  nm = CSTRING(dd->data.id.nme);	/* source proc name, id is just the
					 * assembler label */
  ASSERT(nm[0]!=0);

  /* first a .stabx for the proc descriptor */

  /*
   * Stabstring:	NAME : Class
   * Class:		Procedure	"Subprogram declaration"
   * Procedure:		Proc		"Procedure at current scoping level"
   * Proc:		F TypeID	"External function of type TypeID"
   *		|	f TypeID	"Private function of type TypeID"
   *		|	P		"External procedure"
   *		|	Q		"Private procedure"
   */

  /* +++ when gdb understands, maybe use "P" or "Q" for proc returning void */

  fprintf(as_file, "\t.stabx\t\"%s:%c", nm,(ext ? 'F' : 'f'));

  /*
   * The meaning of TypeId is not clear from the stabstring syntax document,
   * dbx and gdb work best if it is the result type.
   */
  switch (dt->key)
  {
  case DIAG_TYPE_PROC:
    {
      out_dt_TypeId(dt->data.proc.result_type);
      break;
    }
  default:
    {
      /* should never happen, but if it does ... */
      fail("stab_proc2: Should never happen");
      out_dt_TypeId(dt);
      break;
    }
  }

#if defined(__AIX) || defined(CROSS_INCLUDE)
  fprintf(as_file, "\",.%s,%d,%d\n", id, C_FUN, 0);
#endif

#if 1

  /*
   * Then a .function for the proc body. The 5th arg (function length) is not
   * documented in assembler manual, but gcc generates it and it enable gdb to
   * trace down stack properly.
   */
  fprintf(as_file, "\t.function\t.%s,.%s,16,044,E.%s-.%s\n", id, id, id, id);
#else
  fprintf(as_file, "\t.function\t.%s,.%s,16,044\n", id, id);
#endif

  /* the proc start line number  */
  fprintf(as_file, "\t%s\t%d\n", bf_stab ,current_procstart_lineno);

  /* now mork line 1 */
  /*stab_relativeline(line_stab);*/
}


/*
 * diagnostics for proc end
 */
void stab_endproc(exp proc, char *id, bool ext)
{
  /* end all open blocks */
  while (block_depth > 0)
    stab_end_block();

  stab_relativeline(ef_stab);
  fprintf(as_file, "E.%s:\n", id);	/* proc end label */

  current_procstart_lineno = NOT_IN_PROC;

  /* output AIX traceback table, see header file sys/debug.h */
#if defined(__AIX) || defined(CROSS_INCLUDE)
  {
    static struct tbtable_short zero_tbtable_short;
    struct tbtable_short tbtable_sht;
    diag_descriptor *dd = find_dd(proc);
    char *nm;
    int i;

    if (dd == (diag_descriptor *)0)
    {
      COMMENT("stab_endproc: no descriptor");	/* should never happen */
      return;

    }

    nm = CSTRING(dd->data.id.nme);	/* source proc name, id is just the
					 * assembler label */
    ASSERT(nm[0]!=0);
    tbtable_sht = zero_tbtable_short;

    /* +++ set up tbtable_sht more fully */

    tbtable_sht.lang = TB_C;	/* lang C +++ */
    tbtable_sht.has_tboff = 1;	/* optional tb_offset always given */
    tbtable_sht.name_present = 1;	/* optional proc name always given */
    tbtable_sht.saves_lr =
      !p_leaf;	/* non-leaf procs store link reg */
    tbtable_sht.stores_bc =
      !p_leaf;	/* non-leaf procs store stack backchain */

    /* number of float regs saved */
    if (p_sfreg_first_save != FR_NO_REG)
      tbtable_sht.fpr_saved = FR_31 + 1 - p_sfreg_first_save;

    /* number of fixed regs saved */
    if (p_sreg_first_save != R_NO_REG)
      tbtable_sht.gpr_saved = R_31 + 1 - p_sreg_first_save;

    /* number of fixed and float params passed in regs */
    tbtable_sht.fixedparms = p_fixed_params;
    tbtable_sht.floatparms = p_float_params;

    tbtable_sht.parmsonstk = 1;	/* -g always stores parameters on stack */

    /* 0 signifies start of traceback table */
    fprintf(as_file, "#\ttraceback table\n");
    fprintf(as_file, "\t.long\t0\n");

    /* tbtable_sht as bytes */
    fprintf(as_file, "\t.byte\t");
    for (i = 0; i < sizeof(tbtable_sht) - 1; i++)
      fprintf(as_file, "%#x,",((unsigned char *)(&tbtable_sht))[i]);
    fprintf(as_file, "%#x\n",((unsigned char *)(&tbtable_sht))[i]);

    /* optional portions of traceback table */

    /* parminfo */
    if (tbtable_sht.fixedparms || tbtable_sht.floatparms)
      fprintf(as_file, "\t.long\t0\n");	/* +++ */

    /* tb_offset */
    fprintf(as_file, "\t.long\tE.%s-.%s\n", id, id);

    /* we never use hand_mask, ctl_info and ctl_info_disp optional components */
    ASSERT(!tbtable_sht.int_hndl);
    ASSERT(!tbtable_sht.has_ctl);

    /* proc name */
    fprintf(as_file, "\t.short\t%d\n",(int)strlen(nm));
    fprintf(as_file, "\t.byte\t\"%s\"\n", nm);

    /* alloca_reg */
    if (tbtable_sht.uses_alloca)
      fprintf(as_file, "\t.byte\t0\n");	/* +++ */

    /* keep program area [PR] word aligned */
    fprintf(as_file, "\t.align\t2\n");
  }
#endif
}


/*
 * Output appropriate info to symbol table to indicate the declaration of
 * a local identifier, nm, defined by id, displaced by disp; findex is the
 * index of the file containing the declaration. The code below gives does
 * not allow for identifiers allocated in registers; in fact, with
 * current translator, none are used by stab_local. I don't even know
 * whether dbx can actually use them.
 */
void stab_local(char *nm, diag_type dt, exp id, int disp, int findex)
{
#if defined(__AIX) || defined(CROSS_INCLUDE)
  FULLCOMMENT3("stab_local: %s disp=%d boff(id).offset=%d",(long)nm, disp, boff(id).offset);
  disp += boff(id).offset;
again:
  if (name(id) == ident_tag)
  {
    FULLCOMMENT2("stab_local ident_tag: %s disp=%d",(long)nm, disp);
    if ((props(id) & defer_bit) == 0)
    {
      if (isparam(id))
      {
	fprintf(as_file, "\t.stabx\t\"%s:p", nm);
	out_dt_TypeId(dt);
	fprintf(as_file, "\",%d,%d,%d\n", disp, C_PSYM, 0);
	return;
      }
      else
      {
	fprintf(as_file, "\t.stabx\t\"%s:", nm);
	out_dt_TypeId(dt);
	fprintf(as_file, "\",%d,%d,%d\n", disp, C_LSYM, 0);
	return;
      }
    }
    else
    {
      exp sn = son(id);
      int d = disp;

      while (sn != nilexp)
      {
	switch (name(sn))
	{
	case name_tag:
	  {
	    disp = d + no(sn);
	    id = son(sn);
	    if (isvar(id))
	      dt = dt->data.ptr.object;
	    goto again;
	  }
	case reff_tag:
	  {
	    d += no(sn);
	    sn = son(sn);
	    break;
	  }
	case cont_tag:
	  {
	    sn = son(sn);
	    break;
	  }
	default:
	  return;
	}
      }
    }
  }
#endif
}


/*
 * Output a .stabx for all internal struct/unions not already processed.
 * This avoids extremely long stabstrings from internal struct/unions being
 * output on the fly for the top level struct/union.
 * It also avoids dbx going recursive printing some types.
 */
static void stab_internal_types(diag_type dt, bool stabthislevel)
{
  if (IS_OUTED(dt))
    return;				/* already been here */

  switch (dt->key)
  {
  case DIAG_TYPE_STRUCT:
  case DIAG_TYPE_UNION:
    {
      int i;
      diag_field_list fields;
      int non;

      if (stabthislevel && !IS_OUTED(dt))
      {
	non = next_typen();
	SET_OUTED_NO(dt, non);		/* record typeno for future use,
					 * use before definition allowed */
      }
      else
      {
	non = 0;
      }

      if (dt->key == DIAG_TYPE_STRUCT)
	fields = dt->data.t_struct.fields;
      else
	fields = (diag_field_list)dt->data.t_union.fields;

      for (i = fields->lastused - 1; i >= 0; i--)
      {
	diag_field sf = (fields->array)[i];

	stab_internal_types(sf->field_type, 1);
      }

      if (non != 0)
      {
	/*
	 * Generate NamedType stabstring:
	 * NamedType:	T TypeId		"Struct, union, or enumeration tag"
	 * TypeId:	INTEGER = TypeDef	"New type number described by TypeDef"
	 */
	fprintf(as_file, "\t.stabx\t\":");	/* Unnamed object
						 * classification */
	fprintf(as_file, "T%d=", non);
	out_dt_TypeDef_no_recurse(dt);

#if defined(__AIX) || defined(CROSS_INCLUDE)
	fprintf(as_file, "\",%d,%d,%d\n", 0, C_DECL, 0);
#endif
      }

      break;
    }

  case DIAG_TYPE_PTR:
    {
      stab_internal_types(dt->data.ptr.object, 1);
      break;
    }

  case DIAG_TYPE_ARRAY:
    {
      stab_internal_types(dt->data.array.index_type, 1);
      stab_internal_types(dt->data.array.element_type, 1);
      break;
    }

  case DIAG_TYPE_PROC:
    {
      stab_internal_types(dt->data.proc.result_type, 1);
      break;
    }

  default:
    {
      /* simple types without internal types */
      break;
    }
  }
}


/*
 * Generate stab for a basic type, for which there is a standard IBM AIX StabNo.
 */
static void stab_basicshape
(shape sha, char *typename, int tdf_typeidnum, int ibm_typeidnum)
{
#if defined(__AIX) || defined(CROSS_INCLUDE)
  int n = next_typen();

  ASSERT(tdf_typeidnum == n);

  fprintf(as_file, "\t.stabx\t\"%s:t%d=%d", typename, tdf_typeidnum, ibm_typeidnum);
  fprintf(as_file, "\",%d,%d,%d\n", 0, C_DECL, 0);
#endif
}


/*
 * Output diagnostic stabs for built in types, and for structs and unions.
 * Information about procedures is ignored for present;
 * it is output by stab_procN().
 *
 * Must be called before any ".stabx" directive attempted.
 */
static void stab_types(void)
{
  /* Numbering and outputing of basicshapes,structs,unions and typedefs */
 (void)number_and_stab_basicshapes();
 (void)number_structs_and_unions();
 (void)number_typedefs();
 (void)stab_structs_and_unions();
 (void)stab_typedefs();
}


