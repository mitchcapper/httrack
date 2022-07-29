/* ------------------------------------------------------------ */
/*
HTTrack Website Copier, Offline Browser for Windows and Unix
Copyright (C) 1998-2017 Xavier Roche and other contributors

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

Important notes:

- We hereby ask people using this source NOT to use it in purpose of grabbing
emails addresses, or collecting any other private information on persons.
This would disgrace our work, and spoil the many hours we spent on it.

Please visit our Website: http://www.httrack.com
*/

/* ------------------------------------------------------------ */
/* File: httrack.c subroutines:                                 */
/*       basic authentication: password storage                 */
/* Author: Xavier Roche                                         */
/* ------------------------------------------------------------ */

#ifndef HTSBAUTH_DEFH
#define HTSBAUTH_DEFH

#include <sys/types.h>

// robots wizard
#ifndef HTS_DEF_FWSTRUCT_bauth_chain
#define HTS_DEF_FWSTRUCT_bauth_chain
typedef struct bauth_chain bauth_chain;
#endif
struct bauth_chain {
  char prefix[1024];            /* www.foo.com/secure/ */
  char auth[1024];              /* base-64 encoded user:pass */
  struct bauth_chain *next;     /* next element */
};

// buffer pour les cookies et authentification
#ifndef HTS_DEF_FWSTRUCT_t_cookie
#define HTS_DEF_FWSTRUCT_t_cookie
typedef struct t_cookie t_cookie;


#endif
#define COOKIE_PARAM_BUFFER_SIZE 1024
#define COOKIE_VALUE_BUFFER_SIZE 4096 //RFC requirements are technically 4096 total for the entire cookie minimum when adding all 4 together
#define COOKIE_MAX 100 //RFC technically says 3000 max 50 per domain but this ist stack alloced so higher requiures building with larger stack ie /STACK:16777216
#define COOKIE_SINGLE_MAX_SIZE sizeof(t_cookie_expanded)
struct t_cookie_expanded {
	char cook_name[COOKIE_PARAM_BUFFER_SIZE];
	char cook_value[COOKIE_VALUE_BUFFER_SIZE];
	char domain[COOKIE_PARAM_BUFFER_SIZE];
	char path[COOKIE_PARAM_BUFFER_SIZE];
};
typedef struct t_cookie_expanded t_cookie_expanded;

static const struct t_cookie_expanded EmptyExpandedCookieStruct;//a zero struct


struct t_cookie {
  int max_len;
  char data[COOKIE_MAX * COOKIE_SINGLE_MAX_SIZE];
  bauth_chain auth;
};



/* Library internal definictions */
#ifdef HTS_INTERNAL_BYTECODE
typedef enum { COOK_PARAM_PATH = 2, COOK_PARAM_NAME = 5, COOK_PARAM_DOMAIN = 0, COOK_PARAM_VALUE = 6 } COOK_PARAM_t;
// cookies
int cookie_addE(t_cookie* cookie, t_cookie_expanded* cookie_expanded);
int cookie_add(t_cookie * cookie, const  char *cook_name, const  char *cook_value,
               const  char *domain, const  char *path);
int cookie_del(t_cookie * cookie, const char *cook_name, const char *domain, const char *path);

int cookie_load(t_cookie * cookie, const char *path, const char *name);
int cookie_save(t_cookie * cookie, const char *name);
void cookie_insert(char *s, const char *ins);
void cookie_delete(char *s, size_t pos);
const char *cookie_get(char *buffer, const char *cookie_base, COOK_PARAM_t param);
t_cookie_expanded cookie_getE(const char* cookie_base);
char *cookie_find(char *s, const char *cook_name, const char *domain, const char *path);
char *cookie_nextfield(char *a);

// basic auth
int bauth_add(t_cookie * cookie, const char *adr, const char *fil, const char *auth);
char *bauth_check(t_cookie * cookie, const char *adr, const char *fil);
char *bauth_prefix(char *buffer, const char *adr, const char *fil);

#endif

#endif
