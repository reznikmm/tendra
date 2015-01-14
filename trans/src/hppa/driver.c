/* $Id$ */

/*
 * Copyright 2002-2012, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */

#include <stdio.h>
#include <string.h>

#include <shared/check.h>
#include <shared/getopt.h>
#include <shared/error.h>

#include <reader/exp.h>
#include <local/out.h>

#include <reader/main_reads.h>		/* for d_capsule */
#include <reader/externs.h>

#include <construct/flpt.h>
#include <construct/exp.h>		/* for current_alloc_size */
#include <construct/installglob.h>	/* for top_def */
#include <construct/installtypes.h>
#include <construct/machine.h>

#include <main/driver.h>
#include <main/flags.h>

#include "addrtypes.h"		/* for where */
#include "makecode.h"		/* for nowhere */
#include "diag_out.h"
#include "frames.h"

#define VERSION_STR "7.8"

/*
 * XXX: Some of the conditional optimisations local to this installer are
 * miscategorised as OPTIM_PEEPHOLE (which I'm using here for general purpose
 * optimisations, rather than introduce a new category specifically for
 * hppatrans). In particular the jump table optimisations ought to be split
 * out to OPTIM_JUMPS instead.
 */

static void
init(void)
{
	endian    = ENDIAN_BIG;
	assembler = ASM_HP;
	format    = FORMAT_SOM;
	diag      = DIAG_NONE;
	cconv     = CCONV_HPPA;
	abi       = ABI_HPUX;

	redo_structfns     = 0; /* procs delivering structs recast to extra param */
	redo_structparams  = 1; /* struct and union value parameters indirected   */
	do_profile         = 0; /* -P option for profiling info */
	do_alloca          = 0; /* inline alloca */
	PIC_code           = 0;
	keep_PIC_vars      = 1;
	diagnose_registers = 0;
	use_long_double    = 1;

#if use_long_double
	target_dbl_maxexp = 16384;
#else
	target_dbl_maxexp = 308;
#endif

	local_prefix = "$";
	name_prefix  = "";
}

static int
option(char c, const char *optarg)
{
	switch (c) {
	case 'd': plusZ            = 1; break; /* -d emulates cc's +Z flag */
	case 'i': do_indexed_loads = 0; break;

	default:
		return -1;
	}

	return 0;
}

static void
unhas(void)
{
	/* Things trans.hppa does not "has" */
	has &= ~HAS_BYTEOPS;
	has &= ~HAS_BYTEREGS;
	has &= ~HAS_NEGSHIFT;
	has &= ~HAS_ROTATE;
	has &= ~HAS_MAXMIN;
	has &= ~HAS_DIV0;
	has &= ~HAS_SETCC;
	has &= ~HAS_COMPLEX;
	has &= ~HAS_64_BIT;

	/* not implemented */
	optim &= ~OPTIM_TAIL;
	optim &= ~OPTIM_ZEROOFFSETS;
	optim &= ~OPTIM_SUBSTPARAMS;

	/* Careful with procedure results */
	optim &= ~OPTIM_UNPAD_APPLY;

	if (writable_strings) {
		/* TODO: either always on, or always off. error out accordingly */
	}

	if (diag != DIAG_NONE) {
		optim = 0;
		/* TODO: do gdb diagnostics depend on gcc assembly? */
	}

	if (do_profile && PIC_code) {
		error(ERROR_WARNING, "\"-P\" and \"-D\" are mutually exclusive; \"-P\" ignored.");
		do_profile = 0;
	}
}

static void
cleanup(void)
{
	exit_translator();
}

struct driver driver = {
	VERSION_STR,

	init,
	unhas,
	NULL,
	NULL,
	cleanup,

	"di",
	option,
	NULL,

	ABI_HPUX,
	CCONV_HPPA,
	ENDIAN_BIG | ENDIAN_LITTLE,
	FORMAT_SOM,
#ifdef _SYMTAB_INCLUDED
	DIAG_NONE | DIAG_STABS,
#else
	DIAG_NONE | DIAG_STABS | DIAG_XDB | DIAG_GDB,
#endif
	ASM_HP | ASM_GAS
};

