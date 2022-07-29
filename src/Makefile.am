DevIncludesdir = $(includedir)/httrack
DevIncludes_DATA = \
	httrack-library.h \
	htsglobal.h \
	htsopt.h \
	htswrap.h \
	htsconfig.h \
	../config.h \
	htsmodules.h \
	htsbasenet.h \
	htsnet.h \
	htsbauth.h \
	htsdefines.h \
	htsstrings.h \
	htsarrays.h \
	htssafe.h

AM_CPPFLAGS = \
	@DEFAULT_CFLAGS@ \
	@THREADS_CFLAGS@ \
	@V6_FLAG@ \
	@LFS_FLAG@ \
	-DPREFIX=\""$(prefix)"\" \
	-DSYSCONFDIR=\""$(sysconfdir)"\" \
	-DDATADIR=\""$(datadir)"\" \
	-DLIBDIR=\""$(libdir)"\" \
	-I"$(abs_srcdir)" \
	-I"$(abs_srcdir)/vendor" \
	-I"$(abs_srcdir)/vendor/coucal" \
	-I"$(abs_srcdir)/vendor/safeclib/src" \
	-I"$(abs_srcdir)/vendor/safeclib/include"

bin_PROGRAMS = proxytrack httrack htsserver

httrack_LDADD = $(THREADS_LIBS) libhttrack.la
htsserver_LDADD = $(THREADS_LIBS) $(SOCKET_LIBS) libhttrack.la
proxytrack_LDADD = $(THREADS_LIBS) $(SOCKET_LIBS)
proxytrack_CFLAGS = $(AM_CFLAGS) -DNO_MALLOCT -DZLIB_CONST -DHTS_INTHASH_USES_MD5
htsserver_CFLAGS = $(AM_CFLAGS) -DZLIB_CONST -DHTS_INTHASH_USES_MD5

lib_LTLIBRARIES = libhttrack.la libhtsjava.la

htsserver_SOURCES = htsserver.c htsserver.h htsweb.c htsweb.h \
	vendor/cJSON/cJSON.c vendor/path-join/path-join.c vendor/path-join/str-ends-with.c vendor/path-join/str-starts-with.c \
	PlatformFixes.c PlatformFixes.h
proxytrack_SOURCES = proxy/main.c \
	proxy/proxytrack.c proxy/store.c \
	vendor/coucal/coucal.c htsmd5.c md5.c \
	PlatformFixes.c \
	vendor/minizip/ioapi.c vendor/minizip/mztools.c vendor/minizip/unzip.c vendor/minizip/zip.c

whttrackrundir = $(bindir)
whttrackrun_SCRIPTS = webhttrack

libhttrack_la_SOURCES =  htscore.c htsparse.c htsback.c htscache.c \
	htscatchurl.c htsfilters.c htsftp.c htshash.c vendor/coucal/coucal.c \
	htshelp.c htslib.c htscoremain.c \
	vendor/cJSON/cJSON.c vendor/path-join/path-join.c vendor/path-join/str-ends-with.c vendor/path-join/str-starts-with.c \
	PlatformFixes.c PlatformFixes.h \
	htsname.c htsrobots.c htstools.c htswizard.c \
	htsalias.c htsthread.c htsindex.c htsbauth.c \
	htsmd5.c htszlib.c htswrap.c htsconcat.c \
	htsmodules.c htscharset.c punycode.c htsencoding.c \
	md5.c \
	vendor/minizip/ioapi.c vendor/minizip/mztools.c vendor/minizip/unzip.c vendor/minizip/zip.c \
	hts-indextmpl.h htsalias.h htsback.h htsbase.h htssafe.h \
	htsbasenet.h htsbauth.h htscache.h htscatchurl.h  \
	htsconfig.h htscore.h htsparse.h htscoremain.h htsdefines.h  \
	htsfilters.h htsftp.h htsglobal.h htshash.h vendor/coucal/coucal.h \
	htshelp.h htsindex.h htslib.h htsmd5.h \
	htsmodules.h htsname.h htsnet.h \
	htsopt.h htsrobots.h htsthread.h  \
	htstools.h htswizard.h htswrap.h htszlib.h  \
	htsstrings.h htsarrays.h httrack-library.h \
	htscharset.h punycode.h htsencoding.h \
	htsentities.h htsentities.sh htsbasiccharsets.sh htscodepages.h \
	md5.h vendor/coucal/murmurhash3.h \
	vendor/minizip/crypt.h vendor/minizip/ioapi.h vendor/minizip/mztools.h vendor/minizip/unzip.h vendor/minizip/zip.h
libhttrack_la_LIBADD = $(THREADS_LIBS) $(ZLIB_LIBS) $(OPENSSL_LIBS) $(DL_LIBS) $(SOCKET_LIBS) $(ICONV_LIBS)
libhttrack_la_CFLAGS = $(AM_CFLAGS) -DLIBHTTRACK_EXPORTS -DZLIB_CONST
libhttrack_la_LDFLAGS = -version-info $(VERSION_INFO)

libhtsjava_la_SOURCES = htsjava.c htsjava.h
libhtsjava_la_LIBADD = $(THREADS_LIBS) $(DL_LIBS) libhttrack.la
libhtsjava_la_LDFLAGS = -version-info $(VERSION_INFO)

EXTRA_DIST = httrack.h webhttrack \
		vendor/coucal/murmurhash3.h.diff \
		vendor/coucal/murmurhash3.h.orig \
		vendor/minizip/iowin32.c \
		vendor/minizip/iowin32.h \
		vendor/minizip/ioapi.c.diff \
		vendor/minizip/ioapi.h.diff \
		vendor/minizip/zip.c.diff \
		vendor/minizip/zip.h.diff \
		vendor/minizip/mztools.c.diff \
		vendor/minizip/ioapi.c.orig \
		vendor/minizip/ioapi.h.orig \
		vendor/minizip/mztools.c.orig \
		vendor/minizip/zip.c.orig \
		vendor/minizip/zip.h.orig \
		vendor/minizip/MiniZip64_Changes.txt \
		vendor/minizip/MiniZip64_info.txt \
		proxy/AUTHORS \
		proxy/COPYING \
		proxy/changelog.txt \
		proxy/proxystrings.h \
		proxy/proxytrack.h \
		proxy/store.h \
		vendor/coucal/* \
		*.dsw *.dsp *.vcproj */*.vcxproj */*.vcxproj.filters
