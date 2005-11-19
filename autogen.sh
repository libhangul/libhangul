#!/bin/sh

libtoolize --automake
aclocal $ACLOCAL_AMFLAGS
autoheader
automake --add-missing
autoconf
