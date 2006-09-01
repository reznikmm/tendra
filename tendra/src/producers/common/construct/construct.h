/*
 * Copyright (c) 2002-2006 The TenDRA Project <http://www.tendra.org/>.
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


#ifndef CONSTRUCT_INCLUDED
#define CONSTRUCT_INCLUDED


/*
    CONSTRUCTOR AND DESTRUCTOR FUNCTION DECLARATIONS

    The routines in this module are concerned with constructors, destructors
    and conversion functions.
*/

extern NAMESPACE ctor_begin(void);
extern EXP ctor_end(NAMESPACE, EXP, int);
extern void ctor_initialise(NAMESPACE, IDENTIFIER, EXP);
extern EXP ctor_none(EXP, EXP *);
extern EXP ctor_postlude(EXP, EXP);
extern EXP copy_ctor(EXP, int);
extern EXP except_postlude(IDENTIFIER);

extern TYPE check_constr(TYPE, IDENTIFIER, NAMESPACE);
extern TYPE check_destr(TYPE, IDENTIFIER, NAMESPACE);
extern TYPE check_conv(TYPE, IDENTIFIER);
extern TYPE inferred_return(TYPE, IDENTIFIER);

extern IDENTIFIER find_operator(CLASS_TYPE, int);
extern IDENTIFIER make_pseudo_destr(IDENTIFIER, BASE_TYPE, IDENTIFIER,
				    BASE_TYPE);
extern CLASS_INFO implicit_decl(CLASS_TYPE, CLASS_INFO, DECL_SPEC);
extern void implicit_defn(IDENTIFIER, int);

extern EXP init_default(TYPE, EXP *, int, int, ERROR *);
extern EXP convert_constr(TYPE, LIST(EXP), ERROR *, unsigned);
extern EXP convert_conv_aux(TYPE, EXP, ERROR *, unsigned);
extern EXP convert_conv(TYPE, EXP, ERROR *, unsigned);
extern EXP convert_gen(unsigned, EXP, ERROR *);
extern EXP apply_constr(IDENTIFIER, LIST(EXP));
extern EXP add_constr_args(EXP, CLASS_TYPE, int);
extern unsigned extra_constr_args(IDENTIFIER, CLASS_TYPE);
extern EXP apply_trivial_func(IDENTIFIER, LIST(EXP));
extern EXP trivial_destr(EXP);
extern int have_constr_expl;
extern int have_conv_expl;


/*
    IMPLICIT CONSTRUCTOR IDENTIFIERS

    These values are used to represent the four member functions which
    may be implicitly declared for a class - the default constructor, the
    copy constructor, the default destructor and the copy assignment
    operator.  A dummy value for explicit constructors is provided.
*/

#define DEFAULT_USR			0
#define DEFAULT_CONSTR			1
#define DEFAULT_COPY			2
#define DEFAULT_DESTR			3
#define DEFAULT_ASSIGN			4
#define DEFAULT_DELETE			5
#define DEFAULT_PRELUDE			6


/*
    EXTRA CONSTRUCT ARGUMENT VALUES

    These values represent the extra argument which are passed to
    constructors and destructors.
*/

#define EXTRA_NONE			0
#define EXTRA_CONSTR			1

#define EXTRA_DELETE			1
#define EXTRA_DESTR			2


#endif
