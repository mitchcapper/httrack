
//
// path-join.c
//
// Copyright (c) 2013 Stephen Mathieson
// MIT licensed
//

#include <string.h>
#include <stdlib.h>

#include "str-ends-with.h"
#include "str-starts-with.h"
#include "path-join.h"
#include "../PlatformFixes.h"

#define WIN_PATH_JOIN_SEPERATOR   "\\"

#define PATH_JOIN_SEPERATOR   "/"


/*
 * Join `dir` with `file`
 */
char* replace_char(char* str, char find, char replace);
char* replace_char(char* str, char find, char replace) {
	char* current_pos = strchr(str, find);
	while (current_pos) {
		*current_pos = replace;
		current_pos = strchr(current_pos, find);
	}
	return str;
}
char *
path_join(const char *dir, const char *file) {
	size_t size = strlen(dir) + strlen(file) + 2;
  char *buf = malloc(size * sizeof(char));
  if (NULL == buf) return NULL;

  strcpy_s(buf, size * sizeof(char), dir);

  // add the sep if necessary
  if (!str_ends_with(dir, PATH_JOIN_SEPERATOR) && !str_ends_with(dir, WIN_PATH_JOIN_SEPERATOR))
    strcat_s(buf, size * sizeof(char), PATH_JOIN_SEPERATOR);
  

    // remove the sep if necessary
  if (str_starts_with(file, PATH_JOIN_SEPERATOR) || str_starts_with(file, WIN_PATH_JOIN_SEPERATOR))
	  file++;

    strcat_s(buf, size * sizeof(char), file);
	replace_char(buf, WIN_PATH_JOIN_SEPERATOR[0], PATH_JOIN_SEPERATOR[0]);//not needed win is fine with bopth but might a well

  return buf;
}
