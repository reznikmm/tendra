# TenDRA make build infrastructure
#
# $Id$

.if !defined(_TENDRA_WORK_PROG_MK_)
_TENDRA_WORK_PROG_MK_=1

.include <tendra.base.mk>
.include <tendra.functions.mk>

.if !defined(PROG)
.BEGIN:
	@${ECHO} '$${PROG} must be set'
	@${EXIT} 1;
.endif



#
# Build a program.
#
. if !defined(MAN) && exists(${.CURDIR}/${PROG}.1)
MAN=	${PROG}.1
. endif

${PROG}: ${OBJS}
	@${ECHO} "==> Linking ${WRKDIR}/${PROG}"
	${CC} ${LDOPTS} -o ${PROG} ${OBJS} ${LIBS}

_REALWORK: ${PROG} .USE
. if defined(WRAPPER)
	@${ECHO} "==> Adjusting paths for ${WRAPPER}"
	sed -e 1,\$$s%@@MACH_BASE@@%${MACH_BASE}%g \
		-e 1,\$$s%@@PREFIX@@%${PREFIX}%g \
		${.CURDIR}/${WRAPPER} > ${WRAPPER}

CLEAN_EXTRA+=	${WRAPPER}
. endif

CLEAN_EXTRA+=	${PROG} ${PROG}.core core ${OBJS}
_objdir=	${OBJ_SDIR}



#
# Install a program.
#
_REALINSTALL: .USE
	@${ECHO} "==> Installing ${PROG}"
	${CONDCREATE} "${PUBLIC_BIN}" "${MACH_BASE}/bin"
	${INSTALL} -m 755 ${PROG} ${MACH_BASE}/bin/${PROG}
. if "${WRAPPER}" != ""
	${INSTALL} -m 755 ${WRAPPER} ${PUBLIC_BIN}/${PROG}
. endif



.endif	# !defined(_TENDRA_WORK_PROG_MK_)
