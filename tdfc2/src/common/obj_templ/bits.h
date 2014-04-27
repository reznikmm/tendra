/* $Id$ */

/*
 * Copyright 2002-2011, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */

    
    
    
    
    

#ifndef BITS_INCLUDED
#define BITS_INCLUDED


/*
    BIT VALUES

    These macros give the number of bits needed to encode the various
    enumeration and union types.  This file is automatically generated
    from algebra %X (version %V) by %ZX (version %ZV).
*/

@loop enum
#define BITS_%EM%t32%EO2
@end

@loop union
#define BITS_%UM%t32%UO3
@end

#define BITS_cv_qual		3
#define BITS_ellipsis		4
#define BITS_lex		9


#endif