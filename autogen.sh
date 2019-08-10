#!/bin/sh


test -f ChangeLog || touch ChangeLog

aclocal $ACLOCAL_AMFLAGS
autoreconf --force --install
autoheader
automake --add-missing --copy
autoconf
