/*
 * Copyright (c) 2003, The Tendra Project <http://www.ten15.org/>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 *    		 Crown Copyright (c) 1997
 *
 *    This TenDRA(r) Computer Program is subject to Copyright
 *    owned by the United Kingdom Secretary of State for Defence
 *    acting through the Defence Evaluation and Research Agency
 *    (DERA).  It is made available to Recipients with a
 *    royalty-free licence for its use, reproduction, transfer
 *    to other parties and amendment for any purpose not excluding
 *    product development provided that any such use et cetera
 *    shall be deemed to be acceptance of the following conditions:-
 *
 *        (1) Its Recipients shall ensure that this Notice is
 *        reproduced upon any copies or amended versions of it;
 *
 *        (2) Any amended version of it shall be clearly marked to
 *        show both the nature of and the organisation responsible
 *        for the relevant amendment or amendments;
 *
 *        (3) Its onward transfer from a recipient to another
 *        party shall be deemed to be that party's acceptance of
 *        these conditions;
 *
 *        (4) DERA gives no warranty or assurance as to its
 *        quality or suitability for any purpose and DERA accepts
 *        no liability whatsoever in relation to any use to which
 *        it may be put.
 *
 * $TenDRA$
 */

#ifndef _STDARG_H
#define _STDARG_H


#ifndef __BUILDING_TDF_ANSI_STDARG_H_VA_ARGS


/*
    DEFINITION OF VA_LIST

    The definition of va_list is copied from the system header.
*/

#ifndef _VA_LIST
#define _VA_LIST
#ifndef _VA_LIST_DECLARED
#define _VA_LIST_DECLARED
typedef char *va_list ;
#endif
#endif



/*
    TOKEN DECLARATIONS

    The tokens __va_t, __va_start_ten15, va_arg and va_end are as in the
    ansi:stdarg header.  The only difference is that va_list is defined
    as its implementation type, rather than being a tokenised type.
*/

#pragma token TYPE __va_t # ~__va_t
#pragma token PROC ( EXP rvalue : __va_t : ) EXP rvalue : va_list : __va_start_ten15 # ansi.stdarg.__va_start_ten15
#pragma token PROC ( EXP lvalue : va_list : e , TYPE t ) EXP rvalue : t : va_arg # ansi.stdarg.va_arg
#pragma token PROC ( EXP lvalue : va_list : ) EXP rvalue : void : va_end # ansi.stdarg.va_end
#pragma interface __va_t __va_start_ten15 va_arg va_end


/*
    DEFINITION OF VA_START

    How va_start is defined in terms of __va_start_ten15 depends on whether
    this is stdarg.h or varargs.h.
*/

#pragma TenDRA ident ... allow

#ifdef __HACKED_VARARGS
#define va_alist		...
#define va_dcl
#define va_start( l )		( ( void ) ( l = __va_start ( l ) ) )
#else
#define va_start( l, i )	( ( void ) ( l = __va_start ( l, i) ) )
#endif


#else /* __BUILDING_TDF_ANSI_STDARG_H_VA_ARGS */

/*
    IMPLEMENTATION OF STDARG

    This implementation basically works, and avoids the built-in
    operators defined in the system header.
*/

typedef char *va_list ;

#define __va_round( T )\
    ( ( ( sizeof ( T ) + 3 ) / 4 ) * 4 )

#define va_start( AP, ARG )\
    ( AP = &( ARG ) + __va_round ( ARG ) )

#define va_end( AP )	( ( void ) 0 )

#define va_arg( AP, T )\
    ( AP += __va_round ( T ),\
      *( ( T * ) ( ( AP ) - __va_round ( T ) ) ) )


#endif /* __BUILDING_TDF_ANSI_STDARG_H_VA_ARGS */


#ifndef __BUILDING_TDF_ISO99_STDARG_H_VA_COPY

#pragma token PROC ( EXP lvalue : va_list : dst , EXP lvalue : va_list : src ) EXP rvalue : void : va_copy # iso99.stdarg.va_copy
#pragma interface va_copy

#else /* __BUILDING_TDF_ISO99_STDARG_H_VA_COPY */

#define va_copy( DST, SRC )	( ( void ) ( DST = SRC ) )

#endif /* __BUILDING_TDF_ISO99_STDARG_H_VA_COPY */


#endif /* _STDARG_H */