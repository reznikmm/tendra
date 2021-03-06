/*
 * Copyright 2002-2012, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */

#include <shared/bool.h>

#include <local/al.h>
#include <local/out.h>

#include <tdf/shape.h>

#include <main/flags.h>

#include <construct/exp.h>
#include <construct/reg_result.h>

bool gcc_compatible = false;

/*
 * is the result of a procedure delivering
 * this shape produced in registers.
 */
bool
reg_result(shape sha)
{
	unsigned char n = sha->tag;

	if (gcc_compatible) {
		int sz = shape_size(sha);

		if (n == nofhd) {
			return 0;
		}

		if (assembler == ASM_SUN) {
			if (n == cpdhd && sz > 32 && sz != 64) {
				return 0;
			}
		} else {
			if (n == cpdhd && sz != 8 && sz != 16 && sz != 32 && sz != 64) {
				return 0;
			}
		}

		return 1;
	}

	if (n == cpdhd || n == nofhd) {
		return 0;
	}

	return 1;
}

