
keyboardsdir   = $(pkgdatadir)/keyboards
keyboards_DATA = \
	hangul-keyboard-2.xml \
	hangul-keyboard-2y.xml \
	hangul-keyboard-39.xml \
	hangul-keyboard-3f.xml \
	hangul-keyboard-32.xml \
	hangul-keyboard-3s.xml \
	hangul-keyboard-3y.xml \
	hangul-keyboard-ro.xml \
	hangul-keyboard-ahn.xml \
	hangul-combination-default.xml \
	hangul-combination-full.xml \
	$(NULL)

EXTRA_DIST = \
	hangul-keyboard-2.xml.template \
	hangul-keyboard-2y.xml.template \
	hangul-keyboard-39.xml.template \
	hangul-keyboard-3f.xml.template \
	hangul-keyboard-32.xml.template \
	hangul-keyboard-3s.xml.template \
	hangul-keyboard-3y.xml.template \
	hangul-keyboard-ro.xml.template \
	hangul-keyboard-ahn.xml.template \
	hangul-combination-default.xml \
	hangul-combination-full.xml \
	its/hangul-keyboard.its \
	its/hangul-keyboard.loc \
	$(NULL)

# its 폴더의 *.its, *.loc rule을 사용하기 위해 GETTEXTDATADIRS 지정
MSGFMT_COMMAND = env GETTEXTDATADIRS=$(srcdir) $(MSGFMT)

# msgfmt으로 xml 파일을 번역하면 들여쓰기 크기가 바뀌고 같은 줄에 달린 주석
# 앞에 줄바꿈이 추가된다. 이를 방지하고자 root node와 name 부분만 별도의
# name.xml로 만들어 (msgfmt 입출력은 valid XML만 가능하므로 root node도 포함.)
# 번역한후 template과 name.xml을 병합하여 키보드 xml 파일을 생성한다.
hangul-keyboard-%.xml: hangul-keyboard-%.name.xml hangul-keyboard-%.xml.template
	sed -i.tmp -e '1 D' \
		-e '/^<hangul-keyboard/D' \
		-e '/^<\/hangul-keyboard/D' \
		-e 's/^ *<name/    <name/'  $<
	sed \
		-e '/<name>/r $<' \
		-e '/<name>/D' \
		$(srcdir)/$@.template > $@

hangul-keyboard-%.name.xml.in: hangul-keyboard-%.xml.template
	grep "xml version=" $< > $@
	grep "<hangul-keyboard" $< >> $@
	grep "<name>" $< >> $@
	grep "</hangul-keyboard" $< >> $@

hangul-keyboard-%.name.xml: hangul-keyboard-%.name.xml.in Makefile
	$(AM_V_GEN)$(MSGFMT_COMMAND) --xml --template $< -d $(top_srcdir)/po -o $@

CLEANFILES = \
	hangul-keyboard-2.xml \
	hangul-keyboard-2y.xml \
	hangul-keyboard-39.xml \
	hangul-keyboard-3f.xml \
	hangul-keyboard-32.xml \
	hangul-keyboard-3s.xml \
	hangul-keyboard-3y.xml \
	hangul-keyboard-ro.xml \
	hangul-keyboard-ahn.xml \
	hangul-keyboard-2.name.xml.tmp \
	hangul-keyboard-2y.name.xml.tmp \
	hangul-keyboard-39.name.xml.tmp \
	hangul-keyboard-3f.name.xml.tmp \
	hangul-keyboard-32.name.xml.tmp \
	hangul-keyboard-3s.name.xml.tmp \
	hangul-keyboard-3y.name.xml.tmp \
	hangul-keyboard-ro.name.xml.tmp \
	hangul-keyboard-ahn.name.xml.tmp \
	$(NULL)
