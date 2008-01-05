/*
 * Automatically generated from the files:
 *	syntax.sid
 * and
 *	syntax.act
 * by:
 *	sid
 */

/* BEGINNING OF HEADER */

#line 37 "syntax.act"

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


#include "obj_c/tdf.h"
#include "obj_c/cmd_ops.h"
#include "obj_c/cons_ops.h"
#include "obj_c/info_ops.h"
#include "obj_c/link_ops.h"
#include "obj_c/par_ops.h"
#include "obj_c/sort_ops.h"
#include "obj_c/spec_ops.h"
#include "error.h"
#include "input.h"
#include "lex.h"
#include "syntax.h"
#include "xalloc.h"


/*
    LOCAL TYPE ALIASES

    These definitions give the aliases used for compound types within the
    grammar.
*/

typedef LIST ( CONSTRUCT ) SID_CONS_LIST ;
typedef LIST ( LINKAGE ) SID_LINK_LIST ;
typedef LIST ( PARAMETER ) SID_PARAM_LIST ;


/*
    COMPILATION MODE

    We allow unreached code and switch off the variable analysis in the
    automatically generated sections.
*/

#if FS_TENDRA
#pragma TenDRA begin
#pragma TenDRA variable analysis off
#ifndef OLD_PRODUCER
#pragma TenDRA unreachable code allow
#endif
#endif


#line 88 "syntax.c"

/* BEGINNING OF FUNCTION DECLARATIONS */

static void ZRlink_Hpack(SID_LINK_LIST *);
static void ZRparam_Hlist(SID_PARAM_LIST *);
static void ZRclist_Hitem(void);
static void ZRslist_Hpack(void);
static void ZRedge_Hlist(void);
static void ZRkind_Hitem(LINKAGE *);
static void ZRoption_Hlist(void);
static void ZR139(CONSTRUCT *);
static void ZRold_Hsort_Hname(SORT *);
static void ZR140(CONSTRUCT *);
static void ZR141(CONSTRUCT *);
static void ZRclist_Hlist(void);
static void ZRlink_Hitem(LINKAGE *);
static void ZRslist_Hitem(void);
static void ZRkind_Hlist(SID_LINK_LIST *);
static void ZRnew_Hsort_Hname(SORT *);
static void ZRsort_Hpack(void);
static void ZRany_Hname(string *);
static void ZRlink_Hlist(SID_LINK_LIST *);
static void ZRslist_Hlist(void);
static void ZRsort_Hitem(void);
static void ZRsort_Hlist(void);
static void ZRconstruct_Hpack(SORT, SID_CONS_LIST *);
static void ZRconstruct_Hextra(CONSTRUCT);
static void ZRboundary_Hitem(CONSTRUCT);
static void ZRconstruct_Hitem(SORT, CONSTRUCT *);
static void ZRconstruct_Hlist(SORT, SID_CONS_LIST *);
static void ZRparam_Hpack(SID_PARAM_LIST *);
static void ZRedge_Hpack(void);
extern void read_spec(SPECIFICATION *);
static void ZRoption_Hpack(void);
static void ZRclist_Hpack(void);
static void ZRparam_Hitem(PARAMETER *);
static void ZRedge_Hitem(void);
static void ZRkind_Hpack(SID_LINK_LIST *);
static void ZRnumber_Hlist(CONSTRUCT);
static void ZRoption_Hitem(void);

/* BEGINNING OF STATIC VARIABLES */


/* BEGINNING OF FUNCTION DEFINITIONS */

static void
ZRlink_Hpack(SID_LINK_LIST *ZOp)
{
    SID_LINK_LIST ZIp;

    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	SID_LINK_LIST ZIq;

	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 116 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 27:
		{
		    ZRlink_Hlist (&ZIq);
		    if ((CURRENT_TERMINAL) == 32) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		{
		    /* BEGINNING OF ACTION: null_link */
		    {
#line 295 "syntax.act"

    (ZIq) = NULL_list ( LINKAGE ) ;
#line 172 "syntax.c"
		    }
		    /* END OF ACTION: null_link */
		}
		break;
	    }
	}
	/* END OF INLINE: 116 */
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZIp = ZIq;
    }
    goto ZL0;
  ZL1:;
    SAVE_LEXER (32);
    return;
  ZL0:;
    *ZOp = ZIp;
}

static void
ZRparam_Hlist(SID_PARAM_LIST *ZOp)
{
    SID_PARAM_LIST ZIp;

    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	PARAMETER ZIa;
	SID_PARAM_LIST ZIq;

	ZRparam_Hitem (&ZIa);
	/* BEGINNING OF INLINE: 59 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 29:
		{
		    ADVANCE_LEXER;
		    ZRparam_Hlist (&ZIq);
		    if ((CURRENT_TERMINAL) == 32) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		{
		    /* BEGINNING OF ACTION: null_param */
		    {
#line 202 "syntax.act"

    (ZIq) = NULL_list ( PARAMETER ) ;
#line 230 "syntax.c"
		    }
		    /* END OF ACTION: null_param */
		}
		break;
	      case 32:
		RESTORE_LEXER;
		goto ZL1;
	    }
	}
	/* END OF INLINE: 59 */
	/* BEGINNING OF ACTION: cons_param */
	{
#line 206 "syntax.act"

    CONS_par ( (ZIa), (ZIq), (ZIp) ) ;
#line 246 "syntax.c"
	}
	/* END OF ACTION: cons_param */
    }
    goto ZL0;
  ZL1:;
    SAVE_LEXER (32);
    return;
  ZL0:;
    *ZOp = ZIp;
}

static void
ZRclist_Hitem(void)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	SORT ZIs;

	ZRnew_Hsort_Hname (&ZIs);
	if ((CURRENT_TERMINAL) == 32) {
	    RESTORE_LEXER;
	    goto ZL1;
	}
	/* BEGINNING OF ACTION: make_clist */
	{
#line 261 "syntax.act"

    compound_sort ( (ZIs), "_list", info_clist_tag, '*' ) ;
#line 277 "syntax.c"
	}
	/* END OF ACTION: make_clist */
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRslist_Hpack(void)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 101 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 0:
		{
		    ZRslist_Hlist ();
		    if ((CURRENT_TERMINAL) == 32) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		break;
	    }
	}
	/* END OF INLINE: 101 */
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRedge_Hlist(void)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
  ZL2_edge_Hlist:;
    {
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRedge_Hitem ();
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 127 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 29:
		{
		    ADVANCE_LEXER;
		    /* BEGINNING OF INLINE: edge-list */
		    goto ZL2_edge_Hlist;
		    /* END OF INLINE: edge-list */
		}
		/*UNREACHED*/
	      default:
		break;
	    }
	}
	/* END OF INLINE: 127 */
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRkind_Hitem(LINKAGE *ZOa)
{
    LINKAGE ZIa;

    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	SORT ZIs;
	string ZIe;

	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 25:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRold_Hsort_Hname (&ZIs);
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 26:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRany_Hname (&ZIe);
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF ACTION: make_unit_link */
	{
#line 290 "syntax.act"

    MAKE_link_basic ( (ZIe), (ZIs), (ZIa) ) ;
    COPY_string ( sort_unit ( (ZIs) ), (ZIe) ) ;
#line 470 "syntax.c"
	}
	/* END OF ACTION: make_unit_link */
    }
    goto ZL0;
  ZL1:;
    SAVE_LEXER (32);
    return;
  ZL0:;
    *ZOa = ZIa;
}

static void
ZRoption_Hlist(void)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
  ZL2_option_Hlist:;
    {
	ZRoption_Hitem ();
	/* BEGINNING OF INLINE: 105 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 29:
		{
		    ADVANCE_LEXER;
		    /* BEGINNING OF INLINE: option-list */
		    goto ZL2_option_Hlist;
		    /* END OF INLINE: option-list */
		}
		/*UNREACHED*/
	      case 32:
		RESTORE_LEXER;
		goto ZL1;
	      default:
		break;
	    }
	}
	/* END OF INLINE: 105 */
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZR139(CONSTRUCT *ZIc)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZR140 (ZIc);
	if ((CURRENT_TERMINAL) == 32) {
	    RESTORE_LEXER;
	    goto ZL1;
	}
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRold_Hsort_Hname(SORT *ZOs)
{
    SORT ZIs;

    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	string ZIa;

	switch (CURRENT_TERMINAL) {
	  case 0:
	    /* BEGINNING OF EXTRACT: name */
	    {
#line 158 "syntax.act"

    ZIa = xstrcpy ( token_buff ) ;
#line 561 "syntax.c"
	    }
	    /* END OF EXTRACT: name */
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF ACTION: find_old_sort */
	{
#line 253 "syntax.act"

    (ZIs) = find_sort ( (ZIa), 0 ) ;
#line 574 "syntax.c"
	}
	/* END OF ACTION: find_old_sort */
    }
    goto ZL0;
  ZL1:;
    SAVE_LEXER (32);
    return;
  ZL0:;
    *ZOs = ZIs;
}

static void
ZR140(CONSTRUCT *ZIc)
{
    switch (CURRENT_TERMINAL) {
      case 3:
	{
	    ADVANCE_LEXER;
	    switch (CURRENT_TERMINAL) {
	      case 29:
		break;
	      default:
		goto ZL1;
	    }
	    ADVANCE_LEXER;
	    switch (CURRENT_TERMINAL) {
	      case 27:
		break;
	      default:
		goto ZL1;
	    }
	    ADVANCE_LEXER;
	    ZRnumber_Hlist (*ZIc);
	    switch (CURRENT_TERMINAL) {
	      case 28:
		break;
	      case 32:
		RESTORE_LEXER;
		goto ZL1;
	      default:
		goto ZL1;
	    }
	    ADVANCE_LEXER;
	    switch (CURRENT_TERMINAL) {
	      case 28:
		break;
	      default:
		goto ZL1;
	    }
	    ADVANCE_LEXER;
	}
	break;
      case 2:
	{
	    unsigned ZIa;

	    ADVANCE_LEXER;
	    switch (CURRENT_TERMINAL) {
	      case 29:
		break;
	      default:
		goto ZL1;
	    }
	    ADVANCE_LEXER;
	    switch (CURRENT_TERMINAL) {
	      case 1:
		/* BEGINNING OF EXTRACT: number */
		{
#line 170 "syntax.act"

    ZIa = token_value ;
#line 646 "syntax.c"
		}
		/* END OF EXTRACT: number */
		break;
	      default:
		goto ZL1;
	    }
	    ADVANCE_LEXER;
	    switch (CURRENT_TERMINAL) {
	      case 28:
		break;
	      default:
		goto ZL1;
	    }
	    ADVANCE_LEXER;
	    /* BEGINNING OF ACTION: set_break */
	    {
#line 210 "syntax.act"

    PARAMETER p = find_param ( (*ZIc), (ZIa) ) ;
    if ( !IS_NULL_par ( p ) ) COPY_int ( par_brk ( p ), 1 ) ;
#line 667 "syntax.c"
	    }
	    /* END OF ACTION: set_break */
	    ZR141 (ZIc);
	    if ((CURRENT_TERMINAL) == 32) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	}
	break;
      case 32:
	return;
      default:
	goto ZL1;
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZR141(CONSTRUCT *ZIc)
{
    switch (CURRENT_TERMINAL) {
      case 29:
	{
	    ADVANCE_LEXER;
	    ZRboundary_Hitem (*ZIc);
	    if ((CURRENT_TERMINAL) == 32) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	}
	break;
      case 32:
	return;
      default:
	break;
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRclist_Hlist(void)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
  ZL2_clist_Hlist:;
    {
	ZRclist_Hitem ();
	/* BEGINNING OF INLINE: 93 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 29:
		{
		    ADVANCE_LEXER;
		    /* BEGINNING OF INLINE: clist-list */
		    goto ZL2_clist_Hlist;
		    /* END OF INLINE: clist-list */
		}
		/*UNREACHED*/
	      case 32:
		RESTORE_LEXER;
		goto ZL1;
	      default:
		break;
	    }
	}
	/* END OF INLINE: 93 */
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRlink_Hitem(LINKAGE *ZOa)
{
    LINKAGE ZIa;

    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	SORT ZIs;
	string ZIe;

	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 11:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRold_Hsort_Hname (&ZIs);
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 10:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRany_Hname (&ZIe);
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF ACTION: make_edge_link */
	{
#line 285 "syntax.act"

    MAKE_link_basic ( (ZIe), (ZIs), (ZIa) ) ;
    COPY_string ( sort_link ( (ZIs) ), (ZIe) ) ;
#line 837 "syntax.c"
	}
	/* END OF ACTION: make_edge_link */
    }
    goto ZL0;
  ZL1:;
    SAVE_LEXER (32);
    return;
  ZL0:;
    *ZOa = ZIa;
}

static void
ZRslist_Hitem(void)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	SORT ZIs;

	ZRnew_Hsort_Hname (&ZIs);
	if ((CURRENT_TERMINAL) == 32) {
	    RESTORE_LEXER;
	    goto ZL1;
	}
	/* BEGINNING OF ACTION: make_slist */
	{
#line 265 "syntax.act"

    compound_sort ( (ZIs), "_list", info_slist_tag, '%' ) ;
#line 868 "syntax.c"
	}
	/* END OF ACTION: make_slist */
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRkind_Hlist(SID_LINK_LIST *ZOp)
{
    SID_LINK_LIST ZIp;

    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	LINKAGE ZIa;
	SID_LINK_LIST ZIq;

	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRkind_Hitem (&ZIa);
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 120 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 29:
		{
		    ADVANCE_LEXER;
		    ZRkind_Hlist (&ZIq);
		    if ((CURRENT_TERMINAL) == 32) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		{
		    /* BEGINNING OF ACTION: null_link */
		    {
#line 295 "syntax.act"

    (ZIq) = NULL_list ( LINKAGE ) ;
#line 928 "syntax.c"
		    }
		    /* END OF ACTION: null_link */
		}
		break;
	    }
	}
	/* END OF INLINE: 120 */
	/* BEGINNING OF ACTION: cons_link */
	{
#line 299 "syntax.act"

    CONS_link ( (ZIa), (ZIq), (ZIp) ) ;
#line 941 "syntax.c"
	}
	/* END OF ACTION: cons_link */
    }
    goto ZL0;
  ZL1:;
    SAVE_LEXER (32);
    return;
  ZL0:;
    *ZOp = ZIp;
}

static void
ZRnew_Hsort_Hname(SORT *ZOs)
{
    SORT ZIs;

    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	string ZIa;

	switch (CURRENT_TERMINAL) {
	  case 0:
	    /* BEGINNING OF EXTRACT: name */
	    {
#line 158 "syntax.act"

    ZIa = xstrcpy ( token_buff ) ;
#line 971 "syntax.c"
	    }
	    /* END OF EXTRACT: name */
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF ACTION: find_new_sort */
	{
#line 249 "syntax.act"

    (ZIs) = find_sort ( (ZIa), 1 ) ;
#line 984 "syntax.c"
	}
	/* END OF ACTION: find_new_sort */
    }
    goto ZL0;
  ZL1:;
    SAVE_LEXER (32);
    return;
  ZL0:;
    *ZOs = ZIs;
}

static void
ZRsort_Hpack(void)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 89 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 27:
		{
		    ZRsort_Hlist ();
		    if ((CURRENT_TERMINAL) == 32) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		break;
	    }
	}
	/* END OF INLINE: 89 */
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRany_Hname(string *ZOn)
{
    string ZIn;

    switch (CURRENT_TERMINAL) {
      case 0:
	{
	    /* BEGINNING OF EXTRACT: name */
	    {
#line 158 "syntax.act"

    ZIn = xstrcpy ( token_buff ) ;
#line 1054 "syntax.c"
	    }
	    /* END OF EXTRACT: name */
	    ADVANCE_LEXER;
	}
	break;
      case 2: case 3: case 4: case 5: case 6:
      case 7: case 8: case 9: case 10: case 11:
      case 12: case 13: case 14: case 15: case 16:
      case 17: case 18: case 19: case 20: case 21:
      case 22: case 23: case 24: case 25: case 26:
	{
	    /* BEGINNING OF ACTION: keyword_name */
	    {
#line 185 "syntax.act"

    (ZIn) = xstrcpy ( token_buff ) ;
#line 1071 "syntax.c"
	    }
	    /* END OF ACTION: keyword_name */
	    /* BEGINNING OF INLINE: 46 */
	    {
		switch (CURRENT_TERMINAL) {
		  case 3:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 2:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 5:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 4:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 6:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 7:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 8:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 9:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 10:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 11:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 12:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 13:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 14:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 15:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 16:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 17:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 18:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 19:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 20:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 21:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 22:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 23:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 24:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 25:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  case 26:
		    {
			ADVANCE_LEXER;
		    }
		    break;
		  default:
		    goto ZL1;
		}
	    }
	    /* END OF INLINE: 46 */
	}
	break;
      case 32:
	return;
      default:
	goto ZL1;
    }
    goto ZL0;
  ZL1:;
    SAVE_LEXER (32);
    return;
  ZL0:;
    *ZOn = ZIn;
}

static void
ZRlink_Hlist(SID_LINK_LIST *ZOp)
{
    SID_LINK_LIST ZIp;

    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	LINKAGE ZIa;
	SID_LINK_LIST ZIq;

	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRlink_Hitem (&ZIa);
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 114 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 29:
		{
		    ADVANCE_LEXER;
		    ZRlink_Hlist (&ZIq);
		    if ((CURRENT_TERMINAL) == 32) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		{
		    /* BEGINNING OF ACTION: null_link */
		    {
#line 295 "syntax.act"

    (ZIq) = NULL_list ( LINKAGE ) ;
#line 1272 "syntax.c"
		    }
		    /* END OF ACTION: null_link */
		}
		break;
	    }
	}
	/* END OF INLINE: 114 */
	/* BEGINNING OF ACTION: cons_link */
	{
#line 299 "syntax.act"

    CONS_link ( (ZIa), (ZIq), (ZIp) ) ;
#line 1285 "syntax.c"
	}
	/* END OF ACTION: cons_link */
    }
    goto ZL0;
  ZL1:;
    SAVE_LEXER (32);
    return;
  ZL0:;
    *ZOp = ZIp;
}

static void
ZRslist_Hlist(void)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
  ZL2_slist_Hlist:;
    {
	ZRslist_Hitem ();
	/* BEGINNING OF INLINE: 99 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 29:
		{
		    ADVANCE_LEXER;
		    /* BEGINNING OF INLINE: slist-list */
		    goto ZL2_slist_Hlist;
		    /* END OF INLINE: slist-list */
		}
		/*UNREACHED*/
	      case 32:
		RESTORE_LEXER;
		goto ZL1;
	      default:
		break;
	    }
	}
	/* END OF INLINE: 99 */
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRsort_Hitem(void)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	SORT ZIs;
	unsigned ZIb;
	unsigned ZIe;
	SID_CONS_LIST ZIp;

	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 23:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRnew_Hsort_Hname (&ZIs);
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 9:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 1:
	    /* BEGINNING OF EXTRACT: number */
	    {
#line 170 "syntax.act"

    ZIb = token_value ;
#line 1411 "syntax.c"
	    }
	    /* END OF EXTRACT: number */
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 13:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 1:
	    /* BEGINNING OF EXTRACT: number */
	    {
#line 170 "syntax.act"

    ZIe = token_value ;
#line 1461 "syntax.c"
	    }
	    /* END OF EXTRACT: number */
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 4:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRconstruct_Hpack (ZIs, &ZIp);
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF ACTION: make_sort */
	{
#line 257 "syntax.act"

    basic_sort ( (ZIs), (ZIb), (ZIe), (ZIp) ) ;
#line 1520 "syntax.c"
	}
	/* END OF ACTION: make_sort */
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRsort_Hlist(void)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
  ZL2_sort_Hlist:;
    {
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRsort_Hitem ();
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 87 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 29:
		{
		    ADVANCE_LEXER;
		    /* BEGINNING OF INLINE: sort-list */
		    goto ZL2_sort_Hlist;
		    /* END OF INLINE: sort-list */
		}
		/*UNREACHED*/
	      default:
		break;
	    }
	}
	/* END OF INLINE: 87 */
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRconstruct_Hpack(SORT ZIs, SID_CONS_LIST *ZOp)
{
    SID_CONS_LIST ZIp;

    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	SID_CONS_LIST ZIq;

	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 81 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 27:
		{
		    ZRconstruct_Hlist (ZIs, &ZIq);
		    if ((CURRENT_TERMINAL) == 32) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		{
		    /* BEGINNING OF ACTION: null_cons */
		    {
#line 234 "syntax.act"

    (ZIq) = NULL_list ( CONSTRUCT ) ;
#line 1616 "syntax.c"
		    }
		    /* END OF ACTION: null_cons */
		}
		break;
	    }
	}
	/* END OF INLINE: 81 */
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZIp = ZIq;
    }
    goto ZL0;
  ZL1:;
    SAVE_LEXER (32);
    return;
  ZL0:;
    *ZOp = ZIp;
}

static void
ZRconstruct_Hextra(CONSTRUCT ZIc)
{
    switch (CURRENT_TERMINAL) {
      case 29:
	{
	    ADVANCE_LEXER;
	    ZR139 (&ZIc);
	    if ((CURRENT_TERMINAL) == 32) {
		RESTORE_LEXER;
		goto ZL1;
	    }
	}
	break;
      case 32:
	return;
      default:
	break;
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRboundary_Hitem(CONSTRUCT ZIc)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 3:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRnumber_Hlist (ZIc);
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRconstruct_Hitem(SORT ZIs, CONSTRUCT *ZOc)
{
    CONSTRUCT ZIc;

    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	string ZIn;
	unsigned ZIe;
	SORT ZIr;
	SID_PARAM_LIST ZIp;

	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 5:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 0:
	    /* BEGINNING OF EXTRACT: name */
	    {
#line 158 "syntax.act"

    ZIn = xstrcpy ( token_buff ) ;
#line 1768 "syntax.c"
	    }
	    /* END OF EXTRACT: name */
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 8:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 1:
	    /* BEGINNING OF EXTRACT: number */
	    {
#line 170 "syntax.act"

    ZIe = token_value ;
#line 1818 "syntax.c"
	    }
	    /* END OF EXTRACT: number */
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 21:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRnew_Hsort_Hname (&ZIr);
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 20:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRparam_Hpack (&ZIp);
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF ACTION: make_cons */
	{
#line 227 "syntax.act"

    if ( !EQ_sort ( (ZIr), (ZIs) ) ) {
	error ( ERROR_SERIOUS, "Wrong result sort for '%s'", (ZIn) ) ;
    }
    (ZIc) = make_construct ( (ZIn), (ZIe), (ZIs), (ZIp) ) ;
#line 1919 "syntax.c"
	}
	/* END OF ACTION: make_cons */
	ZRconstruct_Hextra (ZIc);
	if ((CURRENT_TERMINAL) == 32) {
	    RESTORE_LEXER;
	    goto ZL1;
	}
    }
    goto ZL0;
  ZL1:;
    SAVE_LEXER (32);
    return;
  ZL0:;
    *ZOc = ZIc;
}

static void
ZRconstruct_Hlist(SORT ZIs, SID_CONS_LIST *ZOp)
{
    SID_CONS_LIST ZIp;

    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	CONSTRUCT ZIc;
	SID_CONS_LIST ZIq;

	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRconstruct_Hitem (ZIs, &ZIc);
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 79 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 29:
		{
		    ADVANCE_LEXER;
		    ZRconstruct_Hlist (ZIs, &ZIq);
		    if ((CURRENT_TERMINAL) == 32) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		{
		    /* BEGINNING OF ACTION: null_cons */
		    {
#line 234 "syntax.act"

    (ZIq) = NULL_list ( CONSTRUCT ) ;
#line 1986 "syntax.c"
		    }
		    /* END OF ACTION: null_cons */
		}
		break;
	    }
	}
	/* END OF INLINE: 79 */
	/* BEGINNING OF ACTION: cons_cons */
	{
#line 238 "syntax.act"

    CONS_cons ( (ZIc), (ZIq), (ZIp) ) ;
#line 1999 "syntax.c"
	}
	/* END OF ACTION: cons_cons */
    }
    goto ZL0;
  ZL1:;
    SAVE_LEXER (32);
    return;
  ZL0:;
    *ZOp = ZIp;
}

static void
ZRparam_Hpack(SID_PARAM_LIST *ZOp)
{
    SID_PARAM_LIST ZIp;

    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	SID_PARAM_LIST ZIq;

	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 62 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 27:
		{
		    ZRparam_Hlist (&ZIq);
		    if ((CURRENT_TERMINAL) == 32) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		{
		    /* BEGINNING OF ACTION: null_param */
		    {
#line 202 "syntax.act"

    (ZIq) = NULL_list ( PARAMETER ) ;
#line 2048 "syntax.c"
		    }
		    /* END OF ACTION: null_param */
		}
		break;
	    }
	}
	/* END OF INLINE: 62 */
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZIp = ZIq;
    }
    goto ZL0;
  ZL1:;
    SAVE_LEXER (32);
    return;
  ZL0:;
    *ZOp = ZIp;
}

static void
ZRedge_Hpack(void)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 129 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 27:
		{
		    ZRedge_Hlist ();
		    if ((CURRENT_TERMINAL) == 32) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		break;
	    }
	}
	/* END OF INLINE: 129 */
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

void
read_spec(SPECIFICATION *ZOspec)
{
    SPECIFICATION ZIspec;

    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	SID_LINK_LIST ZIp;
	SID_LINK_LIST ZIq;
	unsigned ZIv1;
	unsigned ZIv2;

	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 24:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRsort_Hpack ();
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 16:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRclist_Hpack ();
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 22:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRslist_Hpack ();
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 19:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRoption_Hpack ();
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 15:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRlink_Hpack (&ZIp);
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 14:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRkind_Hpack (&ZIq);
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 12:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRedge_Hpack ();
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 17:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 1:
	    /* BEGINNING OF EXTRACT: number */
	    {
#line 170 "syntax.act"

    ZIv1 = token_value ;
#line 2440 "syntax.c"
	    }
	    /* END OF EXTRACT: number */
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 18:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 1:
	    /* BEGINNING OF EXTRACT: number */
	    {
#line 170 "syntax.act"

    ZIv2 = token_value ;
#line 2490 "syntax.c"
	    }
	    /* END OF EXTRACT: number */
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 30:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF ACTION: make_spec */
	{
#line 310 "syntax.act"

    LIST ( SORT ) p = check_sorts () ;
    LIST ( LINKAGE ) q = foreign_sorts () ;
    MAKE_spec_basic ( (ZIv1), (ZIv2), p, (ZIp), (ZIq), q, (ZIspec) ) ;
#line 2526 "syntax.c"
	}
	/* END OF ACTION: make_spec */
    }
    goto ZL0;
  ZL1:;
    {
	/* BEGINNING OF ACTION: syntax_error */
	{
#line 327 "syntax.act"

    error ( ERROR_SERIOUS, "Syntax error" ) ;
#line 2538 "syntax.c"
	}
	/* END OF ACTION: syntax_error */
	/* BEGINNING OF ACTION: null_spec */
	{
#line 316 "syntax.act"

    (ZIspec) = NULL_spec ;
#line 2546 "syntax.c"
	}
	/* END OF ACTION: null_spec */
    }
  ZL0:;
    *ZOspec = ZIspec;
}

static void
ZRoption_Hpack(void)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 107 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 0:
		{
		    ZRoption_Hlist ();
		    if ((CURRENT_TERMINAL) == 32) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		break;
	    }
	}
	/* END OF INLINE: 107 */
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRclist_Hpack(void)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 95 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 0:
		{
		    ZRclist_Hlist ();
		    if ((CURRENT_TERMINAL) == 32) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		break;
	    }
	}
	/* END OF INLINE: 95 */
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRparam_Hitem(PARAMETER *ZOa)
{
    PARAMETER ZIa;

    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	string ZIn;
	SORT ZIs;

	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRany_Hname (&ZIn);
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRnew_Hsort_Hname (&ZIs);
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF ACTION: make_param */
	{
#line 196 "syntax.act"

    int intro = 0 ;
    if ( ends_in ( (ZIn), "_intro" ) ) intro = 1 ;
    MAKE_par_basic ( (ZIn), (ZIs), 0, 0, intro, (ZIa) ) ;
#line 2692 "syntax.c"
	}
	/* END OF ACTION: make_param */
    }
    goto ZL0;
  ZL1:;
    SAVE_LEXER (32);
    return;
  ZL0:;
    *ZOa = ZIa;
}

static void
ZRedge_Hitem(void)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	SORT ZIs;
	string ZIc;

	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 7:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZRold_Hsort_Hname (&ZIs);
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  case 32:
	    RESTORE_LEXER;
	    goto ZL1;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 6:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 29:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 0:
	    /* BEGINNING OF EXTRACT: name */
	    {
#line 158 "syntax.act"

    ZIc = xstrcpy ( token_buff ) ;
#line 2781 "syntax.c"
	    }
	    /* END OF EXTRACT: name */
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF ACTION: set_edge */
	{
#line 273 "syntax.act"

    set_special ( (ZIs), (ZIc), KIND_edge ) ;
    COPY_int ( sort_edge ( (ZIs) ), 1 ) ;
#line 2802 "syntax.c"
	}
	/* END OF ACTION: set_edge */
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRkind_Hpack(SID_LINK_LIST *ZOp)
{
    SID_LINK_LIST ZIp;

    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	SID_LINK_LIST ZIq;

	switch (CURRENT_TERMINAL) {
	  case 27:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF INLINE: 122 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 27:
		{
		    ZRkind_Hlist (&ZIq);
		    if ((CURRENT_TERMINAL) == 32) {
			RESTORE_LEXER;
			goto ZL1;
		    }
		}
		break;
	      default:
		{
		    /* BEGINNING OF ACTION: null_link */
		    {
#line 295 "syntax.act"

    (ZIq) = NULL_list ( LINKAGE ) ;
#line 2849 "syntax.c"
		    }
		    /* END OF ACTION: null_link */
		}
		break;
	    }
	}
	/* END OF INLINE: 122 */
	switch (CURRENT_TERMINAL) {
	  case 28:
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	ZIp = ZIq;
    }
    goto ZL0;
  ZL1:;
    SAVE_LEXER (32);
    return;
  ZL0:;
    *ZOp = ZIp;
}

static void
ZRnumber_Hlist(CONSTRUCT ZIc)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
  ZL2_number_Hlist:;
    {
	unsigned ZIa;

	switch (CURRENT_TERMINAL) {
	  case 1:
	    /* BEGINNING OF EXTRACT: number */
	    {
#line 170 "syntax.act"

    ZIa = token_value ;
#line 2891 "syntax.c"
	    }
	    /* END OF EXTRACT: number */
	    break;
	  default:
	    goto ZL1;
	}
	ADVANCE_LEXER;
	/* BEGINNING OF ACTION: set_boundary */
	{
#line 215 "syntax.act"

    PARAMETER p = find_param ( (ZIc), (ZIa) ) ;
    if ( !IS_NULL_par ( p ) ) COPY_int ( par_align ( p ), 1 ) ;
#line 2905 "syntax.c"
	}
	/* END OF ACTION: set_boundary */
	/* BEGINNING OF INLINE: 69 */
	{
	    switch (CURRENT_TERMINAL) {
	      case 29:
		{
		    ADVANCE_LEXER;
		    /* BEGINNING OF INLINE: number-list */
		    goto ZL2_number_Hlist;
		    /* END OF INLINE: number-list */
		}
		/*UNREACHED*/
	      default:
		break;
	    }
	}
	/* END OF INLINE: 69 */
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

static void
ZRoption_Hitem(void)
{
    if ((CURRENT_TERMINAL) == 32) {
	return;
    }
    {
	SORT ZIs;

	ZRnew_Hsort_Hname (&ZIs);
	if ((CURRENT_TERMINAL) == 32) {
	    RESTORE_LEXER;
	    goto ZL1;
	}
	/* BEGINNING OF ACTION: make_option */
	{
#line 269 "syntax.act"

    compound_sort ( (ZIs), "_option", info_option_tag, '?' ) ;
#line 2950 "syntax.c"
	}
	/* END OF ACTION: make_option */
    }
    return;
  ZL1:;
    SAVE_LEXER (32);
    return;
}

/* BEGINNING OF TRAILER */

#line 332 "syntax.act"

#line 2964 "syntax.c"

/* END OF FILE */