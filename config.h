/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Check for dlopen in c */
#define DLLIB 1

/* Check for ftime */
#define FTIME 1

/* Define if pointers to integers require aligned access */
/* #undef HAVE_ALIGNED_ACCESS_REQUIRED */

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <execinfo.h> header file. */
#define HAVE_EXECINFO_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `z' library (-lz). */
#define HAVE_LIBZ 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `snprintf' function. */
#define HAVE_SNPRINTF 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Check for strnlen */
#define HAVE_STRNLEN 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 or 0, depending whether the compiler supports simple visibility
   declarations. */
#define HAVE_VISIBILITY 1

/* Define to 1 if you have the `vsnprintf' function. */
#define HAVE_VSNPRINTF 1

/* Check for in_addr_t */
#define HTS_DO_NOT_REDEFINE_in_addr_t /**/

/* Check for IPv6 */
#define HTS_INET6 1

/* Check for large files support */
#define HTS_LFS 1

/* Check for OpenSSL */
#define HTS_USEOPENSSL 1

/* Check for libiconv */
/* #undef LIBICONV */

/* Check for libsocket */
/* #undef LIBSOCKET */

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "httrack"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "roche+packaging@httrack.com"

/* Define to the full name of this package. */
#define PACKAGE_NAME "httrack"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "httrack 3.49.2"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "httrack"

/* Define to the home page for this package. */
#define PACKAGE_URL "http://www.httrack.com/"

/* Define to the version of this package. */
#define PACKAGE_VERSION "3.49.2"

/* "enable replacement (v)snprintf if system (v)snprintf is broken" */
/* #undef PREFER_PORTABLE_SNPRINTF */

/* Check for setuid */
#define SETUID 1

/* The size of `long', as computed by sizeof. */
#define SIZEOF_LONG 8

/* The size of `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Check for pthread in pthreads */
#define THREADS 1

/* Enable extensions on AIX 3, Interix.  */
#ifndef _ALL_SOURCE
# define _ALL_SOURCE 1
#endif
/* Enable GNU extensions on systems that have them.  */
#ifndef _GNU_SOURCE
# define _GNU_SOURCE 1
#endif
/* Enable threading extensions on Solaris.  */
#ifndef _POSIX_PTHREAD_SEMANTICS
# define _POSIX_PTHREAD_SEMANTICS 1
#endif
/* Enable extensions on HP NonStop.  */
#ifndef _TANDEM_SOURCE
# define _TANDEM_SOURCE 1
#endif
/* Enable general extensions on Solaris.  */
#ifndef __EXTENSIONS__
# define __EXTENSIONS__ 1
#endif


/* Version number of package */
#define VERSION "3.49.2"

/* Define to 1 if on MINIX. */
/* #undef _MINIX */

/* Define to 2 if the system does not provide POSIX.1 features except with
   this defined. */
/* #undef _POSIX_1_SOURCE */

/* Define to 1 if you need to in order for `stat' and other things to work. */
/* #undef _POSIX_SOURCE */

/* in_port_t */
/* #undef in_port_t */

/* sa_family_t */
/* #undef sa_family_t */
