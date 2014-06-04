/* $Id$ */

/*
 * Copyright 2011, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */

#include <shared/bool.h>

#include <construct/shape.h>

#include <main/flags.h>

#include "regable.h"

bool
reg_result(shape sha)
{
    if ( valregable ( sha ) ) return 1;
    if ( is_floating ( name ( sha ) ) ) {
	if ( use_long_double && shape_size ( sha ) > 64 ) return 0;
	return 1;
    }
    return 0;
}

