/* $Id$ */

/*
 * Copyright 2002-2012, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */

#include <stdlib.h>

#include <shared/check.h>
#include <shared/getopt.h>
#include <shared/error.h>

#include <local/expmacs.h>
#include <local/localflags.h>
#include <local/out.h>

#include <reader/basicread.h>
#include <reader/externs.h>
#include <reader/main_reads.h>
#include <reader/readglob.h>

#include <construct/flags.h>
#include <construct/flpt.h>
#include <construct/installglob.h>
#include <construct/machine.h>
#include <construct/exp.h>

#include <main/driver.h>

#include "weights.h"
#include "instr.h"
#include "instr386.h"
#include "messages_8.h"
#include "assembler.h"
#include "operand.h"

#ifdef NEWDIAGS
#include <newdiag/diag_fns.h>
#else
#include <diag/diag_fns.h>
#endif

#ifdef NEWDWARF
#include <dwarf2/dw2_iface.h>
#include <dwarf2/dw2_abbrev.h>
#include <dwarf2/dw2_common.h>
static bool dump_abbrev = 0;
#endif /* NEWDWARF */

#if DWARF
#include <dwarf/dwarf_mc.h>
#endif

static void init_all(void);

extern int print_inlines;

extern int use_link_stuff;

#define VERSION_STR "5.12"

static void
init_all(void)
{
	init_flpt();

#include <reader/inits.h> /* XXX: not nice */

	good_trans       = 0;
	capsule_freelist = NULL;
	old_proc_props   = NULL;
	promote_pars     = 0;
	load_ptr_pars    = 1;

	/* 80x86 specific */
	extra_stack = 0;
	top_def = NULL;
	init_weights();
	initzeros();
	const_list = nilexp;
}

static void
init(void)
{
	cpu       = CPU_80586;
	endian    = ENDIAN_LITTLE;
	assembler = ASM_GAS;
	format    = FORMAT_ELF;
	cconv     = CCONV_GCC; /* TODO: name properly */
	diag      = DIAG_NONE;
	abi       = ABI_IBCS;

#ifdef NEWDIAGS
	diag_visible = 0;
	extra_diags  = 0;
#endif /* NEWDIAGS */

	redo_structfns         = 1; /* replace fns delivering structs */
	redo_structparams      = 0; /* no change to struct params */
	separate_units         = 0; /* combine units */
	always_use_frame       = 0; /* avoid using frame pointer */
	do_profile             = 0; /* no profiling code */
	writable_strings       = 0; /* strings are read only */
	PIC_code               = 0; /* do not produce PIC code */
	all_variables_visible  = 0; /* use registers */
	do_alloca              = 1; /* inline calls of alloca */
	round_after_flop       = 0; /* do not round after each floating point operation */
	strict_fl_div          = 1; /* do not replace divisions by multiplication by the inverse */
	flpt_always_comparable = 0; /* this is the default for SVR4.2 */
	use_long_double        = 1;
	report_versions        = 0; /* do not print version numbers */
	permit_8byte_align     = 1; /* allow 8byte alignment for local doubles */
	replace_arith_type     = 1; /* use the C definitions of promote etc. */
	no_bss                 = 0; /* use .comm */
	trap_on_nil_contents   = 0;
	load_ptr_pars          = 1;
	use_link_stuff         = 1;

	target_dbl_maxexp   = 4932; /* I am not sure what's significant about 4932, and not 16384 */

	ptr_null  = 0x0; /* NULL value for pointer */
	proc_null = 0x0; /* NULL value for proc */
	lv_null   = 0x0; /* NULL value for label_value*/
}

static int
option(char c, const char *optarg)
{
	/* TODO: default various lowercase flags, depending on abi */

	switch (c) {
	case 'a': always_use_frame       = 1; break;
	case 'b': all_variables_visible  = 1; break;
	case 'c': replace_arith_type     = 0; break;
	case 'd': redo_structfns         = 0; break;
	case 'g': flpt_always_comparable = 1; break;
	case 'h': no_bss                 = 1; break;
	case 'i': print_inlines          = 1; break;
	case 'v': avoid_intov = atoi(optarg); break;

	case 'f':
		/* XXX: undocumented */
		/* NULL value for pointer */
		ptr_null = 0x55555555;
		break;

	case 't':
		/* TODO: I think it ought to be set by the ABI. But I'm putting
		 * it here for the moment to keep it out of the way.
		 * I think this is only relevant for a.out systems. */
		gcc_compatible = (*optarg == '1');
		break;

	default:
		return -1;
	}

	return 0;
}

static int
switch_cpu(const char *optarg)
{
	cpu = 0;

	switch (*optarg) {
	case '5': cpu |= CPU_80586;
	case '4': cpu |= CPU_80486;
	case '3': cpu |= CPU_80386; break;

	default:
		return -1;
	}

	return 0;
}

static void
main(int argc, char **argv)
{
	int ch;
	char *outfname;
	const char *optstring;

	if (argc == 0 || (argc % 2) != 0) {
		failer(BAD_COMMAND1);
		exit(EXIT_FAILURE);
	}

	/*
	 * TODO: unsure about this. I think it ought to be set by the ABI.
	 * It might not neccessarily always be the same as gcc_compatible,
	 * because those differed for Solaris x86. Or that was an a.out system,
	 * and this isn't relevant for ELF.
	 */
	remove_struct_ref = gcc_compatible;

	/* Things trans.80x86 does not "has" */
	has &= ~HAS_NEGSHIFT;
	has &= ~HAS_CONDASSIGN;
	has &= ~HAS_COMPLEX;
	has &= ~HAS_64_BIT;

	if (~cpu & CPU_80586) {
		has &= ~HAS_SETCC;
	}

	/* XXX: invalid assembly is generated without this */
	optim |= OPTIM_CASE;

	if (diag != DIAG_NONE) {
		optim = 0;
		always_use_frame = 1;
		all_variables_visible = 1;
#ifdef NEWDIAGS
		diag_visible = 1;
#endif
	}

	set_format(format);

	while (*argv) {
		outfname = argv[1];

		/* initiate the output file */
		if (!outinit(outfname)) {
			failer(CANT_OPEN);
			exit(EXIT_FAILURE);
		}

		if (!initreader(argv[0])) {
			failer(CANT_READ);
			exit(EXIT_FAILURE);
		}

		init_all();

#ifdef NEWDWARF
		if (diag == DIAG_DWARF2)
			init_dwarf2();
		else
#endif
			if (diag != DIAG_NONE)
				out_diagnose_prelude();

#ifdef NEWDWARF
		if (dump_abbrev) {
			outs(".text\n");
			do_abbreviations();
			dwarf2_prelude();
			make_dwarf_common();
			dwarf2_postlude();
		} else
#endif

		(void) d_capsule();

		while (weak_list) {
			outs(".set ");
			outs(weak_list->weak_id);
			outs(",");
			outs(weak_list->val_id);
			outnl();
			weak_list = weak_list->next;
		}

#ifdef NEWDWARF
		if (diag == DIAG_DWARF2)
			end_dwarf2();
		else
#endif
			if (diag != DIAG_NONE)
				out_diagnose_postlude();

		outend(); /* close the .s file */
		endreader();

		if (good_trans)
			exit(EXIT_FAILURE);

		argv += 2; /* next pair of input/output files */
	}
}

struct driver driver = {
	VERSION_STR,

	init,
	main,

	"abcdfghit:v:",
	option,
	switch_cpu,

	ABI_SCO | ABI_SVR4 | ABI_LINUX | ABI_SOLARIS | ABI_IBCS,
	CCONV_GCC, /* TODO: move remove_struct_ref and possibly also gcc_compatible here */
	ENDIAN_LITTLE,
	FORMAT_ELF | FORMAT_AOUT,
	DIAG_NONE | DIAG_STABS | DIAG_DWARF | DIAG_DWARF2,
	ASM_GAS
};

