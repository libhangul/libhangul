#!/bin/sh

aclocal $ACLOCAL_AMFLAGS
autoheader
automake --add-missing
autoconf
