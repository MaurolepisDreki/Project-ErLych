## Process this file with autoconf to generate ./configure
AC_INIT([Project ErLych], [2019.133])
AC_CONFIG_HEADERS([config.h]) #< config.h.in is created by autoheader
# NOTE TO SELF: use AH_TOP and AH_BOTTOM to prepend or append boilerplate
#  Code appears to be copied verbatim from macro, and GNU recomends using 
#  includes for the boilerplate instead of having autoconf stich the file 
#  together in the name of readability.
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CXX
AM_PROG_AR

LT_INIT #< Use Libtool

# NOTE TO SELF:
#  AC_CONFIG_* take paramaters as [OUT:PRE:PROC:POST], [OUT:PROC], and [OUT]
#  where PRE and POST are copied verbatum as the header and footer of the output
#  and PROC is parsed as usual; if PROC is no provided, it is assumed as /(?=OUT)\.in/

AC_CONFIG_FILES([
	Makefile
	libmidget/Makefile
])
AC_OUTPUT

