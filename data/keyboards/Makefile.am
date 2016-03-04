
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
	$(NULL)

# intltool로 xml 파일을 번역하면 주석이 모두 사라지고 attr의 순서도
# 재정렬된다. 이를 방지하고자 name 부분만 별도의 name.xml로 만들어
# 번역한후 template과 name.xml을 병합하여 키보드 xml 파일을 생성한다.
hangul-keyboard-%.xml: hangul-keyboard-%.name.xml hangul-keyboard-%.xml.template
	sed -i -e '1 D' -e 's/^<name/    <name/' $<
	sed \
		-e '/<_name>/r $<' \
		-e '/<_name>/a\ ' \
		-e '/<_name>/D' \
		$(srcdir)/$@.template > $@

hangul-keyboard-%.name.xml.in: hangul-keyboard-%.xml.template
	grep "xml version=" $< > $@
	grep "<_name>" $< >> $@

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
	$(NULL)

@INTLTOOL_XML_RULE@
