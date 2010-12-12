#!/bin/sh


test -f ChangeLog || touch ChangeLog

libtoolize=`which glibtoolize`
if test -z "$libtoolize"; then
    libtoolize=libtoolize
fi

gettextize --force --no-changelog
$libtoolize --automake
aclocal $ACLOCAL_AMFLAGS
autoheader
automake --add-missing
autoconf
