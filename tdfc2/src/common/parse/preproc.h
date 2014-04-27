/* $Id$ */

/*
 * Copyright 2002-2011, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */


#ifndef PREPROC_INCLUDED
#define PREPROC_INCLUDED


/*
    PREPROCESSING DECLARATIONS

    The routines in this module are concerned with the processing of
    preprocessing directives.  They include preprocess_file which is the
    stand-alone preprocessing action of the program.
*/

extern int read_preproc_dir(int, int);
extern void patch_preproc_dir(PPTOKEN *);
extern int read_include(int, int);
extern unsigned check_macro(HASHID, int);
extern IDENTIFIER make_assert(HASHID, int);
extern void read_ident(int);
extern void read_weak(int);
extern void preprocess_file(void);
extern void start_preproc_if (void);
extern int clear_preproc_if (void);
extern int patch_cond(int, int);


/*
    PREPROCESSING VARIABLES

    These variables are used by the preprocessing routines.
*/

extern int preproc_only;
extern int preproc_space;
extern int pragma_number;
extern int in_preproc_dir;
extern int no_preproc_dir;
extern int in_pragma_dir;
extern int in_hash_if_exp;
extern EXP crt_hash_if_exp;
extern LOCATION preproc_loc;
extern IDENTIFIER token_macro;


/*
    CONDITIONAL COMPILATION STATES

    These values are used to identify the various conditional compilation
    states.  The condition may be true, false, being skipped, or unresolved.
    In addition a conditional compilation may or may not have associated
    '#else' and '#elif' statements.  There is a special value for indicating
    the end of an individual file.
*/

#define PP_FALSE			((unsigned)0x00)
#define PP_TRUE				((unsigned)0x01)
#define PP_PAST				((unsigned)0x02)
#define PP_SKIP				((unsigned)0x03)
#define PP_UNRESOLVED			((unsigned)0x04)
#define PP_COND_MASK			((unsigned)0x0f)
#define PP_HAVE_ELSE			((unsigned)0x10)
#define PP_HAVE_ELIF			((unsigned)0x20)
#define PP_TOKEN			((unsigned)0x40)
#define PP_END				((unsigned)0xff)


#endif