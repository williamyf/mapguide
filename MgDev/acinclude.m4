
dnl MAP_HELP_STRING(LHS, RHS)
dnl Autoconf 2.50 can not handle substr correctly.  It does have 
dnl AC_HELP_STRING, so let's try to call it if we can.
dnl Note: this define must be on one line so that it can be properly returned
dnl as the help string.
AC_DEFUN([MAP_HELP_STRING],[ifelse(regexp(AC_ACVERSION, 2\.1), -1, AC_HELP_STRING($1,$2),[  ]$1 substr([                       ],len($1))$2)])dnl

dnl MAP_SUBST(VARIABLE)
dnl Makes VARIABLE available in generated files
dnl (do not use @variable@ in Makefiles, but $(variable))
AC_DEFUN([MAP_SUBST],[
  MAP_VAR_SUBST="$MAP_VAR_SUBST $1"
  AC_SUBST($1)
])

dnl MAP_FAST_OUTPUT(FILENAME)
dnl Perform substitutions on FILENAME (Makefiles only)
AC_DEFUN([MAP_FAST_OUTPUT],[
  MAP_FAST_OUTPUT_FILES="$MAP_FAST_OUTPUT_FILES $1"
])

dnl MAP_GEN_CONFIG_VARS
dnl Creates config_vars.mk
AC_DEFUN([MAP_GEN_CONFIG_VARS],[
  MAP_SUBST(map_srcdir)
  MAP_SUBST(bindir)
  MAP_SUBST(sbindir)
  MAP_SUBST(phpdir)
  MAP_SUBST(logfiledir)
  MAP_SUBST(libdir)
  MAP_SUBST(libexecdir)
  MAP_SUBST(htdocsdir)
  MAP_SUBST(includedir)
  MAP_SUBST(installbuilddir)
  MAP_SUBST(runtimedir)
  MAP_SUBST(other_targets)
  MAP_SUBST(progname)
  MAP_SUBST(prefix)
  MAP_SUBST(AWK)
  MAP_SUBST(CC)
  MAP_SUBST(CPP)
  MAP_SUBST(CXX)
  MAP_SUBST(CPPFLAGS)
  MAP_SUBST(CFLAGS)
  MAP_SUBST(CXXFLAGS)
  MAP_SUBST(LTFLAGS)
  MAP_SUBST(LDFLAGS)
  MAP_SUBST(LT_LDFLAGS)
  MAP_SUBST(SH_LDFLAGS)
  MAP_SUBST(LIBS)
  MAP_SUBST(DEFS)
  MAP_SUBST(INCLUDES)
  MAP_SUBST(LIBTOOL)
  abs_srcdir="`(cd $srcdir && pwd)`"
 echo creating config_vars.mk
  test -d build || $mkdir_p build
  > config_vars.mk
  for i in $MAP_VAR_SUBST; do
    eval echo "$i = \$$i" >> config_vars.mk
  done
])

dnl # 
dnl # MAP_GENERATE_MAKEFILES
dnl # Creates Makefiles
AC_DEFUN([MAP_GENERATE_MAKEFILES],[
  $SHELL $srcdir/fastgen.sh $srcdir $ac_cv_mkdir_p $MAP_FAST_OUTPUT_FILES
])

dnl ## MAP_OUTPUT(file)
dnl ## adds "file" to the list of files generated by AC_OUTPUT
dnl ## This macro can be used several times.
AC_DEFUN([MAP_OUTPUT], [
  MAP_OUTPUT_FILES="$MAP_OUTPUT_FILES $1"
])

dnl
dnl MAP_DEFINE_TYPE_RLIM_T
dnl
dnl If rlim_t is not defined, define it to int
dnl
AC_DEFUN([MAP_DEFINE_TYPE_RLIM_T], [
  AC_CACHE_CHECK([for rlim_t], ac_cv_type_rlim_t, [
    AC_TRY_COMPILE([
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
], [rlim_t spoon;], [
      ac_cv_type_rlim_t=yes
    ],[ac_cv_type_rlim_t=no
    ])
  ])
  if test "$ac_cv_type_rlim_t" = "no" ; then
      AC_DEFINE(rlim_t, int,
          [Define to 'int' if <sys/resource.h> doesn't define it for us])
  fi
])



dnl NOT YET IMPLEMENTED
dnl MAP_EXPORT_ARGUMENTS
dnl Export (via MAP_SUBST) the various path-related variables that
dnl Autodesk MapGuide Open Source will use while generating scripts
dnl like autoconf and the default config file.

AC_DEFUN([MAP_SUBST_EXPANDED_ARG],[
  APR_EXPAND_VAR(exp_$1, [$]$1)
  MAP_SUBST(exp_$1)
  APR_PATH_RELATIVE(rel_$1, [$]exp_$1, ${prefix})
  MAP_SUBST(rel_$1)
])

AC_DEFUN([MAP_EXPORT_ARGUMENTS],[
  MAP_SUBST_EXPANDED_ARG(exec_prefix)
  MAP_SUBST_EXPANDED_ARG(bindir)
  MAP_SUBST_EXPANDED_ARG(sbindir)
  MAP_SUBST_EXPANDED_ARG(libdir)
  MAP_SUBST_EXPANDED_ARG(libexecdir)
  MAP_SUBST_EXPANDED_ARG(mandir)
  MAP_SUBST_EXPANDED_ARG(sysconfdir)
  MAP_SUBST_EXPANDED_ARG(datadir)
  MAP_SUBST_EXPANDED_ARG(installbuilddir)
  MAP_SUBST_EXPANDED_ARG(errordir)
  MAP_SUBST_EXPANDED_ARG(iconsdir)
  MAP_SUBST_EXPANDED_ARG(htdocsdir)
  MAP_SUBST_EXPANDED_ARG(manualdir)
  MAP_SUBST_EXPANDED_ARG(cgidir)
  MAP_SUBST_EXPANDED_ARG(includedir)
  MAP_SUBST_EXPANDED_ARG(localstatedir)
  MAP_SUBST_EXPANDED_ARG(runtimedir)
  MAP_SUBST_EXPANDED_ARG(logfiledir)
  MAP_SUBST_EXPANDED_ARG(proxycachedir)
])

