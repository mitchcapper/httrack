
SUBDIRS = src man m4 libtest templates lang html tests

ACLOCAL_AMFLAGS = -I m4 --install

EXTRA_DIST = INSTALL.Linux \
		gpl-fr.txt license.txt greetings.txt history.txt \
		httrack-doc.html lang.def README.md \
		config.h.in \
		stamp-h.in \
		tool-versions.txt

DISTCLEANFILES = tool-versions.txt

tool-versions.txt: Makefile
	( automake --version ; echo ; \
	autoconf --version ; echo ; \
	$(CC) --version ; echo ; \
	m4 --version ; echo ; \
	makeinfo --version ) > $@