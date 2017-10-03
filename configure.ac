# Simplified configure.ac for use with basically any autoconf version
# written by Manuel Kessler

dnl Process this file with autoconf to produce a configure script.

AC_INIT(cgnspp, 0.9.6)

AC_CONFIG_AUX_DIR(admin)

dnl Checking host/target/build systems, for make, install etc.
AC_CANONICAL_SYSTEM
dnl Perform program name transformation
#AC_ARG_PROGRAM

dnl Automake doc recommends to do this only here. (Janos)
AM_INIT_AUTOMAKE

dnl create a working libtool-script
#AM_PROG_LIBTOOL

AC_PREFIX_DEFAULT(/usr/local)
#AC_CONFIG_HEADERS(include/config.h)

AC_CONFIG_FILES([ Makefile ])
AC_CONFIG_FILES([ adfpp/Makefile ])
AC_CONFIG_FILES([ adfpp/test/Makefile ])
AC_CONFIG_FILES([ cgnspp/Makefile ])
AC_CONFIG_FILES([ cgnspp/docs/Makefile ])
AC_CONFIG_FILES([ cgnspp/docs/en/Makefile ])
AC_CONFIG_FILES([ cgnspp/test/Makefile ])
AC_CONFIG_FILES([ cgnspp/example/Makefile ])

dnl checks for programs.
dnl first check for c/c++ compilers
AC_PROG_CC
AC_PROG_CXX

dnl CXXFLAGS="$NOOPT_CXXFLAGS" dnl __kdevelop[noopt]__
dnl CFLAGS="$NOOPT_CFLAGS" dnl __kdevelop[noopt]__
dnl CXXFLAGS="$CXXFLAGS $USE_EXCEPTIONS" dnl __kdevelop[exc]__

AC_LANG_CPLUSPLUS

AC_MSG_CHECKING([for strncasecmp])
AC_TRY_LINK([#include <string.h>],
	[int result=strncasecmp("test", "TeStEr", 4);],
	[ac_cv_have_strncasecmp="yes"],
	[ac_cv_have_strncasecmp="no"])
AC_MSG_RESULT($ac_cv_have_strncasecmp)
if test "$ac_cv_have_strncasecmp" = "yes"; then
	AC_DEFINE(HAVE_STRNCASECMP, 1, [Define if the strncasecmp function
is available])
fi

AC_MSG_CHECKING([for MIPSPro compiler])
AC_TRY_COMPILE([
	#ifndef __sgi
	#error No MIPSPro
	The !@"#%&$ SUN C++ compiler gives only a warning for #error
	#endif
	#ifdef __GNUC__
	#error GNU CC
	#endif
	],[],
	[ac_cv_have_mipspro="yes"],
	[ac_cv_have_mipspro="no"])
AC_MSG_RESULT($ac_cv_have_mipspro)
if test "$ac_cv_have_mipspro" = "yes"; then
	AC_DEFINE(HAVE_MIPSPRO, 1, [Define if we use the MIPSPro C++
compiler])
dnl can't use CPPFLAGS because it is used in the linking stage, giving a
libtool error	
dnl	CPPFLAGS="${CPPFLAGS} -LANG:std -ptused -FE:template_in_elf_section"
	CPPFLAGS="${CPPFLAGS} -LANG:std"
	AR="../ar-mips"
	CXXLD="${CXX} -LANG:std"
else
	CXXLD="${CXX}"
fi
AC_SUBST(CXXLD)

AC_MSG_CHECKING([for SUNPro compiler])
AC_TRY_COMPILE([
	#ifndef __SUNPRO_CC
	#error No SUNPro
	The !@"#%&$ SUN C++ compiler gives only a warning for #error
	#endif
	],[],
	[ac_cv_have_sunpro="yes"],
	[ac_cv_have_sunpro="no"])
AC_MSG_RESULT($ac_cv_have_sunpro)
if test "$ac_cv_have_sunpro" = "yes"; then
	AC_DEFINE(HAVE_SUNPRO, 1, [Define if we use the SUNPro C++
compiler])
	AR="../ar-sun"
fi
if test "$AR" = ""; then
	AR="ar"
fi
AC_SUBST(AR)

AC_MSG_CHECKING([for long long support])
AC_TRY_COMPILE([],
	[long long ll = 0;],
	[ac_cv_have_long_long="yes"],
	[ac_cv_have_long_long="no"])
AC_MSG_RESULT($ac_cv_have_long_long)
if test "$ac_cv_have_long_long" = "yes"; then
	AC_DEFINE(HAVE_LONG_LONG, 1, [Define if the C++ compiler supports
long long])
fi

AC_MSG_CHECKING([for long long stream support])
AC_TRY_LINK([#include <iostream>],
	[std::cout << static_cast<long long>(0); ],
	[ac_cv_output_long_long="yes"],
	[ac_cv_output_long_long="no"])
AC_MSG_RESULT($ac_cv_output_long_long)
if test "$ac_cv_output_long_long" = "yes"; then
	AC_DEFINE(OUTPUT_LONG_LONG, 1, [Define if the C++ compiler can
output long long])
fi

AC_MSG_CHECKING([for standard conforming C headers])
AC_TRY_LINK([#include <cstring>],
	[std::strncpy(reinterpret_cast<char *>(0), "", 1); ],
	[ac_cv_std_c_header="yes"],
	[ac_cv_std_c_header="no"])
AC_MSG_RESULT($ac_cv_std_c_header)
if test "$ac_cv_std_c_header" = "yes"; then
	AC_DEFINE(HAVE_STD_C_HEADER, 1, [Define if the C++ compiler has
standard conforming C headers])
fi

AC_MSG_CHECKING([for explicit instantiation of functions in namespaces])
AC_TRY_COMPILE([namespace N { template <typename T> void f(T t) {} }
	template void N::f<int>(int);], [],
	[ac_cv_have_namespace_instantiation="yes"],
	[ac_cv_have_namespace_instantiation="no"])
AC_MSG_RESULT($ac_cv_have_namespace_instantiation)
if test "$ac_cv_have_namespace_instantiation" = "yes"; then
	AC_DEFINE(HAVE_NAMESPACE_INSTANTIATION, 1,
		[Define if the C++ compiler can explicit instantiate
functions in namespaces])
fi

AC_MSG_CHECKING([for using hidden base members])
AC_TRY_COMPILE([struct B { void f(int); };
	struct D : B { using B::f; void f(int, long); };],
	[D d; d.f(0, 0L);],
	[ac_cv_have_using_base_members="yes"],
	[ac_cv_have_using_base_members="no"])
AC_MSG_RESULT($ac_cv_have_using_base_members)
if test "$ac_cv_have_using_base_members" = "yes"; then
	AC_DEFINE(HAVE_USING_BASE_MEMBERS, 1,
		[Define if the C++ compiler can pull in hidden base members
by using])
fi

AC_MSG_CHECKING([for template template parameters])
AC_TRY_COMPILE([struct S { template <typename T, template<typename> class C>
static void f(T & t, C<T> &c) {} };
	template<typename T> class C {};],
	[int i; C<int> c; S::f(i, c); ],
	[ac_cv_have_template_templates="yes"],
	[ac_cv_have_template_templates="no"])
AC_MSG_RESULT($ac_cv_have_template_templates)
if test "$ac_cv_have_template_templates" = "yes"; then
	AC_DEFINE(HAVE_TEMPLATE_TEMPLATES, 1,
		[Define if the C++ compiler supports template template
parameters])
fi

AC_PROG_RANLIB

#AC_SUBST(AUTODIRS)

AC_OUTPUT
