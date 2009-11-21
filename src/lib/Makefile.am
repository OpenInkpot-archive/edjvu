MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libedjvu.la

EDJVUHEADERS = \
Edjvu.h \
edjvu_document.h \
edjvu_page.h \
edjvu_index.h \
edjvu_private.h

installed_headersdir = $(prefix)/include/edjvu
installed_headers_DATA = $(EDJVUHEADERS)

libedjvu_la_SOURCES = \
edjvu_document.c \
edjvu_page.c \
edjvu_misc.c \
edjvu_index.c

libedjvu_la_CPPFLAGS = @EDJVU_CFLAGS@
libedjvu_la_LIBADD = @EDJVU_LIBS@
libedjvu_la_LDFLAGS = -no-undefined -version-info @version_info@

EXTRA_DIST = $(EDJVUHEADERS)
