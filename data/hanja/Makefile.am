
hanjadicdir   = $(datadir)/libhangul/hanja
hanjadic_DATA = hanja.txt

hanjadicbindir   = $(libdir)/libhangul/hanja
hanjadicbin_DATA = hanja.bin

$(hanjadicbin_DATA): hanja.txt ../../tools/hanjac
	../../tools/hanjac $< $@

EXTRA_DIST = $(hanjadic_DATA)
CLEANFILES = $(hanjadicbin_DATA)
