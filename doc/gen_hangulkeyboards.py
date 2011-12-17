#!/usr/bin/env python
# coding=utf-8

import sys

table = {}

current_name = ''
current_table = []

src = open('../hangul/hangulkeyboard.h', 'r')
for i in src.readlines():
	pos = i.find('hangul_keyboard_table_')
	if pos >= 0:
		end = i.find('[')
		current_name = i[pos + 22:end];
		continue
		
	pos = i.find('}')
	if pos >= 0:
		if len(current_name) > 0:
			table[current_name] = current_table
			current_name = ''
			current_table = []
		continue

	if len(current_name) > 0:
		code = int(i.split()[0].replace(',', ''), 16)
		current_table.append(code)

def jamo_to_compat_jamo(code):
	table = [
		0x3131, 0x3132, 0x3134, 0x3137,
		0x3138, 0x3139, 0x3141, 0x3142,
		0x3143, 0x3145, 0x3146, 0x3147,
		0x3148, 0x3149, 0x314a, 0x314b,
		0x314c, 0x314d, 0x314e, 0x0000,
		0x3165, 0x3166, 0x0000, 0x0000,
		0x0000, 0x0000, 0x3140, 0x0000,
		0x316e, 0x3171, 0x3172, 0x0000,
		0x3173, 0x3144, 0x3174, 0x3175,
		0x0000, 0x0000, 0x0000, 0x3176,
		0x0000, 0x3177, 0x0000, 0x3178,
		0x3179, 0x317a, 0x317b, 0x317c,
		0x0000, 0x0000, 0x317d, 0x0000,
		0x0000, 0x0000, 0x317e, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x317f, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x3180,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x3181, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x3184,
		0x3185, 0x3186, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x3164, 0x314f, 0x3150, 0x3151,
		0x3152, 0x3153, 0x3154, 0x3155,
		0x3156, 0x3157, 0x3158, 0x3159,
		0x315a, 0x315b, 0x315c, 0x315d,
		0x315e, 0x315f, 0x3160, 0x3161,
		0x3162, 0x3163, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x3187, 0x3188, 0x0000, 0x0000,
		0x3189, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x318a, 0x318b, 0x0000,
		0x318c, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x318d, 0x0000,
		0x0000, 0x318e, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x3131, 0x3132, 0x3133, 0x3134,
		0x3135, 0x3136, 0x3137, 0x3139,
		0x313a, 0x313b, 0x313c, 0x313d,
		0x313e, 0x313f, 0x3140, 0x3141,
		0x3142, 0x3144, 0x3145, 0x3146,
		0x3147, 0x3148, 0x314a, 0x314b,
		0x314c, 0x314d, 0x314e, 0x0000,
		0x0000, 0x0000, 0x0000, 0x3167,
		0x3168, 0x0000, 0x0000, 0x0000,
		0x3169, 0x0000, 0x316a, 0x0000,
		0x0000, 0x0000, 0x0000, 0x316b,
		0x0000, 0x0000, 0x0000, 0x316c,
		0x0000, 0x316d, 0x0000, 0x0000,
		0x0000, 0x316f, 0x0000, 0x3170,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x3182, 0x3183, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000,
		0x0000, 0x0000, 0x0000, 0x0000,
	]

	ret = 0
	if code >= 0x1100 and code <= 0x11ff:
		ret = table[code - 0x1100]

	if ret == 0:
		return code
	else:
		return ret

def is_choseong(code):
	if code >= 0x1100 and code <= 0x115f:
		return True
	else:
		return False

def is_jungseong(code):
	if code >= 0x1160 and code <= 0x11a7:
		return True
	else:
		return False

def is_jongseong(code):
	if code >= 0x11a8 and code <= 0x11ff:
		return True
	else:
		return False

def to_html_entity(code):
	if is_choseong(code):
		return '&#%d;&#%d;' % (code, 0x1160)
	elif is_jungseong(code):
		return '&#%d;&#%d;' % (0x115f, code)
	elif is_jongseong(code):
		return '&#%d;&#%d;&#%d;' % (0x115f, 0x1160, code)
	else:
		return '&#%d;' % code
#	if is_choseong(code):
#		return (unichr(code) + u'\x1160').encode('utf-8')
#	elif is_jungseong(code):
#		return (u'\0x115f' + unichr(code)).encode('utf-8')
#	elif is_jongseong(code):
#		return (u'\x115f\x1160' + unichr(code)).encode('utf-8')
#	else:
#		return unichr(code).encode('utf-8')

def to_choseong_label(s):
	return '<span class="choseong">%s</span>' % s

def to_jungseong_label(s):
	return '<span class="jungseong">%s</span>' % s

def to_jongseong_label(s):
	return '<span class="jongseong">%s</span>' % s

def to_regular_label(s):
	return '<span class="other">%s</span>' % s

def make_label(code):
	c = jamo_to_compat_jamo(code)
	s = to_html_entity(c);

	if is_choseong(code):
		return to_choseong_label(s)
	elif is_jungseong(code):
		return to_jungseong_label(s)
	elif is_jongseong(code):
		return to_jongseong_label(s)
	else:
		return to_regular_label(s)

def print_empty_cell():
	print '<td class="emptycell"></td>'

def print_cell_with_label(label, span):
	print '<td class="graykeycell" colspan="%d">%s</td>' % (span, label)

def print_cell_with_table(table, upper_index, lower_index, span = 4):
	upper = table[upper_index];
	lower = table[lower_index];	

	if upper == lower:
		upper = ord(' ')

	print '<td class="keycell" colspan="%d">%s<br/>%s</td>' % (span, make_label(upper), make_label(lower))

def print_keyboard_table(table):
	print ''
	print '@htmlonly'
	print ''
	print '<table class="keyboard">'

	print '<tr>'
	for i in range(0, 60):
		print_empty_cell()
	print '</tr>'

	# 1st row
	print '<tr>'
	print_cell_with_table(table, 0x7e, 0x60)
	print_cell_with_table(table, 0x21, 0x31)
	print_cell_with_table(table, 0x40, 0x32)
	print_cell_with_table(table, 0x23, 0x33)
	print_cell_with_table(table, 0x24, 0x34)
	print_cell_with_table(table, 0x25, 0x35)
	print_cell_with_table(table, 0x5e, 0x36)
	print_cell_with_table(table, 0x26, 0x37)
	print_cell_with_table(table, 0x2a, 0x38)
	print_cell_with_table(table, 0x28, 0x39)
	print_cell_with_table(table, 0x29, 0x30)
	print_cell_with_table(table, 0x5f, 0x2d)
	print_cell_with_table(table, 0x2b, 0x3d)
	print_cell_with_label('Backspace', 8)
	print '</tr>'

	# 2nd row
	print '<tr>'
	print_cell_with_label('Tab', 6)
	print_cell_with_table(table, 0x51, 0x71)
	print_cell_with_table(table, 0x57, 0x77)
	print_cell_with_table(table, 0x45, 0x65)
	print_cell_with_table(table, 0x52, 0x72)
	print_cell_with_table(table, 0x54, 0x74)
	print_cell_with_table(table, 0x59, 0x79)
	print_cell_with_table(table, 0x55, 0x75)
	print_cell_with_table(table, 0x49, 0x69)
	print_cell_with_table(table, 0x4f, 0x6f)
	print_cell_with_table(table, 0x50, 0x70)
	print_cell_with_table(table, 0x7b, 0x5b)
	print_cell_with_table(table, 0x7d, 0x5d)
	print_cell_with_table(table, 0x7c, 0x5c, 6)
	print '</tr>'

	# 3rd row
	print '<tr>'
	print_cell_with_label('Caps Lock', 8)
	print_cell_with_table(table, 0x41, 0x61)
	print_cell_with_table(table, 0x53, 0x73)
	print_cell_with_table(table, 0x44, 0x64)
	print_cell_with_table(table, 0x46, 0x66)
	print_cell_with_table(table, 0x47, 0x67)
	print_cell_with_table(table, 0x48, 0x68)
	print_cell_with_table(table, 0x4a, 0x6a)
	print_cell_with_table(table, 0x4b, 0x6b)
	print_cell_with_table(table, 0x4c, 0x6c)
	print_cell_with_table(table, 0x3a, 0x3b)
	print_cell_with_table(table, 0x22, 0x27)
	print_cell_with_label('Enter', 8)
	print '</tr>'

	# 4th row
	print '<tr>'
	print_cell_with_label('Shift', 10)
	print_cell_with_table(table, 0x5a, 0x7a)
	print_cell_with_table(table, 0x58, 0x78)
	print_cell_with_table(table, 0x43, 0x63)
	print_cell_with_table(table, 0x56, 0x76)
	print_cell_with_table(table, 0x42, 0x62)
	print_cell_with_table(table, 0x4e, 0x6e)
	print_cell_with_table(table, 0x4d, 0x6d)
	print_cell_with_table(table, 0x3c, 0x2c)
	print_cell_with_table(table, 0x3e, 0x2e)
	print_cell_with_table(table, 0x3f, 0x2f)
	print_cell_with_label('Shift', 10)
	print '</tr>'

	# 5th row
	print '<!--'
	print '<tr>'
	print_cell_with_label('Ctrl', 5)
	print_cell_with_label('Super', 4)
	print_cell_with_label('Alt', 5)
	print_cell_with_label('한자', 4)
	print_cell_with_label('&nbsp;<br/>&nbsp;', 20)
	print_cell_with_label('한/영', 4)
	print_cell_with_label('Alt', 5)
	print_cell_with_label('Super', 4)
	print_cell_with_label('Menu', 4)
	print_cell_with_label('Ctrl', 5)
	print '</tr>'
	print '-->'
	print '</table>'
	print ''
	print '@endhtmlonly'
	print ''


print '''/**

@defgroup hangulkeyboards 한글 자판

libhangul에서 builtin으로 지원하는 자판은 @ref layout_2, @ref layout_2y,
@ref layout_3f, @ref layout_390, @ref layout_3s, @ref layout_2y,
@ref layout_3y, @ref layout_ahn, @ref layout_ro 자판 이다.

아래의 자판 배열은 libhangul에서 지원하는 자판을 알아보기 쉽게 그림형태로
나타낸 것이다.
@li 각 네모에 표시된 글자중 위에 있는 글자는 shift와 함께 눌렀을때
입력되는 글자고 아래에 있는 글자는 그냥 눌렀을때 입력되는 글자다.
@li 초성, 중성, 종성은 각각 붉은색, 녹색, 파란색(RGB 순서)으로 표시하였다.
@li 한글이 아닌 것은 검은색으로 표시하였다.

@htmlonly

<style type="text/css">

.choseong {
	color: darkred;
}

.jungseong {
	color: darkgreen;
}

.jongseong {
	color: darkblue;
}

.other {
	color: black;
}

table.keyboard {
	font-family: sans-serif;
	font-size: 120%;
	border: 0px solid darkgray;
	padding: 3px;
	border-collapse: collapse;
	border-collapse: separate;
}

td.emptycell {
	width: 8px;
	height: -3px;
	padding: 0px;
	margin: 0px;
	border-style: none;
}

td.keycell {
	text-align: center;
	line-height: 110%;
	background: whitesmoke;
	padding: 2px;
	margin: 1px;
	border: 1px solid dimgray;
	//border-width: 0px 1px 1px 0px;
}

td.graykeycell {
	font-size: small;
	text-align: center;
	line-height: 110%;
	color: lightgray;
	background-color: white;
	padding: 2px;
	margin: 0px;
	border: 2px solid lightgray;
	border-width: 1px 1px 1px 1px;
}

</style>

@endhtmlonly

'''

print '''@section layout_2 두벌식
이 자판은 표준 두벌식을 구현한 것이다. 그러나 표준 자판과 조금 다른 동작이
있다. 된소리 ㅃㅉㄸㄲㅆ들은 shift와 함께 누르는 방법 이외에도 반복해서
누르면 입력된다.
'''
print_keyboard_table(table['2'])
print ''

print '''@section layout_2y 두벌식 옛글
이 자판은 옛한글을 입력하기 위한 두벌식 자판이다. 아래한글의 두벌식 옛글 자판과
같은 배열을 가지도록 만들었다. @ref layout_2 자판과 마찬가지로 된소리를 두번
누르는 방법으로 입력할 수 있다.
'''
print_keyboard_table(table['2y'])
print ''

print '''@section layout_3f 세벌식 최종
이 자판은 세벌식 최종 자판을 구현한 것이다. 원래 방식대로라면 왼쪽의 ㅗㅜ는
다른 모음과 조합되지 않아야 하지만, libhangul에서는 좌우의 ㅗㅜ를 구분하지
않고 다른 모음과 조합된다.
'''
print_keyboard_table(table['3final'])
print ''

print '''@section layout_390 세벌식 390
이 자판은 세벌식 390 자판을 구현한 것이다. 원래 방식대로라면 왼쪽의 ㅗㅜ는
다른 모음과 조합되지 않아야 하지만, libhangul에서는 좌우의 ㅗㅜ를 구분하지
않고 다른 모음과 조합된다.
'''
print_keyboard_table(table['390'])
print ''

print '''@section layout_3s 세벌식 순아래
이 자판은 세벌식 순아래 자판을 구현한 것이다.
'''
print_keyboard_table(table['3sun'])
print ''

print '''@section layout_3y 세벌식 옛글
이 자판은 세벌식 옛글 자판을 구현한 것이다. 자판 배열은 아래한글의 세벌식
옛글 자판과 동일하다.
'''
print_keyboard_table(table['3yet'])
print ''

print '''@section layout_32 세벌식 두벌배열
이 자판은 두벌식 자판의 배열을 그대로 사용하면서 shift와 함께 자음을 누르면
종성으로 입력되도록 만든 자판이다. 일반 사용을 위해 만들어진 것이 아니고
두벌식 사용자가 손쉽게 세벌식 자판의 테스트를 할 수 있도록 하기 위해서
만든 자판이다.
'''
print_keyboard_table(table['32'])
print ''

print '''@section layout_ahn 안마태
이 자판은 안마태 자판을 구현한 것이다. libhangul의 안마태 자판 구현은
안마태 자판의 규격을 충실하게 구현한 것은 아니고 적당한 수준에서 사용가능하게
만든 것이다. 키입력이 동시에 일어났는지 판단하여 입력을 하는 방식이 아니고,
키를 순서대로 입력하여도 자모를 조합하도록 구현하여, 안마태 자판과 유사한
동작을 하도록 구현하였다.
'''
print_keyboard_table(table['ahn'])
print ''

print '''@section layout_ro 로마자
이 자판은 현재 자판 배열의 라틴 문자를 음차하여 한글로 입력되도록 만든 자판이다.
예를 들면 'an'을 치면 '안'이 입력되는 방식이다.
로마자 자판은 다른 한글자판과 달리 <a href="http://en.wikipedia.org/wiki/Transliteration">Transliteration</a> 방식으로 작동하는
자판이므로 별도의 레이아웃이 없다. 이 입력 자판에서 사용한 알파벳과 한글
자모의 대응은 기본적으로 한글 로마자 표기법과 유사하지만
동일하지는 않다. 로마자 자판은 한글을 입력하기 위한 것이므로 한글을 그럴듯
하게 표기하기 위한 로마자 표기법과는 목적이 다르다. 주로 한글 자모를 모두
입력할 수 있도록 하는 것을 우선적으로 고려하였다. 따라서 아래 자판의 대응대로
한글을 로마자로 표기하는 것은 로마자 표기법으로는 적절하지 않다.

한글 자모와 알파벳의 대응은 아래와 같다.
@li ㄱ : g
@li ㄴ : n
@li ㄷ : d
@li ㄹ : r, l
@li ㅁ : m
@li ㅂ : b, v
@li ㅅ : s
@li ㅇ : ng
@li ㅈ : j
@li ㅊ : c, ch
@li ㅋ : k, q
@li ㅌ : t
@li ㅍ : f, p
@li ㅎ : h

@li ㄲ : gg
@li ㄸ : dd
@li ㅃ : bb
@li ㅆ : ss
@li ㅉ : jj
@li ㅊ : ch

@li ㅏ : a
@li ㅐ : ae, ai
@li ㅑ : ya, ia
@li ㅒ : yae, yai, iae, iai
@li ㅓ : eo
@li ㅔ : e
@li ㅕ : yeo, ieo
@li ㅖ : ye, ie
@li ㅗ : o
@li ㅘ : oa, wa
@li ㅙ : oae, oai
@li ㅚ : oi
@li ㅛ : yo, io
@li ㅜ : u, w
@li ㅝ : ueo,  weo, uo
@li ㅞ : ue
@li ㅟ : ui, wi
@li ㅠ : yu, iu
@li ㅡ : eu
@li ㅢ : eui
@li ㅣ : i, y

그 외에 다음과 같은 조합 규칙이 있다.

@li 자음없이 모음만 먼저 입력하면 초성 ㅇ이 추가된다.
    예를 들어 'a'만 눌러도 '아'로 입력된다.
@li ng 는 종성 ㅇ으로 입력된다.
@li x는 음절의 시작에서는 초성 ㅈ, 끝에서는 종성 ㄳ으로 입력된다.
@li 종성의 자음군은 앞음절의 글자로 입력된다.
    예를 들어 'banga'는 '방아'로 입력된다.
@li 대문자를 입력하면 음절의 시작으로 인식한다.
    예를 들어 'banga'는 '방아'로, 'banGa'는 '반가'로 입력된다.
    'beos'는 '벗'으로 'beOs'는 '베옷'으로 입력된다.
@li 자음군으로 음절이 시작되면 ㅡ를 자동으로 추가한다.
    예를 들어 'string'을 입력하면 s와 t에 ㅡ가 붙게 되어 '스트링'으로 입력된다.

@sa
다음 위키 페이지를 참조하라. @n
http://en.wikipedia.org/wiki/Korean_romanization @n
http://en.wikipedia.org/wiki/Revised_Romanization_of_Korean

'''
print '*/'
