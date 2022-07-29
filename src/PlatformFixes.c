
#include <stdio.h>
#include "PlatformFixes.h"

#ifndef _WIN32
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"

#include "vendor/safeclib/src/str/strcpy_s.c"
#include "vendor/safeclib/src/str/strncat_s.c"
#include "vendor/safeclib/src/str/strncpy_s.c"
#include "vendor/safeclib/src/str/strnlen_s.c"
#include "vendor/safeclib/src/str/strtok_s.c"
#include "vendor/safeclib/src/str/sprintf_s.c"
#include "vendor/safeclib/src/str/strcat_s.c"
#include "vendor/safeclib/src/str/strerror_s.c"
#include "vendor/safeclib/src/str/vsnprintf_s.c"
#include "vendor/safeclib/src/str/safe_str_constraint.c"
#define HAVE_WCHAR_H
#include "vendor/safeclib/src/wchar/wcstombs_s.c"
#include "vendor/safeclib/src/wchar/wcsnlen_s.c"

#include "vendor/safeclib/src/ignore_handler_s.c"
#include "vendor/safeclib/src/abort_handler_s.c"



#include "vendor/safeclib/src/io/printf_s.c"
#include "vendor/safeclib/src/io/vfprintf_s.c"
#include "vendor/safeclib/src/io/vprintf_s.c"

#include "vendor/safeclib/src/extstr/strstr_s.c"
#pragma GCC diagnostic pop
#endif

time_t getGMT(struct tm* tm) {   /* hey, time_t is local! */
	time_t t = mktime(tm);

	if (t != (time_t)-1 && t != (time_t)0) {
		/* BSD does not have static "timezone" declared */
#if (defined(BSD) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD_kernel__))
		time_t now = time(NULL);
		time_t timezone = -localtime(&now)->tm_gmtoff;
#elif defined(_WIN32) || defined(__ANDROID__)
		if (timezone == -9999)
			timezone = time_offset();
#endif
		return (time_t)(t - timezone);
	}
	return (time_t)-1;
}


int time_offset(void)//https://stackoverflow.com/questions/13804095/get-the-time-zone-gmt-offset-in-c
{
	time_t gmt, rawtime = time(0);
	struct tm* ptm;

#if !defined(_WIN32)
	struct tm gbuf;
	ptm = gmtime_r(&rawtime, &gbuf);
#else
	ptm = gmtime(&rawtime);
#endif
	// Request that mktime() looksup dst in timezone database
	ptm->tm_isdst = -1;
	gmt = mktime(ptm);

	return (int)difftime(rawtime, gmt);
}
