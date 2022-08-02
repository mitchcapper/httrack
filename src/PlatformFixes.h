#pragma once

/*
	Win32 redefine of functions to the stdlib names
	Win32 & Android fix for missing timezone global
	Linux - csafelib safe c functions

*/
#include "time.h"
/* Safe C functions and those noops for those they dont yet have*/
#ifndef _WIN32
#include "vendor/safeclib/include/safe_lib.h"
#include "vendor/safeclib/include/safe_str_lib.h"
#include "vendor/safeclib/include/safe_mem_lib.h"

inline size_t fread_s(
	void* buffer,
	size_t bufferSize,
	size_t elementSize,
	size_t count,
	FILE* stream
);
inline size_t fread_s(
	void* buffer,
	size_t bufferSize,
	size_t elementSize,
	size_t count,
	FILE* stream
) {
	return fread(buffer, elementSize, count, stream);
}
#endif

/* Specific macros */
#ifndef malloct
#define malloct malloc
#define freet free
#define calloct calloc
#define strcpybuff strcpy
#endif

#ifdef _WIN32 //winlist of functions to refine to use as gcc doesn't include stdc versions, note this must appear after standard library includes
#define strcasecmp(a,b) _stricmp(a,b)
#define strncasecmp(a,b,n) _strnicmp(a,b,n)
#define snprintf _snprintf
#define access    _access
#define chdir     _chdir
#define chmod     _chmod
#define close     _close
#define creat     _creat
#define dup       _dup
#define dup2      _dup2
#define ecvt      _ecvt
#define execl     _execl
#define execle    _execle
#define execlp    _execlp
#define execv     _execv
#define execve    _execve
#define execvp    _execvp
#define execvpe   _execvpe
#define fcloseall _fcloseall
#define fcvt      _fcvt
#define fdopen    _fdopen
#define fileno    _fileno
#define gcvt      _gcvt
#define getcwd    _getcwd
#define getpid    _getpid
#define getw      _getw
#define isatty    _isatty
#define j0        _j0
#define j1        _j1
#define jn        _jn
#define lfind     _lfind
#define lsearch   _lsearch
#define lseek     _lseek
#define memccpy   _memccpy
#define mkdir     _mkdir
#define mktemp    _mktemp
#define open      _open
#define putenv    _putenv
#define putw      _putw
#define read      _read
#define rmdir     _rmdir
#define strdup    _strdup
#define swab      _swab
#define tempnam   _tempnam
#define tzset     _tzset
#define umask     _umask
#define unlink    _unlink
#define utime     _utime
#define wcsdup    _wcsdup
#define write     _write
#define y0        _y0
#define y1        _y1
#define yn        _yn
#endif

/*Windows/android dont have timezone but we can deduce it*/
#if defined(_WIN32) || defined(__ANDROID__)
static int timezone = -9999;
#endif // !timezone
int time_offset(void);
time_t getGMT(struct tm* tm);




#if !defined(_WIN32) && !defined(errno_t)
typedef int errno_t;
#endif

#ifndef _WIN32
#ifndef BOOL
typedef int                 BOOL;
#endif
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#endif // ! _WIN32

