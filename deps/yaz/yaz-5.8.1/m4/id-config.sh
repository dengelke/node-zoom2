# To be included by ../buildconf.sh .
automake=automake
aclocal=aclocal
autoconf=autoconf
libtoolize=libtoolize
autoheader=autoheader

test -d autom4te.cache && rm -r autom4te.cache
test -d config || mkdir config
if [ "`uname -s`" = FreeBSD ]; then
    # FreeBSD intalls the various auto* tools with version numbers
    echo "Using special configuration for FreeBSD ..."
    automake=automake
    aclocal="aclocal -I /usr/local/share/aclocal"
    autoconf=autoconf
    libtoolize=libtoolize
    autoheader=autoheader
fi

if [ "`uname -s`" = Darwin ]; then
    echo "Using special configuration for Darwin/MacOS ..."
    libtoolize=glibtoolize
fi

if $automake --version|head -1 |grep ' 1\.[4-7]'; then
    echo "automake 1.4-1.7 is active. You should use automake 1.8 or later"
    if [ -f /etc/debian_version ]; then
        echo " sudo apt-get install automake1.9"
        echo " sudo update-alternatives --config automake"
    fi
    exit 1
fi

set -x
$aclocal -I m4
if grep AC_CONFIG_HEADERS configure.ac >/dev/null; then
    $autoheader
fi
if grep AM_PROG_LIBTOOL configure.ac >/dev/null; then
    has_libtool=true
    $libtoolize --automake --force 
else
    has_libtool=false
fi

$automake --add-missing 
$autoconf
set -
if [ -f config.cache ]; then
	rm config.cache
fi

enable_configure=false
enable_help=true
sh_flags=""
conf_flags=""
case $1 in
    -d)
	sh_cflags="-g -Wall -Wdeclaration-after-statement -Wstrict-prototypes"
	sh_cxxflags="-g -Wall"
	enable_configure=true
	enable_help=false
	shift
	;;
    -c)
	sh_cflags=""
	sh_cxxflags=""
	enable_configure=true
	enable_help=false
	shift
	;;
esac

if $enable_configure; then
    if [ -n "$sh_cflags" ]; then
	if $has_libtool; then
	    CFLAGS="$sh_cflags" CXXFLAGS="$sh_cxxflags" ./configure \
		--disable-shared --enable-static --with-pic $*
        else
	    CFLAGS="$sh_cflags" CXXFLAGS="$sh_cxxflags" ./configure $*
        fi
    else
	./configure $*
    fi
fi
