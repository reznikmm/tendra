# Copyright 2002-2011, The TenDRA Project.
# Copyright 1997, United Kingdom Secretary of State for Defence.
#
# See doc/copyright/ for the full copyright terms.


+IMPLEMENT "c/c89", "stdlib.h.ts", "alloc" (!?) ;
+IMPLEMENT "c/c89", "stdlib.h.ts", "free" (!?) ;

+FIELD struct mallinfo | mallinfo_struct {
    int arena ;
    int ordblks ;
    int smblks ;
    int hblkhd ;
    int hblks ;
    int usmblks ;
    int fsmblks ;
    int uordblks ;
    int fordblks ;
    int keepcost ;
} ;

+CONST int M_MXFAST, M_NLBLKS, M_GRAIN, M_KEEP ;

+FUNC int mallopt ( int, int ) ;
+FUNC struct mallinfo mallinfo ( void ) ;
