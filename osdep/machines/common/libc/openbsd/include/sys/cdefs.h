/* $Id$ */

#ifndef __HACKED_LIBC_SYS_CDEFS_H
#define __HACKED_LIBC_SYS_CDEFS_H

#if defined(_OPENBSD5_2) || defined(_OPENBSD5_3)
#define __restrict
#endif

#include_next <sys/cdefs.h>

#endif

