
noinst_PROGRAMS = hangul hanja

hangul_CFLAGS = -DTEST_LIBHANGUL_KEYBOARD_PATH=\"${abs_top_builddir}/data/keyboards\"
hangul_SOURCES = hangul.c
hangul_LDADD = ../hangul/libhangul.la $(LTLIBINTL) $(LTLIBICONV)

hanja_CFLAGS = -DTEST_HANJA_TXT=\"${abs_top_srcdir}/data/hanja/hanja.txt\"
hanja_SOURCES = hanja.c
hanja_LDADD = ../hangul/libhangul.la $(LTLIBINTL)

TESTS = test
check_PROGRAMS = test
test_SOURCES = test.c ../hangul/hangul.h
test_CFLAGS =  \
	$(CHECK_CFLAGS) \
	-DTEST_SOURCE_DIR=\"$(abs_srcdir)\" \
	-DTEST_LIBHANGUL_KEYBOARD_PATH=\"${abs_top_builddir}/data/keyboards\" \
	$(NULL)
test_LDADD = $(CHECK_LIBS) ../hangul/libhangul.la $(LTLIBINTL)
