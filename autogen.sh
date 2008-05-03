#!/bin/sh


test -f ChangeLog || touch ChangeLog

if libtoolize --version | grep GNU > /dev/null 2>&1 ; then
    libtoolize=libtoolize
else
    libtoolize=glibtoolize
fi

$libtoolize --automake
aclocal $ACLOCAL_AMFLAGS
autoheader
automake --add-missing
autoconf
