##### http://autoconf-archive.cryp.to/ac_check_icu.html
#
# SYNOPSIS
#
#   AC_CHECK_ICU(version, action-if, action-if-not)
#
# DESCRIPTION
#
#   Defines ICU_LIBS, ICU_CFLAGS, ICU_CXXFLAGS. See icu-config(1) man
#   page.
#
# LAST MODIFICATION
#
#   2005-09-20
#
# COPYLEFT
#
#   Copyright (c) 2005 Akos Maroy <darkeye@tyrell.hu>
#
#   Copying and distribution of this file, with or without
#   modification, are permitted in any medium without royalty provided
#   the copyright notice and this notice are preserved.

AC_DEFUN([AC_CHECK_ICU],[
	succeeded=no
	icudir=default
	AC_ARG_WITH(icu,[  --with-icu[=PREFIX]       use ICU libs in PREFIX],icudir=$withval)
	
	if test "$icudir" != "no"; then
	    if test "$icudir" = "yes" -o "$icudir" = "default"; then
		AC_PATH_PROG(ICU_CONFIG, icu-config, no)
	    else
		ICU_CONFIG=${icudir}/bin/icu-config
	    fi

	    if test -x $ICU_CONFIG; then
		ICU_VERSION=`$ICU_CONFIG --version`
		AC_MSG_CHECKING(for ICU >= $1)
		VERSION_CHECK=`expr $ICU_VERSION \>\= $1`
		if test "$VERSION_CHECK" = "1" ; then
		    AC_MSG_RESULT(yes)
		    succeeded=yes
		    
		    ICU_PREFIX=`$ICU_CONFIG --prefix`
		    
		    AC_MSG_CHECKING(ICU_CPPFLAGS)
		    ICU_CPPFLAGS=`$ICU_CONFIG --cppflags-searchpath`""
		    if test "$ICU_PREFIX" = "/usr"; then
		       ICU_CPPFLAGS=`echo $ICU_CPPFLAGS|sed 's@-I/usr/include@@'`
		    fi
		    AC_MSG_RESULT($ICU_CPPFLAGS)
		    
		    AC_MSG_CHECKING(ICU_CFLAGS)
		    ICU_CFLAGS=`$ICU_CONFIG --cflags`
		    AC_MSG_RESULT($ICU_CFLAGS)
		    
		    AC_MSG_CHECKING(ICU_CXXFLAGS)
		    ICU_CXXFLAGS=`$ICU_CONFIG --cxxflags`
		    AC_MSG_RESULT($ICU_CXXFLAGS)
		    
		    AC_MSG_CHECKING(ICU_LIBS)
		    if test "$ICU_PREFIX" = "/usr"; then
		        ICU_LIBS=`$ICU_CONFIG --ldflags-libsonly`
	            else
		        ICU_LIBS=`$ICU_CONFIG --ldflags`
		    fi
		    case $host_os in
			solaris*)
			    AC_CHECK_LIB([Crun],[_fini],[ICU_LIBS="$ICU_LIBS -lCrun"])
			    ;;
		    esac
		    AC_MSG_RESULT($ICU_LIBS)
		else
		    ICU_CPPFLAGS=""
		    ICU_CFLAGS=""
		    ICU_CXXFLAGS=""
		    ICU_LIBS=""
		    AC_MSG_RESULT([can not find ICU >= $1])
		fi
		
		AC_SUBST(ICU_CPPFLAGS)
		AC_SUBST(ICU_CFLAGS)
		AC_SUBST(ICU_CXXFLAGS)
		AC_SUBST(ICU_LIBS)
	    fi
	    if test "$succeeded" = "no"; then
		if test "$icudir" != "default"; then
		    AC_MSG_ERROR([libicu development libraries not found.])
		fi
	    fi
	fi
	if test "$succeeded" = "yes"; then
	    ifelse([$2], , :, [$2])
	else
	    ifelse([$3], , :, [$3])
	fi
    ])

dnl Local Variables:
dnl mode:shell-script
dnl sh-indentation:2
dnl sh-basic-offset: 4
dnl End:
