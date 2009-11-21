AC_PREREQ([2.57])
AC_INIT([edjvu], [0.1], [lunohod@openinkpot.org])

AM_INIT_AUTOMAKE(foreign)

AC_PROG_CC_C99
AC_PROG_LIBTOOL

INTERFACE_CURRENT="0"
INTERFACE_REVISION="1"
INTERFACE_AGE="0"
version_info=${INTERFACE_CURRENT}:${INTERFACE_REVISION}:${INTERFACE_AGE}
AC_SUBST(version_info)

requirements_edjvu="evas ecore"

PKG_CHECK_MODULES(EDJVU, [evas ecore ddjvuapi])
PKG_CHECK_MODULES(ECORE_EVAS, [ecore-evas])

AC_SUBST(requirements_edjvu)

AC_OUTPUT([
Makefile
edjvu.pc
src/Makefile
src/lib/Makefile
])
