# $Id$

.include "Makefile.inc"

.include <tendra.base.mk>
.include <tendra.makedefs.mk>


TRANS_CFLAGS+=	-D TRANS_POWER

TRANS_CFLAGS+=	-I ${BASE_DIR}/src/power
TRANS_CFLAGS+=	-I ${BASE_DIR}/src/common
TRANS_CFLAGS+=	-I ${BASE_DIR}/src/common/linkinfo


TRANS_DEBUG?= diag3

# TODO: these are not yet selectable at runtime
.if "${TRANS_DEBUG}" == "diag3"

TRANS_CFLAGS+= -D TDF_DIAG3

PARTS+= src/common/diag3

TRANS_CFLAGS+= -D DIAG3_STABS

SRCS+= stabs_diag3.c

.else

.BEGIN:
	@${ECHO} '$${TRANS_DEBUG} unrecognised'
	@${EXIT} 1;

.endif


PART_CFLAGS+=	${TRANS_CFLAGS}

PARTS+=	src/common/shared
PARTS+=	src/common/tdf
PARTS+=	src/common/special
PARTS+=	src/common/construct
PARTS+=	src/common/linkinfo
PARTS+=	src/common/flpt
PARTS+=	src/common/reader
PARTS+=	src/common/refactor
PARTS+= src/common/utility
PARTS+= src/common/main

.include <tendra.partial.mk>


#TCCOPTS=-Xc -Y32bit -Yc89

CCOPTS+=	${TRANS_CFLAGS}

SRCS+=	ashof.c
SRCS+=	code_here.c
SRCS+=	dynamic_init.c
SRCS+=	error.c
SRCS+=	eval.c
SRCS+=	frames.c
SRCS+=	getregs.c
SRCS+=	guard.c
SRCS+=	inlinechoice.c
SRCS+=	inst_fmt.c
SRCS+=	instruct.c
SRCS+=	is_worth.c
SRCS+=	labels.c
SRCS+=	locate.c
SRCS+=	reg_result.c
SRCS+=	machine_toks.c
SRCS+=	macro.c
SRCS+=	driver.c
SRCS+=	make_code.c
SRCS+=	mask.c
SRCS+=	mem_copy.c
SRCS+=	move.c
SRCS+=	muldvrem.c
SRCS+=	scan.c
SRCS+=	ops.c
SRCS+=	parameter.c
SRCS+=	pp.c
SRCS+=	proc.c
SRCS+=	record_bit.c
SRCS+=	regable.c
SRCS+=	regalloc.c
SRCS+=	regexps.c
SRCS+=	special_fn.c
SRCS+=	special_tok.c
SRCS+=	stack.c
SRCS+=	tempdecs.c
SRCS+=	translate.c
SRCS+=	weights.c

.include <tendra.obj.mk>


PROG=    	trans.${.CURDIR:T}
EXEC_BIN=	${PREFIX_LIBEXEC}

.include <tendra.prog.mk>

