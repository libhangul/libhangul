require 'mkmf'

dir_config('hangul')
find_header('hangul.h', '../../hangul')
have_library('expat')
find_library('hangul', 'hangul_ic_new', '../../hangul/.libs')
create_makefile('hangul')

# vim: set sts=2 sw=2 et:
