AC_PREREQ([2.68])
AC_INIT([remotee], [0.1], [lmi@ift.uni.wroc.pl])
AM_INIT_AUTOMAKE

AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB

AC_LANG([C++])

REMOTEE_CXXFLAGS="-Wall -Wextra -pedantic -std=c++11"

PKG_PROG_PKG_CONFIG(0.25)

PKG_CHECK_MODULES(jansson, [jansson >= 2.3],
                           [REMOTEE_CXXFLAGS="$REMOTEE_CXXFLAGS $jansson_CFLAGS";
                            REMOTEE_LDFLAGS="$REMOTEE_LDFLAGS $jansson_LIBS"])

PKG_CHECK_MODULES(libcurl, [libcurl >= 7.18],
                           [REMOTEE_CXXFLAGS="$REMOTEE_CXXFLAGS $libcurl_CFLAGS";
                            REMOTEE_LDFLAGS="$REMOTEE_LDFLAGS $libcurl_LIBS"])

PKG_CHECK_MODULES(libxml2, [libxml-2.0 >= 2.7],
                           [REMOTEE_CXXFLAGS="$REMOTEE_CXXFLAGS $libxml2_CFLAGS";
                            REMOTEE_LDFLAGS="$REMOTEE_LDFLAGS $libxml2_LIBS"])

PKG_CHECK_MODULES(paludis, [paludis >= 0.72],
                           [REMOTEE_CXXFLAGS="$REMOTEE_CXXFLAGS $paludis_CFLAGS";
                            REMOTEE_LDFLAGS="$REMOTEE_LDFLAGS $paludis_LIBS"])

AC_MSG_CHECKING([for boost.regex])
save_CXXFLAGS="$CXXFLAGS"
save_LIBS="$LIBS"
BOOST_REGEX_LIB=
for lib in boost_regex boost_regex-mt; do
          LIBS="$LIBS -l$lib"
          AC_RUN_IFELSE([AC_LANG_SOURCE([
#include <boost/regex.hpp>

int main()
{
    boost::regex re;
}
          ])],
              [BOOST_REGEX_LIB="-l$lib"
               AC_MSG_RESULT([$BOOST_REGEX_LIB])
               break],
              [LIBS="$save_LIBS"])
done
if test "x$BOOST_REGEX_LIB" = x; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Cannot find boost.regex library.])
fi
REMOTEE_LDFLAGS="$REMOTEE_LDFLAGS $BOOST_REGEX_LIB"

LIBS="$save_LIBS"
CXXFLAGS="$save_CXXFLAGS"

AC_SUBST(REMOTEE_CXXFLAGS)
AC_SUBST(REMOTEE_LDFLAGS)

AC_OUTPUT([Makefile engines/Makefile])
