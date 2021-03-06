/*
 * Copyright 2002-2011, The TenDRA Project.
 * Copyright 1997, United Kingdom Secretary of State for Defence.
 *
 * See doc/copyright/ for the full copyright terms.
 */


/*
 * file-name.c - File name manipulation routines.
 *
 * This file implements the file name manipulation facility specified in the
 * file "file-name.h".  See that file for more details.
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>

#include <stddef.h>
#include <string.h>

#include <shared/bool.h>
#include <shared/check.h>

#include <exds/common.h>
#include <exds/cstring.h>

#include "file-name.h"

char *
file_name_basename(char * path)
{
    char * last = strrchr(path, '/');

    if (last) {
	return cstring_duplicate(last + 1);
    } else {
	return cstring_duplicate(path);
    }
}

char *
file_name_dirname(char * path)
{
    char * last = strrchr(path, '/');

    if (last) {
	return cstring_duplicate_prefix(path, (size_t) (last - path));
    } else {
	return cstring_duplicate(path);
    }
}

char *
file_name_expand(const char * dir,			  const char * name, 
			  const char * suffix)
{
    size_t dir_length  = strlen(dir);
    size_t name_length = strlen(name);
    size_t suf_length  = (suffix ?(strlen(suffix)): 0);
    size_t suf_extra   = (size_t) (suffix ? 1 : 0);
    size_t extra;
    size_t length;
    char * path;
    char * tmp;

    if ((dir_length > 0) && (dir[dir_length - 1]!= '/')) {
	extra = 1;
    } else {
	extra = 0;
    }
    length = (dir_length + extra + name_length + suf_extra + suf_length + 1);
    path   = ALLOCATE_VECTOR(char, length);
    tmp    = path;
    if (dir_length > 0) {
	IGNORE memcpy((void *)tmp,(void *)dir,(size_t)dir_length);
	tmp += dir_length;
	if (dir[dir_length - 1]!= '/') {
	    tmp[0] = '/';
	    tmp++;
	}
    }
   IGNORE memcpy((void *)tmp,(void *)name,(size_t)name_length);
    tmp += name_length;
    if (suffix) {
	tmp[0] = '.';
	tmp++;
	IGNORE memcpy((void *)tmp,(void *)suffix,(size_t)suf_length);
	tmp += suf_length;
    }
    tmp[0] = '\0';
    return path;
}

bool
file_name_is_basename(const char * path)
{
    return !strchr(path, '/');
}

bool
file_name_is_absolute(char * path)
{
    return path[0] == '/';
}

void
file_name_populate(char * path)
{
    char * new_path = cstring_duplicate(path);
    char * tmp      = strchr(new_path, '/');

    if (tmp) {
	do {
	    *tmp = '\0';
	    IGNORE mkdir(new_path, 0755);
	    *tmp = '/';
	} while ((tmp = strchr(tmp + 1, '/')) != NULL);
    }
    DEALLOCATE(new_path);
}
