# $TenDRA$

CFLAGS+=${COPTS} ${DEBUG_FLAGS}

# Calculate our object directory and obj prefix.
OBJ_PREFIX?=   ${.CURDIR:C/(.*)\/src(.*)/\1\/obj/}
OBJ_DIR=       ${.CURDIR:C/(.*)\/src(.*)/${OBJ_PREFIX}\/\2/}


.if !defined(SRC_DIR)
SRC_DIR=        ${.CURDIR:C/(.*)\/src.*/\1/}
.endif

.if exists (${SRC_DIR}/config.mk)
HAVE_CONF=      yes
.endif


MAIN_TARGETS=	config-check obj-dir make-dir depend all

.MAIN: ${MAIN_TARGETS}

.ORDER: config-check obj-dir make-dir depend all

make-subdir: ${MAIN_TARGETS}


.if defined(HAVE_CONF)

.if defined(TL)
.include "tendra.api.mk"
.endif

.if defined(PROG)
.include "tendra.bin.mk"
.endif

.if defined(SHLIB)
.include "tendra.lib.mk"
.endif

# All our nifty targets.


all:
	env MAKEOBJDIR=${OBJ_DIR} ${MAKE} make-all


MAKE_ALL=	${PROG} ${TL} ${SHLIB}

make-all: ${MAKE_ALL}

depend:	
	env MAKEOBJDIR=${OBJ_DIR} ${MAKE} depend-all

depend-all: ${DEPEND_SRC}


# Install target.
install:
	env MAKEOBJDIR=${OBJ_DIR} ${MAKE} install-all



# Our clean targets.

CLEANFILES+= ${PROG} ${OBJS}

clean:
	env MAKEOBJDIR=${OBJ_DIR} ${MAKE} clean-all

clean-all:
.if !empty(CLEANFILES)
	rm -f ${CLEANOPT} ${CLEANFILES}
.endif

obj-dir:
	@if ! test -d ${OBJ_DIR}/; then \
		${MKDIR} ${OBJ_DIR}; \
		if ! test -d ${OBJ_DIR}/; then \
			${ECHO} "Unable to create ${OBJ_DIR}."; \
			exit 1; \
		fi; \
		${ECHO} "${OBJ_DIR} created for ${.CURDIR}"; \
	fi

make-dir:
.if defined(MAKEDIR)
.for i in ${MAKEDIR}
	@if ! test -d ${OBJ_DIR}/${i}; then \
		${MKDIR} ${OBJ_DIR}/${i}; \
	fi
.endfor
.endif

.include "tendra.def.mk"
.include "tendra.sys.mk"
.include "tendra.install.mk"
.include "tendra.subdir.mk"

.else

.endif

.include "tendra.config.mk"
