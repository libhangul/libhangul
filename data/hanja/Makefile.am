
hanjadicdir   = $(datadir)/libhangul/hanja
hanjadic_DATA = hanja.txt

hanjadicbindir   = $(datadir)/libhangul/hanja
hanjadicbin_DATA = hanja.bin

$(hanjadicbin_DATA):
	../../tools/hanjac hanja.txt $@

EXTRA_DIST = $(hanjadic_DATA) $(hanjadicbin_DATA)
