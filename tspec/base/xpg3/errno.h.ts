# $Id$

# Copyright 2002-2011, The TenDRA Project.
# Copyright 1997, United Kingdom Secretary of State for Defence.
#
# See doc/copyright/ for the full copyright terms.


+IMPLEMENT "posix", "errno.h.ts" ;

+SUBSET "enums" := {
    +SUBSET "expg4" := {
	+IMPLEMENT "posix", "errno.h.ts", "enums" ;
	+CONST int EIDRM, ENOMSG, ETXTBSY ;
    } ;
    +CONST int ENOTBLK ;
} ;