
hanjadicbindir   = $(libdir)/libhangul/hanja
hanjadicbin_DATA = hanja.bin

$(hanjadicbin_DATA): hanja.txt ../../tools/hanjac
	../../tools/hanjac $< $@

EXTRA_DIST = hanja.txt
CLEANFILES = $(hanjadicbin_DATA)
