/* $Id$ */

/*
 * Copyright 2002-2012, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */

/*
 * This is the main routine.
 * It constructs .G (binasm) and .T (symbol table) files from the input .t
 * files. If -S is given it will also construct a .s (assembler) file in as_file.
 */

#include <stdio.h>

#include <shared/check.h>
#include <shared/getopt.h>
#include <shared/error.h>

#include <reader/basicread.h>
#include <reader/main_reads.h>
#include <reader/externs.h>

#include <construct/flags.h>
#include <construct/flpt.h>
#include <construct/installglob.h>
#include <construct/machine.h>
#include <construct/exp.h>

#include <main/driver.h>

#include "dump_distr.h"

extern void output_symtab(char*);

#define VERSION_STR "4.49"

void
out_rename(char * oldid, char * newid)
{
	/* ??? this may be wrong with diagnostics */
	return;
}

extern int good_trans;
bool do_extern_adds;
FILE * as_file;
FILE * ba_file;

int majorno =  3;
int minorno = 18;

long currentfile    = -1; /* our source fileno 0.. */
long mainfile       = 0;  /* would be best if it actually contained main ! */
bool produce_binasm = 0;

static void
init(void)
{
	endian    = ENDIAN_BIG;
	assembler = ASM_GAS;
	format    = FORMAT_ELF;
	diag      = DIAG_NONE;
	cconv     = CCONV_O32; /* TODO: confirm this is what we generate */
	abi       = ABI_MIPS;

	produce_binasm      = 0;
	PIC_code            = 0;
	keep_PIC_vars       = 1;
	diagnose_registers  = 0;
	target_dbl_maxexp   = 308;
	use_long_double     = 0;
	redo_structfns      = 0;
	do_extern_adds      = 0;
	do_alloca           = 0;

	as_file = NULL;
}

static int
option(char c, const char *optarg)
{
	switch (c) {
	case 'e': do_extern_adds = 1; break;
	case 's': produce_binasm = 1; break;

	default:
		return -1;
	}

	return 0;
}

static void
main(int argc, char **argv)
{
	char *nm;
	char *aname;
	char *dname;

	if (diag != DIAG_NONE) {
		/* dbx does not understand variable frame sizes */
		do_alloca = 0;
	}

	if (do_alloca && PIC_code) {
		failer("Can't do inline alloca with PIC code at the moment");
		exit(EXIT_FAILURE);
	}

	/* Things trans.mips does not "has" */
	has &= ~HAS_BYTEOPS;
	has &= ~HAS_BYTEREGS;
	has &= ~HAS_NEGSHIFT;
	has &= ~HAS_ROTATE;
	has &= ~HAS_MAXMIN;
	has &= ~HAS_SETCC;
	has &= ~HAS_COMPLEX;
	has &= ~HAS_64_BIT;

	/* line numbering goes to hell with optimisations */
	if (diag != DIAG_NONE) {
		optim = 0;
 	}

	if (produce_binasm) {
		dname = argv[1]; /* the .T file */
		nm    = argv[2]; /* the .G file */

		ba_file = fopen(nm, "w");
		if (ba_file == NULL) {
			printf("install: can't open output file %s\n", nm);
			exit(EXIT_FAILURE);
		}
	} else {
		aname = argv[1];

		as_file = fopen(aname, "w");
		if (as_file == NULL) {
			failer("can't open .s file");
			exit(EXIT_FAILURE);
		}
	}

	if (!initreader(argv[0])) {
		failer("cant read .t file");
		exit(EXIT_FAILURE);
	}

	init_flpt();

#include <reader/inits.h>

	top_def = NULL;

	local_prefix = "$$";
	name_prefix = "";

	d_capsule();

	if (produce_binasm) {
		fclose(ba_file);
		output_symtab(dname);
	} else {
		fclose(as_file);
	}
}

struct driver driver = {
	VERSION_STR,

	init,
	main,

	"es",
	option,
	NULL,

	ABI_MIPS,
	CCONV_O32,
	ENDIAN_BIG | ENDIAN_LITTLE,
	FORMAT_ELF,
	DIAG_NONE | DIAG_STABS,
	ASM_OSF1 | ASM_SGI | ASM_ULTRIX
};

