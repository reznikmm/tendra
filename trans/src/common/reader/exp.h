/* $Id$ */

/*
 * Copyright 2011-2014, The TenDRA Project.
 * Copyright 1996, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */

#ifndef READER_EXP_H
#define READER_EXP_H


#include <reader/code.h>

#ifdef NEWDIAGS
#include <newdiag/dg_first.h>
#else
#include <diag/dg_first.h>
#include <diag/diaginfo.h>
#endif

struct dec_t;
struct aldef_t;


/*
 * STRUCTURE REPRESENTING EXPRESSION PROPERTIES
 *
 * This is a bitfield, with different bits representing different properties.
 * The lower bits are used by common code, and the upper bits are used
 * for each architecture to specify its own architecture-specific properties.
 *
 * See <reader/exp.h> for details.
 */
typedef unsigned short prop;


/*
 * UNION REPRESENTING A EXPRESSION CONSTITUENT
 *
 * An expression constituent can be another expression, a string, a
 * numerical value, a floating-point value or an external declaration.
 */
typedef union {
	struct exp_t *e;
	char *str;
#if defined(TRANS_680x0) && !defined(tdf3)
	long l;
	unsigned long ui;
#else
	int l;
	unsigned int ui;
#endif
	float f;
	struct dec_t *glob;

#ifndef NEWDIAGS
	diag_info *d;
#endif

	struct aldef_t *ald;
} expno;


/*
 * STRUCTURE REPRESENTING EXPRESSIONS
 *
 * An expression has a number of constituents given by the sonf, brof,
 * ptrf and numf fields, a shape (which is another expression),
 * a name representing the expression type and a properties field.
 *
 * .lastf serves as an end marker.
 */
struct exp_t {
	/* TODO: rename all fields sans-f */

	expno sonf;
	expno brof;
	expno ptf;
	expno numf;

	struct exp_t *shf;

	prop propsf;

	unsigned char namef;

	unsigned int lastf:1;
	unsigned int park :1;

#ifdef TRANS_HPPA
	unsigned int commuted:1;
#endif

#ifdef TRANS_ALPHA
	char *dfile; /* source .c file (for debugging only) */
#endif

#ifdef NEWDIAGS
	diag_info *diagf;
#endif
};


/*
 * THE EXPRESSION TYPE
 *
 * The type exp is a pointer to the structure given above.  Access to the
 * fields of the structure is by means of the macros given in <reader/exp.h>.
 * XXX: don't typedef pointers
 */
typedef struct exp_t *exp;


/*
 * MAIN COMPONENTS OF AN EXPRESSION
 */

#define son(x)            ((x)->sonf.e)
#define bro(x)            ((x)->brof.e)
#define last(x)           ((x)->lastf)
#define name(x)           ((x)->namef)
#define sh(x)             ((x)->shf)
#define pt(x)             ((x)->ptf.e)
#define props(x)          ((x)->propsf)
#define no(x)             ((x)->numf.l)
#define parked(x)         ((x)->park)

#ifdef NEWDIAGS
#define dgf(x)            ((x)->diagf)
#endif


/*
 * ALTERNATIVE COMPONENTS OF AN EXPRESSION
 */

#define brog(x)           ((x)->brof.glob)
#define nostr(x)          ((x)->numf.str)
#define ptno(x)           ((x)->ptf.l)
#define sonno(x)          ((x)->sonf.l)
#define fno(x)            ((x)->numf.f)
#define uno(x)            ((x)->numf.ui)

#ifndef NEWDIAGS
#define dno(x)            ((x)->numf.d)
#endif


/*
 * MACROS FOR SETTING COMPONENTS OF AN EXPRESSION
 */

#define setbro(x, b)      bro(x) = (b)
#define setsh(x, b)       sh(x) = (b)
#define setson(x, b)      son(x) = (b)
#define setpt(x, b)       pt(x) = (b)
#define setlast(x)        last(x) = 1
#define clearlast(x)      last(x) = 0
#define setname(x, n)     name(x) = (n)
#define setfather(f, s)   setbro(s, f); setlast(s)


/*
 * ERROR HANDLING
 */

#define errhandle(x)       (props(x) & 0x7)
#define optop(x)           ((int) (errhandle(x) <= 2))
#define seterrhandle(x, e) props(x) = (props(x) & ~0x7) | (e)


/*
 * PROPERTIES OF CONSTRUCTS WITH EXCEPTIONS
 */

#define setjmp_dest(x, d)  { setpt(x, d) ; no(son(d))++; }
#define seterr_code(x, d)   props(x) = (d)
#define isov(x)            (props(x) == 0x4)


/*
 * COMPONENTS OF SHAPES
 */

#define shape_size(x)     ((x)->numf.l)
#define al2ul(x)          ((unsigned long) ((x)->al.al_val.al))
#define align_of(x)       ((x)->brof.ald)
#define shape_align(x)    al2ul(align_of (x))
#define al1_of(x)         ((x)->sonf.ald)
#define al1(x)            al2ul(al1_of (x))
#define al2_of(x)         ((x)->ptf.ald)
#define al2(x)            al2ul(al2_of (x))
#define is_signed(x)      ((x)->lastf)


/*
 * CONSTITUENTS OF AN EXPRESSION FOR ROUNDING MODES
 */

#define round_number(x)       (props (x) >> 3)
#define setround_number(x, r) props (x) = (props (x) & 0x7) | ((r) << 3)


/*
 * MACROS FOR MANIPULATING PROPERTIES
 */

#define pset(x, m)        (props(x) |= (prop)  (m))
#define pclr(x, m)        (props(x) &= (prop) ~(m))
#define ptst(x, m)       ((props(x) & (m)) != 0)


/*
 * PROPERTIES OF JUMP RECORD
 */

#define fstack_pos_of(x)      props (x)


/*
 * PROPERTIES OF IDENT
 */

/* is a variable */
#define setvar(x)                 pset(x, 0x01)
#define clearvar(x)               pclr(x, 0x01)
#define isvar(x)                  ptst(x, 0x01)
#define setid(x)                  clearvar(x)

/* declaration in memory */
#define setvis(x)                 pset(x, 0x02)
#define clearvis(x)               pclr(x, 0x02)
#define isvis(x)                  ptst(x, 0x02)

/* declaration visiblity */
#define setenvoff(x)              pset(x, 0x04)
#define clearenvoff(x)            pclr(x, 0x04)
#define isenvoff(x)               ptst(x, 0x04)

/* only operated on by contents and assign operations */
#define setcaonly(x)              pset(x, 0x08)
#define clearcaonly(x)            pclr(x, 0x08)
#define iscaonly(x)               ptst(x, 0x08)

/* recommend the use of a register */
#define setusereg(x)              pset(x, 0x10)
#define clearusereg(x)            pclr(x, 0x10)
#define isusereg(x)               ptst(x, 0x10)

/* is a parameter */
#define setparam(x)               pset(x, 0x20)
#define clearparam(x)             pclr(x, 0x20)
#define isparam(x)                ptst(x, 0x20)

/* global - in this case bro(e) is the index of the declaration */
#define setglob(x)                pset(x, 0x40)
#define clearglob(x)              pclr(x, 0x40)
#define isglob(x)                 ptst(x, 0x40)

/* used during copying */
#define setcopy(x)                pset(x, 0x80)
#define clearcopy(x)              pclr(x, 0x80)
#define copying(x)                ptst(x, 0x80)

/* TODO: renumber to 0x100 */
#ifdef TRANS_SPARC
#define setinlined(x)             pset(x, 0x0400) /* XXX: more than lower eight bits */
#define clearinlined(x)           pclr(x, 0x0400)
#define isinlined(x)              ptst(x, 0x0400)
#else
#define setinlined(x)             pset(x, 0x0200) /* XXX: more than lower eight bits */
#define clearinlined(x)           pclr(x, 0x0200)
#define isinlined(x)              ptst(x, 0x0200)
#endif

/* parameter used for output (needs only to be in memory for 80x86 and 68k) */
#if defined(TRANS_80x86) || (defined(TRANS_680x0) && !defined(tdf3))
#define setoutpar(x)              setvis(x)
#define isoutpar(x)               isvis(x)
#define clearoutpar(x)
#else
#define setoutpar(x)              pset(x, 0x8000)
#define clearoutpar(x)            pclr(x, 0x8000)
#define isoutpar(x)               ptst(x, 0x8000)
#endif


/*
 * ALTERNATIVE USES OF PROPS FIELD: CONSTITUENTS OF AN EXPRESSION FOR NTESTS
 */

/* XXX: horrible */
#if defined(TRANS_680x0) || defined(TRANS_MIPS)
#define test_number(X)            ((int) props(X))
#define settest_number(X, Y)       props(X) = (Y)
#define setntest(X, Y)             props(X) = (Y)
#elif defined(TRANS_80x86) || defined(TRANS_ALPHA)
#define test_number(x)            (ntest) ((x)->propsf & 0x1f)
#define settest_number(x, t)      (x)->propsf = (prop)(((x)->propsf & ~0x1f) | (int) (t))
#elif defined(TRANS_HPPA)
#define test_number(x)            (props(x) & 127)
#define settest_number(x, t)       props(x) = (t)
#define setntest(x, t)             props(x) = (t)
#else
#define test_number(x)            (props(x) & 0xf)
#define settest_number(x, t)       props(x) = ((props(x) & ~0xf)| (t))
#endif


/*
 * PROPERTIES OF MAKE_PROC CONSTRUCTION
 */

#define set_struct_res(x)         pset(x, 0x01)
#define has_struct_res(x)         ptst(x, 0x01)

#define set_loc_address(x)        pset(x, 0x02)
#define loc_address(x)            ptst(x, 0x02)

#define set_proc_has_setjmp(x)    pset(x, 0x04)
#define proc_has_setjmp(x)        ptst(x, 0x04)

#define set_proc_has_alloca(x)    pset(x, 0x08)
#define proc_has_alloca(x)        ptst(x, 0x08)

#define set_proc_has_lv(x)        pset(x, 0x10)
#define proc_has_lv(x)            ptst(x, 0x10)

#define setrecursive(x)           pset(x, 0x20)
#define isrecursive(x)            ptst(x, 0x20)

#define set_proc_uses_crt_env(x)  pset(x, 0x40)
#define proc_uses_crt_env(x)      ptst(x, 0x40)

#define set_proc_uses_external(x) pset(x, 0x80)
#define proc_uses_external(x)     ptst(x, 0x80)


/*
 * PROPERTIES OF SOLVE CONSTRUCT
 */

#define setcrtsolve(x)            props(x) = 0x01


/*
 * PROPERTIES OF REP AND SOLVE DURING COPY
 */

#define set_copying_solve(x)      pset(x, 0x01)
#define clear_copying_solve(x)    pclr(x, 0x01)
#define is_copying_solve(x)       ptst(x, 0x01)


/*
 * PROPERTIES OF MOVE SOME CONSTRUCT
 */

#define setnooverlap(x)           pset(x, 0x01)
#define isnooverlap(x)            ptst(x, 0x01)


/*
 * PROPERTIES OF CONTENTS CONSTRUCT
 */

#define set_propagate(x)          pset(x, 0x01)
#define clear_propagate(x)        pclr(x, 0x01)
#define to_propagate(x)           ptst(x, 0x01)


/*
 * PROPERTIES OF LABST CONSTRUCTION
 */

#define set_loaded_lv(x)          pset(x, 0x10)
#define is_loaded_lv(x)           ptst(x, 0x10)

#ifdef TRANS_680x0
#define setunroll(x)              pset(x, 0x20)
#define clearunroll(x)            pclr(x, 0x20)
#define isunroll(x)               ptst(x, 0x20)
#else
#define setunroll(x)              pset(x, 0x04)
#define clearunroll(x)            pclr(x, 0x04)
#define isunroll(x)               ptst(x, 0x04)
#endif


/*
 * PROPERTIES OF NAME CONSTRUCT
 */

#define setlastuse(x)             pset(x, 0x01)
#define islastuse(x)              ptst(x, 0x01)

#define setloadparam(x)           pset(x, 0x02)
#define isloadparam(x)            ptst(x, 0x02)

#if defined(TRANS_SPARC) || defined(TRANS_HPPA) || defined(TRANS_680x0)
#define setreallyass(x)           /* not used */
#define isreallyass(x)            (0)
#else
#define setreallyass(x)           pset(x, 0x04)
#define isreallyass(x)            ptst(x, 0x04)
#endif

#ifdef NEWDIAGS
#define setisdiaginfo(x)          pset(x, 0x08)
#define isdiaginfo(x)             ptst(x, 0x08)

#define setdiscarded(x)           pset(x, 0x10)
#define isdiscarded(x)            ptst(x, 0x10)
#endif


/*
 * PROPERTIES OF VAL CONSTRUCT
 */

#define setbigval(x)              pset(x, 0x01)
#define clearbigval(x)            pclr(x, 0x01)
#define isbigval(x)               ptst(x, 0x01)

#define setconstovf(x)            pset(x, 0x02)
#define constovf(x)               ptst(x, 0x02)


/*
 * PROPERTIES OF REP CONSTRUCT
 */

#define setunrolled(x)            pset(x, 0x01)
#define isunrolled(x)             ptst(x, 0x01)


/*
 * PROPERTIES OF APPLY CONSTRUCT
 */

#define settoinline(x)            pset(x, 0x01)
#define istoinline(x)             ptst(x, 0x01)

/*
 * PROPERTIES OF ALLOCA CONSTRUCT
 */

#define set_checkalloc(x)         pset(x, 0x01)
#define checkalloc(x)             ptst(x, 0x01)


/*
 * ROUNDING FOR ALIGNMENT
 */

#define rounder(n, a) ((a) == 0 ? n : ((((n) + (a) - 1) / (a)) * (a)))


/*
 * BASIC ALIGNMENTS
 */

#define  align32    32UL
#define  align16    16UL
#define  align8      8UL
#define  align1      1UL

#define  size64      64L
#define  size32      32L
#define  size16      16L
#define  size8        8L

#define  Z            0L


/* New operations for spec 3.1 (previously from extra_expmacs.h) */
/* TODO: rewrite to use ptst() */
#define set_make_procprops(e, p)   props(e) |= ((p) << 8)
#define proc_has_vcallees(e)     ((props(e) & 0x200) != 0)
#define postlude_has_call(e)      (props(e) & 1)
#define call_has_vcallees(e)     ((props(e) & 2)      != 0)
#define call_has_vcallers(e)     ((props(e) & 1)      != 0)
#define proc_has_checkstack(e)   ((props(e) & 0x800)  != 0)
#define proc_has_vcallers(e)     ((props(e) & 0x100)  != 0)
#define proc_has_nolongj(e)      ((props(e) & 0x1000) != 0)
#define call_is_untidy(e)        ((props(e) & 4)      != 0)

#define set_callee(id) setname(son(id), formal_callee_tag)


#endif
