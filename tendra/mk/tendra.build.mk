# TenDRA make build infrastructure
#
# Keep tendra.bootstrap.mk and tendra.build.mk in sync!
# $Id$

.if !defined(_TENDRA_BUILD_MK_)
_TENDRA_BUILD_MK_=1

.include <tendra.base.mk>
.include <tendra.functions.mk>
.include <tendra.rules.mk>

# Do some pretty-printing.
WRKDIR=		${.CURDIR:C/^${BASE_DIR}\///1}

# The main target is synthetised based upon actual needs.
.if !target(.MAIN)
.MAIN: all
.endif

# Do it all. Real workers, mutually exclusive, follow.

.if "${API}" != ""
#
# Build API dependencies.
#

CC=	${OBJ_DIR}/src/tools/tcc/tcc -Y${OBJ_DIR}/src/lib/env/build -yTENDRA_BASEDIR=${OBJ_DIR}/src
CFLAGS=
LDFLAGS=

JOPTS= -Y32bit -I${BASE_DIR}/src/lib/machines/${OSFAM}/${BLDARCH}/include \
	-I/usr/include -f${BASE_DIR}/${STARTUP_MACH}/${API}.h -D__BUILDING_LIBS

api-depend:
	@${ECHO} "==> Creating API source for ${API}"
	${TSPEC} -I${BASE_DIR}/${APIS} -O./ -S./building ${API}

# Include Makefile generated by tspec during api-depend.
.if exists(./building/${API}.api/Makefile)
.include "./building/${API}.api/Makefile"
.endif

#
# Build API.
#
_REALWORK: api-depend
	cd ${BASE_DIR}/${APIS}/${API}/ && ${.MAKE} makeapi
makeapi: ${APILIB}

${APILIB}: ${APIOBJS}
	@${ECHO} "==> Linking ${API} API"
	${TLD} -mc -o ${APILIB} ${APIOBJS}

CLEAN_EXTRA+= ${APILIB} ${APIOBJS} ${APIOBJS:S/.j/.c/} \
              ./building/${API}.api/Makefile

_objdir=	${OBJ_DIR}/${APIS}
.elif "${ENVFILE}" != ""
#
# Build environments.
#
fixenv.sed:
	@${ECHO} "==> Create ${.TARGET}"
	@${ECHO} "1,\$$s%-MACH-%${OSFAM}/${BLDARCH}%g" 		\
		> ${OBJ_DIR}/${.TARGET}
	@${ECHO} "1,\$$s%-BLDARCH-%${BLDARCH}%g" 		\
		>> ${OBJ_DIR}/${.TARGET}
	@${ECHO} "1,\$$s%-OSFAM-%${OSFAM}%g" 			\
		>> ${OBJ_DIR}/${.TARGET}
	@${ECHO} "1,\$$s%-MACHDIR-%${MACH_BASE}%g" 		\
		>> ${OBJ_DIR}/${.TARGET}
	@${ECHO} "1,\$$s%-BINDIR-%${MACH_BASE}/bin%g" 		\
		>> ${OBJ_DIR}/${.TARGET}
	@${ECHO} "1,\$$s%-ENVDIR-%${MACH_BASE}/env%g" 		\
		>> ${OBJ_DIR}/${.TARGET}
	@${ECHO} "1,\$$s%-LIBDIR-%${MACH_BASE}/lib%g" 		\
		>> ${OBJ_DIR}/${.TARGET}
	@${ECHO} "1,\$$s%-INCLDIR-%${COMMON_DIR}/include%g" 	\
		>> ${OBJ_DIR}/${.TARGET}
	@${ECHO} "1,\$$s%-STARTUPDIR-%${COMMON_DIR}/startup%g" 	\
		>> ${OBJ_DIR}/${.TARGET}
	@${ECHO} "1,\$$s%-TMPDIR-%${TMP_DIR}%g" 		\
		>> ${OBJ_DIR}/${.TARGET}

_REALWORK: fixenv.sed .USE
	@${ECHO} "==> Fixing paths for ${ENVFILE} environments"
. for entry in ${ENVFILE}
	sed -f ${OBJ_DIR}/fixenv.sed ${.CURDIR}/${entry} > ${OBJ_DIR}/${ENVIRONMENT}/${entry}
. endfor
. if "${ENVEXTRA}" != ""
	cat ${.CURDIR}/${ENVEXTRA} >> ${OBJ_DIR}/${ENVIRONMENT}/build
	cat ${.CURDIR}/${ENVEXTRA} >> ${OBJ_DIR}/${ENVIRONMENT}/default
. endif

_objdir=	${OBJ_DIR}/${ENVIRONMENT}
.elif "${STARTUPSUBDIR}" != ""
#
# Install startup files in object directory
#
_REALWORK: .USE
	@${ECHO} "# Installing ${STARTUPSUBDIR} startup directories into object directory"
	${CONDCREATE} ${STARTUPSUBDIR:S/^/${OBJ_SDIR}\//g}
	@${FIND} ${STARTUPSUBDIR} -name '.*' -prune -or -print | \
	while read file; do \
		if ${TEST} -d $${file}; then \
			${ECHO} ${INSTALL} -m 755 -d ${OBJ_SDIR}/${startsub}/$${file}; \
			${INSTALL} -m 755 -d ${OBJ_SDIR}/${startsub}/$${file} || ${EXIT} $$?; \
		else \
			${ECHO} ${INSTALL} -m 644 $${file} ${OBJ_SDIR}/${startsub}/$${file}; \
			${INSTALL} -m 644 $${file} ${OBJ_SDIR}/${startsub}/$${file} || ${EXIT} $$?; \
		fi; \
	done

_objdir=	${OBJ_SDIR}
.elif "${SYS_FILES}" || "${TDFLIB}" != ""
#
# Build token definition.
#
_REALWORK: .USE
	@${ECHO} "==> Building ${TDFLIB} token definition"

_objdir=	${OBJ_SDIR}
.elif "${PROG}" != ""
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
. endif

CLEAN_EXTRA+=	${PROG} ${PROG}.core core ${OBJS}
_objdir=	${OBJ_SDIR}
.elif "${LIB}" != ""
#
# Build a library
#

CC=	${OBJ_DIR}/src/tools/tcc/tcc -Y${OBJ_DIR}/src/lib/env/build -yTENDRA_BASEDIR=${OBJ_DIR}/src
CFLAGS=
LDFLAGS=

JOPTS=

_REALWORK: lib${LIB}.a

lib${LIB}.a: ${OBJS}
	${AR} cr ${.TARGET} ${OBJS}
	${RANLIB} ${.TARGET}

CLEAN_EXTRA+=	lib${LIB}.a ${OBJS}
_objdir=	${OBJ_SDIR}
.else
#
# Nothing to do here.
#
_REALWORK:
.endif

# Suppport targets.

# Remove registered garbage.
clean:
.if "${CLEAN_EXTRA}" != ""
	@${ECHO} "==> Cleaning ${.CURDIR}"
	${REMOVE} ${CLEAN_EXTRA}
.endif

# Clean objdir blindly.
cleandir:
.if "${_objdir}" != ""
	${REMOVE} -r ${_objdir}/* 2> /dev/null
.endif

# Remove obj symlink from curdir.
cleanobj:
.if "${_objdir}" != ""
	@if ${TEST} -h ${.CURDIR}/obj ; then \
		${ECHO} "==> Removing obj link ${WRKDIR}/obj" ; \
		${REMOVE} ${.CURDIR}/obj ; \
		${REMOVE} ${OBJ_DIR_INDICATOR} 2> /dev/null ; \
	fi
.endif

# Create objdir and its symlink.
obj:
.if "${_objdir}" != ""
. if !exists(${_objdir})
	@${ECHO} "==> Creating objdir ${_objdir}"
	@${MKDIR} -p ${_objdir}
. endif
	@${REMOVE} ${.CURDIR}/obj
	@${LN} -sf ${_objdir} ${.CURDIR}/obj
.endif

# Glue all the steps together.

all: _REALWORK

# Make sure subdirs are walked after real work is done.
.ORDER: _REALWORK _SUBDIR

# Proceed to subdirs.
_SUBDIR: .USE
.if defined(SUBDIR) && !empty(SUBDIR)
. for entry in ${SUBDIR}
	@cd ${.CURDIR}/${entry}; ${MAKE} ${.TARGET}
. endfor
.endif

# Ensure existence of basic targets.
.for target in obj all clean install cleanobj cleandir
${target}: _SUBDIR .PHONY
.endfor

.endif	# !defined(_TENDRA_BUILD_MK_)
