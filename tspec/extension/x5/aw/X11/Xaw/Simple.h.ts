# Copyright 2002-2011, The TenDRA Project.
# Copyright 1997, United Kingdom Secretary of State for Defence.
#
# See doc/copyright/ for the full copyright terms.


+USE "x5/t", "X11/Intrinsic.h.ts" ;

# Simple Widget (3.8)

+TYPE ( struct ) SimpleClassRec ;
+TYPEDEF SimpleClassRec *SimpleWidgetClass ;

+EXP lvalue WidgetClass simpleWidgetClass ;

+TYPE ( struct ) SimpleRec ;
+TYPEDEF SimpleRec *SimpleWidget ;
