#include "hangul.h"

static const wchar_t hangul_keyboard_table_2[] = {
    0x0021,     /* exclam:       exclamation mark               */
    0x0022,     /* quotedbl:     quotation mark                 */
    0x0023,     /* numbersign:   number sign                    */
    0x0024,     /* dollar:       dollar sign                    */
    0x0025,     /* percent:      percent sign                   */
    0x0026,     /* ampersand:    ampersand                      */
    0x0027,     /* apostrophe:   apostrophe                     */
    0x0028,     /* parenleft:    left parenthesis               */
    0x0029,     /* parenright:   right parenthesis              */
    0x002a,     /* asterisk:     asterisk                       */
    0x002b,     /* plus:         plus sign                      */
    0x002c,     /* comma:        comma                          */
    0x002d,     /* minus:        minus sign                     */
    0x002e,     /* period:       period                         */
    0x002f,     /* slash:        slash                          */
    0x0030,     /* 0:            0                              */
    0x0031,     /* 1:            1                              */
    0x0032,     /* 2:            2                              */
    0x0033,     /* 3:            3                              */
    0x0034,     /* 4:            4                              */
    0x0035,     /* 5:            5                              */
    0x0036,     /* 6:            6                              */
    0x0037,     /* 7:            7                              */
    0x0038,     /* 8:            8                              */
    0x0039,     /* 9:            9                              */
    0x003a,     /* colon:        colon                          */
    0x003b,     /* semicolon:    semicolon                      */
    0x003c,     /* less:         less-than sign                 */
    0x003d,     /* equal:        equals sign                    */
    0x003e,     /* greater:      greater-than sign              */
    0x003f,     /* question:     question mark                  */
    0x0040,     /* at:           commercial at                  */
    0x1106,     /* A:            choseong mieum                 */
    0x1172,     /* B:            jungseong yu                   */
    0x110e,     /* C:            choseong chieuch               */
    0x110b,     /* D:            choseong ieung                 */
    0x1104,     /* E:            choseong ssangtikeut           */
    0x1105,     /* F:            choseong rieul                 */
    0x1112,     /* G:            choseong hieuh                 */
    0x1169,     /* H:            jungseong o                    */
    0x1163,     /* I:            jungseong ya                   */
    0x1165,     /* J:            jungseong eo                   */
    0x1161,     /* K:            jungseong a                    */
    0x1175,     /* L:            jungseong i                    */
    0x1173,     /* M:            jungseong eu                   */
    0x116e,     /* N:            jungseong u                    */
    0x1164,     /* O:            jungseong yae                  */
    0x1168,     /* P:            jungseong ye                   */
    0x1108,     /* Q:            choseong ssangpieup            */
    0x1101,     /* R:            choseong ssangkiyeok           */
    0x1102,     /* S:            choseong nieun                 */
    0x110a,     /* T:            choseong ssangsios             */
    0x1167,     /* U:            jungseong yeo                  */
    0x1111,     /* V:            choseong phieuph               */
    0x110d,     /* W:            choseong ssangcieuc            */
    0x1110,     /* X:            choseong thieuth               */
    0x116d,     /* Y:            jungseong yo                   */
    0x110f,     /* Z:            choseong khieukh               */
    0x005b,     /* bracketleft:  left bracket                   */
    0x005c,     /* backslash:    backslash                      */
    0x005d,     /* bracketright: right bracket                  */
    0x005e,     /* asciicircum:  circumflex accent              */
    0x005f,     /* underscore:   underscore                     */
    0x0060,     /* quoteleft:    grave accent                   */
    0x1106,     /* a:            choseong  mieum                */
    0x1172,     /* b:            jungseong yu                   */
    0x110e,     /* c:            choseong  chieuch              */
    0x110b,     /* d:            choseong  ieung                */
    0x1103,     /* e:            choseong  tikeut               */
    0x1105,     /* f:            choseong  rieul                */
    0x1112,     /* g:            choseong  hieuh                */
    0x1169,     /* h:            jungseong o                    */
    0x1163,     /* i:            jungseong ya                   */
    0x1165,     /* j:            jungseong eo                   */
    0x1161,     /* k:            jungseong a                    */
    0x1175,     /* l:            jungseong i                    */
    0x1173,     /* m:            jungseong eu                   */
    0x116e,     /* n:            jungseong u                    */
    0x1162,     /* o:            jungseong ae                   */
    0x1166,     /* p:            jungseong e                    */
    0x1107,     /* q:            choseong  pieup                */
    0x1100,     /* r:            choseong  kiyeok               */
    0x1102,     /* s:            choseong  nieun                */
    0x1109,     /* t:            choseong  sios                 */
    0x1167,     /* u:            jungseong yeo                  */
    0x1111,     /* v:            choseong  phieuph              */
    0x110c,     /* w:            choseong  cieuc                */
    0x1110,     /* x:            choseong  thieuth              */
    0x116d,     /* y:            jungseong yo                   */
    0x110f,     /* z:            choseong  khieukh              */
    0x007b,     /* braceleft:    left brace                     */
    0x007c,     /* bar:          vertical bar                   */
    0x007d,     /* braceright:   right brace                    */
    0x007e      /* asciitilde:   tilde                          */
};

static const wchar_t hangul_keyboard_table_32[] = {
    0x0021,	/* exclam:	exclamation mark		*/
    0x0022,	/* quotedbl:	quotation mark 			*/
    0x0023,	/* numbersign:	number sign	 		*/
    0x0024,	/* dollar:	dollar sign			*/
    0x0025,	/* percent:	percent sign			*/
    0x0026,	/* ampersand:	ampersand		 	*/
    0x0027,	/* apostrophe:	apostrophe			*/
    0x0028,	/* parenleft:	left parenthesis		*/
    0x0029,	/* parenright:	right parenthesis		*/
    0x002a,	/* asterisk:	asterisk			*/
    0x002b,	/* plus:	plus sign			*/
    0x002c,	/* comma:	comma				*/
    0x002d,	/* minus:	minus sign			*/
    0x002e,	/* period:	period				*/
    0x002f,	/* slash:	slash				*/
    0x0030,	/* 0:		0				*/
    0x0031,	/* 1:		1				*/
    0x0032,	/* 2:		2				*/
    0x0033,	/* 3:		3				*/
    0x0034,	/* 4:		4				*/
    0x0035,	/* 5:		5				*/
    0x0036,	/* 6:		6				*/
    0x0037,	/* 7:		7				*/
    0x0038,	/* 8:		8				*/
    0x0039,	/* 9:		9				*/
    0x003a,	/* colon:	colon				*/
    0x003b,	/* semicolon:	semicolon			*/
    0x003c,	/* less:	less-than sign			*/
    0x003d,	/* equal:	equals sign			*/
    0x003e,	/* greater:	greater-than sign		*/
    0x003f,	/* question:	question mark	 		*/
    0x0040,	/* at:		commercial at			*/
    0x11b7,	/* A:		jongseong mieum			*/
    0x1172,	/* B:		jungseong yu			*/
    0x11be,	/* C:		jongseong chieuch		*/
    0x11bc,	/* D:		jongseong ieung			*/
    0x11ae,	/* E:		jongseong tikeut		*/
    0x11af,	/* F:		jongseong rieul			*/
    0x11c2,	/* G:		jongseong hieuh			*/
    0x1169,	/* H:		jungseong o			*/
    0x1163,	/* I:		jungseong ya			*/
    0x1165,	/* J:		jungseong eo			*/
    0x1161,	/* K:		jungseong a			*/
    0x1175,	/* L:		jungseong i			*/
    0x1173,	/* M:		jungseong eu			*/
    0x116e,	/* N:		jungseong u			*/
    0x1164,	/* O:		jungseong yae			*/
    0x1168,	/* P:		jungseong ye			*/
    0x11b8,	/* Q:		jongseong pieup			*/
    0x11a8,	/* R:		jongseong kiyeok		*/
    0x11ab,	/* S:		jongseong nieun			*/
    0x11ba,	/* T:		jongseong sios			*/
    0x1167,	/* U:		jungseong yeo			*/
    0x11c1,	/* V:		jongseong phieuph		*/
    0x11bd,	/* W:		jongseong cieuc			*/
    0x11c0,	/* X:		jongseong thieuth		*/
    0x116d,	/* Y:		jungseong yo			*/
    0x11bf,	/* Z:		jongseong khieukh		*/
    0x002a,	/* bracketleft:	left bracket			*/
    0x005c,	/* backslash:	backslash			*/
    0x005d,	/* bracketright:right bracket			*/
    0x005e,	/* asciicircum:	circumflex accent		*/
    0x005f,	/* underscore:	underscore			*/
    0x0060,	/* quoteleft:	grave accent			*/
    0x1106,	/* a:		choseong  mieum			*/
    0x1172,	/* b:		jungseong yu			*/
    0x110e,	/* c:		choseong  chieuch		*/
    0x110b,	/* d:		choseong  ieung			*/
    0x1103,	/* e:		choseong  tikeut		*/
    0x1105,	/* f:		choseong  rieul			*/
    0x1112,	/* g:		choseong  hieuh			*/
    0x1169,	/* h:		jungseong o			*/
    0x1163,	/* i:		jungseong ya			*/
    0x1165,	/* j:		jungseong eo			*/
    0x1161,	/* k:		jungseong a			*/
    0x1175,	/* l:		jungseong i			*/
    0x1173,	/* m:		jungseong eu			*/
    0x116e,	/* n:		jungseong u			*/
    0x1162,	/* o:		jungseong ae			*/
    0x1166,	/* p:		jungseong e			*/
    0x1107,	/* q:		choseong  pieup			*/
    0x1100,	/* r:		choseong  kiyeok		*/
    0x1102,	/* s:		choseong  nieun			*/
    0x1109, 	/* t:		choseong  sios			*/
    0x1167,	/* u:		jungseong yeo			*/
    0x1111,	/* v:		choseong  phieuph		*/
    0x110c,	/* w:		choseong  cieuc			*/
    0x1110,	/* x:		choseong  thieuth		*/
    0x116d,	/* y:		jungseong yo			*/
    0x110f,	/* z:		choseong  khieukh		*/
    0x007b,	/* braceleft:	left brace			*/
    0x007c,	/* bar:		vertical bar			*/
    0x007d,	/* braceright:	right brace			*/
    0x203b,	/* asciitilde:	tilde				*/
};

static const wchar_t hangul_keyboard_table_390[] = {
    0x11bd,	/* exclam:		jongseong cieuc			*/
    0x0022,	/* quotedbl:	quotatioin mark			*/
    0x0023,	/* numbersign:	number sign	 		*/
    0x0024,	/* dollar:		dollar sign			*/
    0x0025,	/* percent:		percent sign			*/
    0x0026,	/* ampersand:	ampersand		 	*/
    0x1110,	/* apostrophe:	choseong thieuth		*/
    0x0028,	/* parenleft:	left parenthesis		*/
    0x0029,	/* parenright:	right parenthesis		*/
    0x002a,	/* asterisk:	asterisk			*/
    0x002b,	/* plus:		plus sign			*/
    0x002c,	/* comma:		comma				*/
    0x002d,	/* minus:		minus sign			*/
    0x002e,	/* period:		period				*/
    0x1169,	/* slash:		jungseong o			*/
    0x110f,	/* 0:		choseong  khieukh		*/
    0x11c2,	/* 1:		jongseong hieuh			*/
    0x11bb,	/* 2:		jongseong ssangsios		*/
    0x11b8,	/* 3:		jongseong pieup			*/
    0x116d,	/* 4:		jungseong yo			*/
    0x1172,	/* 5:		jungseong yu			*/
    0x1163,	/* 6:		jungseong ya			*/
    0x1168,	/* 7:		jungseong ye			*/
    0x1174,	/* 8:		jungseong yi			*/
    0x116e,	/* 9:		jungseong u			*/
    0x003a,	/* colon:		colon				*/
    0x1107,	/* semicolon:	choseong  pieup			*/
    0x0032,	/* less:		2				*/
    0x003d,	/* equal:		euals sign			*/
    0x0033,	/* greater:		3				*/
    0x003f,	/* question:	question mark	 		*/
    0x0040,	/* at:		commertial at			*/
    0x11ae,	/* A:		jongseong tikeut		*/
    0x0021,	/* B:		exclamation mark		*/
    0x11b1,	/* C:		jongseong rieul-mieum		*/
    0x11b0,	/* D:		jongseong rieul-kiyeok		*/
    0x11bf,	/* E:		jongseong khieukh		*/
    0x11a9,	/* F:		jongseong ssangkiyeok		*/
    0x002f,	/* G:		slash				*/
    0x0027,	/* H:		apostrophe			*/
    0x0038,	/* I:		8				*/
    0x0034,	/* J:		4				*/
    0x0035,	/* K:		5				*/
    0x0036,	/* L:		6				*/
    0x0031,	/* M:		1				*/
    0x0030,	/* N:		0				*/
    0x0039,	/* O:		9				*/
    0x003e,	/* P:		greater-than sign		*/
    0x11c1,	/* Q:		jongseong phieuph		*/
    0x1164,	/* R:		jungseong yae			*/
    0x11ad,	/* S:		jongseong nieun-hieuh		*/
    0x003b,	/* T:		semicolon			*/
    0x0037,	/* U:		7				*/
    0x11b6,	/* V:		jongseong rieul-hieuh		*/
    0x11c0,	/* W:		jongseong thikeuth		*/
    0x11b9,	/* X:		jongseong pieup-sios		*/
    0x003c,	/* Y:		less-than sign			*/
    0x11be,	/* Z:		jongseong chieuch		*/
    0x005b,	/* bracketleft:	left bracket			*/
    0x005c,	/* backslash:	backslash			*/
    0x005d,	/* bracketright:	right bracket			*/
    0x005e,	/* asciicircum:	circumflex accent		*/
    0x005f,	/* underscore:	underscore			*/
    0x0060,	/* quoteleft:	grave accent			*/
    0x11bc,	/* a:		jongseong ieung			*/
    0x116e,	/* b:		jungseong u			*/
    0x1166,	/* c:		jungseong e			*/
    0x1175,	/* d:		jungseong i			*/
    0x1167,	/* e:		jungseong yeo			*/
    0x1161,	/* f:		jungseong a			*/
    0x1173,	/* g:		jungseong eu			*/
    0x1102,	/* h:		choseong  nieun			*/
    0x1106,	/* i:		choseong  mieum			*/
    0x110b,	/* j:		choseong  ieung			*/
    0x1100,	/* k:		choseong  kiyeok		*/
    0x110c,	/* l:		choseong  cieuc			*/
    0x1112,	/* m:		choseong  hieuh			*/
    0x1109,	/* n:		choseong  sios			*/
    0x110e,	/* o:		choseong  chieuch		*/
    0x1111,	/* p:		choseong  phieuph		*/
    0x11ba,	/* q:		jongseong sios			*/
    0x1162,	/* r:		jungseong ae			*/
    0x11ab,	/* s:		jongseong nieun			*/
    0x1165, 	/* t:		jungseong eo			*/
    0x1103,	/* u:		choseong  tikeut		*/
    0x1169,	/* v:		jungseong o			*/
    0x11af,	/* w:		jongseong rieul			*/
    0x11a8,	/* x:		jongseong kiyeok		*/
    0x1105,	/* y:		choseong  rieul			*/
    0x11b7,	/* z:		jongseong mieum			*/
    0x007b,	/* braceleft:	left brace			*/
    0x007c,	/* bar:		vertical line(bar)		*/
    0x007d,	/* braceright:	right brace			*/
    0x007e,	/* asciitilde:	tilde				*/
};

static const wchar_t hangul_keyboard_table_3final[] = {
    0x11a9,	/* exclam:		jongseong ssangkiyeok		*/
    0x00b7,	/* quotedbl:	middle dot 			*/
    0x11bd,	/* numbersign:	jognseong cieuc 		*/
    0x11b5,	/* dollar:		jongseong rieul-phieuph		*/
    0x11b4,	/* percent:		jongseong rieul-thieuth		*/
    0x201c,	/* ampersand:	left double quotation mark 	*/
    0x1110,	/* apostrophe:	choseong  thieuth		*/
    0x0027,	/* parenleft:	apostrophe			*/
    0x007e,	/* parenright:	Tilde				*/
    0x201d,	/* asterisk:	right double quotation mark	*/
    0x002b,	/* plus:		plus sign			*/
    0x002c,	/* comma:		comma				*/
    0x0029,	/* minus:		right parenthesis		*/
    0x002e,	/* period:		period				*/
    0x1169,	/* slash:		jungseong o			*/
    0x110f,	/* 0:		choseong  khieukh		*/
    0x11c2,	/* 1:		jongseong hieuh			*/
    0x11bb,	/* 2:		jongseong ssangsios		*/
    0x11b8,	/* 3:		jongseong pieup			*/
    0x116d,	/* 4:		jungseong yo			*/
    0x1172,	/* 5:		jungseong yu			*/
    0x1163,	/* 6:		jungseong ya			*/
    0x1168,	/* 7:		jungseong ye			*/
    0x1174,	/* 8:		jungseong yi			*/
    0x116e,	/* 9:		jungseong u			*/
    0x0034,	/* colon:		4				*/
    0x1107,	/* semicolon:	choseong  pieup			*/
    0x002c,	/* less:		comma				*/
    0x003e,	/* equal:		greater-than sign		*/
    0x002e,	/* greater:		period				*/
    0x0021,	/* question:	exclamation mark 		*/
    0x11b0,	/* at:		jongseong rieul-kiyeok		*/
    0x11ae,	/* A:		jongseong tikeut		*/
    0x003f,	/* B:		question mark			*/
    0x11bf,	/* C:		jongseong khieukh		*/
    0x11b2,	/* D:		jongseong rieul-pieup		*/
    0x11ac,	/* E:		jongseong nieun-cieuc		*/
    0x11b1,	/* F:		jongseong rieul-mieum		*/
    0x1164,	/* G:		jungseong yae			*/
    0x0030,	/* H:		0				*/
    0x0037,	/* I:		7				*/
    0x0031,	/* J:		1				*/
    0x0032,	/* K:		2				*/
    0x0033,	/* L:		3				*/
    0x0022,	/* M:		double quotation mark		*/
    0x002d,	/* N:		minus sign			*/
    0x0038,	/* O:		8				*/
    0x0039,	/* P:		9				*/
    0x11c1,	/* Q:		jongseong phieuph		*/
    0x11b6,	/* R:		jongseong rieul-hieuh		*/
    0x11ad,	/* S:		jongseong nieun-hieuh		*/
    0x11b3,	/* T:		jongseong rieul-sios		*/
    0x0036,	/* U:		6				*/
    0x11aa,	/* V:		jongseong kiyeok-sios		*/
    0x11c0,	/* W:		jongseong thikeuth		*/
    0x11b9,	/* X:		jongseong pieup-sios		*/
    0x0035,	/* Y:		5				*/
    0x11be,	/* Z:		jongseong chieuch		*/
    0x0028,	/* bracketleft:	left parenthesis		*/
    0x003a,	/* backslash:	colon				*/
    0x003c,	/* bracketright:	less-than sign			*/
    0x003d,	/* asciicircum:	equals sign			*/
    0x003b,	/* underscore:	semicolon			*/
    0x002a,	/* quoteleft:	asterisk			*/
    0x11bc,	/* a:		jongseong ieung			*/
    0x116e,	/* b:		jungseong u			*/
    0x1166,	/* c:		jungseong e			*/
    0x1175,	/* d:		jungseong i			*/
    0x1167,	/* e:		jungseong yeo			*/
    0x1161,	/* f:		jungseong a			*/
    0x1173,	/* g:		jungseong eu			*/
    0x1102,	/* h:		choseong  nieun			*/
    0x1106,	/* i:		choseong  mieum			*/
    0x110b,	/* j:		choseong  ieung			*/
    0x1100,	/* k:		choseong  kiyeok		*/
    0x110c,	/* l:		choseong  cieuc			*/
    0x1112,	/* m:		choseong  hieuh			*/
    0x1109,	/* n:		choseong  sios			*/
    0x110e,	/* o:		choseong  chieuch		*/
    0x1111,	/* p:		choseong  phieuph		*/
    0x11ba,	/* q:		jongseong sios			*/
    0x1162,	/* r:		jungseong ae			*/
    0x11ab,	/* s:		jongseong nieun			*/
    0x1165, 	/* t:		jungseong eo			*/
    0x1103,	/* u:		choseong  tikeut		*/
    0x1169,	/* v:		jungseong o			*/
    0x11af,	/* w:		jongseong rieul			*/
    0x11a8,	/* x:		jongseong kiyeok		*/
    0x1105,	/* y:		choseong  rieul			*/
    0x11b7,	/* z:		jongseong mieum			*/
    0x0025,	/* braceleft:	percent sign			*/
    0x005c,	/* bar:		backslash			*/
    0x002f,	/* braceright:	slash				*/
    0x203b,	/* asciitilde:	reference mark			*/
};

static const wchar_t hangul_keyboard_table_3sun[] = {
    0x0021,	/* exclam:		exclamation mark		*/
    0x0022,	/* quotedbl:	quotatioin mark			*/
    0x0023,	/* numbersign:	number sign	 		*/
    0x0024,	/* dollar:		dollar sign			*/
    0x0025,	/* percent:		percent sign			*/
    0x0026,	/* ampersand:	ampersand		 	*/
    0x1110,	/* apostrophe:	choseong thieuth		*/
    0x0028,	/* parenleft:	left parenthesis		*/
    0x0029,	/* parenright:	right parenthesis		*/
    0x002a,	/* asterisk:	asterisk			*/
    0x002b,	/* plus:		plus sign			*/
    0x002c,	/* comma:		comma				*/
    0x11bd,	/* minus:		jongseong cieuc			*/
    0x002e,	/* period:		period				*/
    0x11ae,	/* slash:		jongseong tikeut		*/
    0x1164,	/* 0:		choseong  yae			*/
    0x11c2,	/* 1:		jongseong hieuh			*/
    0x11bb,	/* 2:		jongseong ssangsios		*/
    0x11b8,	/* 3:		jongseong pieup			*/
    0x116d,	/* 4:		jungseong yo			*/
    0x1172,	/* 5:		jungseong yu			*/
    0x1163,	/* 6:		jungseong ya			*/
    0x1168,	/* 7:		jungseong ye			*/
    0x1174,	/* 8:		jungseong yi			*/
    0x110f,	/* 9:		choseong khieukh		*/
    0x003a,	/* colon:		colon				*/
    0x1107,	/* semicolon:	choseong  pieup			*/
    0x0032,	/* less:		2				*/
    0x11be,	/* equal:		jongseong chieuch		*/
    0x0033,	/* greater:		3				*/
    0x003f,	/* question:	question mark	 		*/
    0x0040,	/* at:		commertial at			*/
    0x11bc,	/* A:		jongseong ieung			*/
    0x0021,	/* B:		exclamation mark		*/
    0x005c,	/* C:		backslash			*/
    0x005d,	/* D:		right bracket			*/
    0x1167,	/* E:		jungseong yeo			*/
    0x1161,	/* F:		jungseong a			*/
    0x002f,	/* G:		slash				*/
    0x0027,	/* H:		apostrophe			*/
    0x0038,	/* I:		8				*/
    0x0034,	/* J:		4				*/
    0x0035,	/* K:		5				*/
    0x0036,	/* L:		6				*/
    0x0031,	/* M:		1				*/
    0x0030,	/* N:		0				*/
    0x0039,	/* O:		9				*/
    0x003e,	/* P:		greater-than sign		*/
    0x11ba,	/* Q:		jongseong sios			*/
    0x1162,	/* R:		jungseong ae			*/
    0x005b,	/* S:		left bracket			*/
    0x003b,	/* T:		semicolon			*/
    0x0037,	/* U:		7				*/
    0x1169,	/* V:		jungseong o			*/
    0x11af,	/* W:		jongseong rieul			*/
    0x003d,	/* X:		equals sign			*/
    0x003c,	/* Y:		less-than sign			*/
    0x002d,	/* Z:		minus sign			*/
    0x11c0,	/* bracketleft:	jongseong thieuth		*/
    0x11bf,	/* backslash:	jongseong khieukh		*/
    0x11c1,	/* bracketright:	jongseong phieuph		*/
    0x005e,	/* asciicircum:	circumflex accent		*/
    0x005f,	/* underscore:	underscore			*/
    0x0060,	/* quoteleft:	grave accent			*/
    0x11bc,	/* a:		jongseong ieung			*/
    0x116e,	/* b:		jungseong u			*/
    0x1166,	/* c:		jungseong e			*/
    0x1175,	/* d:		jungseong i			*/
    0x1167,	/* e:		jungseong yeo			*/
    0x1161,	/* f:		jungseong a			*/
    0x1173,	/* g:		jungseong eu			*/
    0x1102,	/* h:		choseong  nieun			*/
    0x1106,	/* i:		choseong  mieum			*/
    0x110b,	/* j:		choseong  ieung			*/
    0x1100,	/* k:		choseong  kiyeok		*/
    0x110c,	/* l:		choseong  cieuc			*/
    0x1112,	/* m:		choseong  hieuh			*/
    0x1109,	/* n:		choseong  sios			*/
    0x110e,	/* o:		choseong  chieuch		*/
    0x1111,	/* p:		choseong  phieuph		*/
    0x11ba,	/* q:		jongseong sios			*/
    0x1162,	/* r:		jungseong ae			*/
    0x11ab,	/* s:		jongseong nieun			*/
    0x1165, 	/* t:		jungseong eo			*/
    0x1103,	/* u:		choseong  tikeut		*/
    0x1169,	/* v:		jungseong o			*/
    0x11af,	/* w:		jongseong rieul			*/
    0x11a8,	/* x:		jongseong kiyeok		*/
    0x1105,	/* y:		choseong  rieul			*/
    0x11b7,	/* z:		jongseong mieum			*/
    0x007b,	/* braceleft:	left brace			*/
    0x007c,	/* bar:		vertical line(bar)		*/
    0x007d,	/* braceright:	right brace			*/
    0x007e,	/* asciitilde:	tilde				*/
};

static const wchar_t hangul_keyboard_table_3yet[] = {
    0x11bd,	/* exclam:		jongseong cieuc			*/
    0x0022,	/* quotedbl:	quotatioin mark			*/
    0x0023,	/* numbersign:	number sign	 		*/
    0x0024,	/* dollar:		dollar sign			*/
    0x0025,	/* percent:		percent sign			*/
    0x0026,	/* ampersand:	ampersand		 	*/
    0x1110,	/* apostrophe:	choseong thieuth		*/
    0x0028,	/* parenleft:	left parenthesis		*/
    0x0029,	/* parenright:	right parenthesis		*/
    0x002a,	/* asterisk:	asterisk			*/
    0x002b,	/* plus:		plus sign			*/
    0x002c,	/* comma:		comma				*/
    0x002d,	/* minus:		minus sign			*/
    0x002e,	/* period:		period				*/
    0x1169,	/* slash:		jungseong o			*/
    0x110f,	/* 0:		choseong  khieukh		*/
    0x11c2,	/* 1:		jongseong hieuh			*/
    0x11bb,	/* 2:		jongseong ssangsios		*/
    0x11b8,	/* 3:		jongseong pieup			*/
    0x116d,	/* 4:		jungseong yo			*/
    0x1172,	/* 5:		jungseong yu			*/
    0x1163,	/* 6:		jungseong ya			*/
    0x1168,	/* 7:		jungseong ye			*/
    0x1174,	/* 8:		jungseong yi			*/
    0x116e,	/* 9:		jungseong u			*/
    0x003a,	/* colon:		colon				*/
    0x1107,	/* semicolon:	choseong  pieup			*/
    0x113c,	/* less:		choseong chitueumsios		*/
    0x003d,	/* equal:		euals sign			*/
    0x113e,	/* greater:		choseong ceongchieumsios	*/
    0x003f,	/* question:	question mark	 		*/
    0x11eb,	/* at:		jongseong pansios		*/
    0x11ae,	/* A:		jongseong tikeut		*/
    0x116e,	/* B:		jungseong u			*/
    0x11b1,	/* C:		jongseong rieul-mieum		*/
    0x11b0,	/* D:		jongseong rieul-kiyeok		*/
    0x11bf,	/* E:		jongseong khieukh		*/
    0x11a9,	/* F:		jongseong ssangkiyeok		*/
    0x119e,	/* G:		jungseong araea			*/
    0x1102,	/* H:		choseong nieun			*/
    0x1154,	/* I:		choseong chitueumchieuch	*/
    0x114c,	/* J:		choseong yesieung		*/
    0x114e,	/* K:		choseong chitueumcieuc		*/
    0x1150,	/* L:		choseong ceongchieumcieuc	*/
    0x1159,	/* M:		choseong yeorinhieuh		*/
    0x1140,	/* N:		choseong pansios		*/
    0x1155,	/* O:		choseong ceongchieumchieuch	*/
    0x1111,	/* P:		choseong phieuph		*/
    0x11c1,	/* Q:		jongseong phieuph		*/
    0x1164,	/* R:		jungseong yae			*/
    0x11ad,	/* S:		jongseong nieun-hieuh		*/
    0x1165,	/* T:		jungseong eo			*/
    0x302e,	/* U:		single dot tone mark		*/
    0x11b6,	/* V:		jongseong rieul-hieuh		*/
    0x11c0,	/* W:		jongseong thikeuth		*/
    0x11b9,	/* X:		jongseong pieup-sios		*/
    0x302f,	/* Y:		double dot tone mark		*/
    0x11be,	/* Z:		jongseong chieuch		*/
    0x005b,	/* bracketleft:	left bracket			*/
    0x005c,	/* backslash:	backslash			*/
    0x005d,	/* bracketright:	right bracket			*/
    0x005e,	/* asciicircum:	circumflex accent		*/
    0x005f,	/* underscore:	underscore			*/
    0x11f9,	/* quoteleft:	jongseong yeorinhieuh		*/
    0x11bc,	/* a:		jongseong ieung			*/
    0x116e,	/* b:		jungseong u			*/
    0x1166,	/* c:		jungseong e			*/
    0x1175,	/* d:		jungseong i			*/
    0x1167,	/* e:		jungseong yeo			*/
    0x1161,	/* f:		jungseong a			*/
    0x1173,	/* g:		jungseong eu			*/
    0x1102,	/* h:		choseong  nieun			*/
    0x1106,	/* i:		choseong  mieum			*/
    0x110b,	/* j:		choseong  ieung			*/
    0x1100,	/* k:		choseong  kiyeok		*/
    0x110c,	/* l:		choseong  cieuc			*/
    0x1112,	/* m:		choseong  hieuh			*/
    0x1109,	/* n:		choseong  sios			*/
    0x110e,	/* o:		choseong  chieuch		*/
    0x1111,	/* p:		choseong  phieuph		*/
    0x11ba,	/* q:		jongseong sios			*/
    0x1162,	/* r:		jungseong ae			*/
    0x11ab,	/* s:		jongseong nieun			*/
    0x1165, 	/* t:		jungseong eo			*/
    0x1103,	/* u:		choseong  tikeut		*/
    0x1169,	/* v:		jungseong o			*/
    0x11af,	/* w:		jongseong rieul			*/
    0x11a8,	/* x:		jongseong kiyeok		*/
    0x1105,	/* y:		choseong  rieul			*/
    0x11b7,	/* z:		jongseong mieum			*/
    0x007b,	/* braceleft:	left brace			*/
    0x007c,	/* bar:		vertical line(bar)		*/
    0x007d,	/* braceright:	right brace			*/
    0x11f0,	/* asciitilde:	jongseong yesieung		*/
};

static const HangulJamoCombination hangul_combination_table_default[] = {
  { 0x11001100, 0x1101 }, /* choseong  kiyeok + kiyeok	= ssangkiyeok	*/
  { 0x11031103, 0x1104 }, /* choseong  tikeut + tikeut	= ssangtikeut	*/
  { 0x11071107, 0x1108 }, /* choseong  pieup  + pieup	= ssangpieup	*/
  { 0x11091109, 0x110a }, /* choseong  sios   + sios	= ssangsios	*/
  { 0x110c110c, 0x110d }, /* choseong  cieuc  + cieuc	= ssangcieuc	*/
  { 0x11691161, 0x116a }, /* jungseong o      + a	= wa		*/
  { 0x11691162, 0x116b }, /* jungseong o      + ae	= wae		*/
  { 0x11691175, 0x116c }, /* jungseong o      + i	= oe		*/
  { 0x116e1165, 0x116f }, /* jungseong u      + eo	= weo		*/
  { 0x116e1166, 0x1170 }, /* jungseong u      + e	= we		*/
  { 0x116e1175, 0x1171 }, /* jungseong u      + i	= wi		*/
  { 0x11731175, 0x1174 }, /* jungseong eu     + i	= yi		*/
  { 0x11a811a8, 0x11a9 }, /* jongseong kiyeok + kiyeok	= ssangekiyeok	*/
  { 0x11a811ba, 0x11aa }, /* jongseong kiyeok + sios	= kiyeok-sois	*/
  { 0x11ab11bd, 0x11ac }, /* jongseong nieun  + cieuc	= nieun-cieuc	*/
  { 0x11ab11c2, 0x11ad }, /* jongseong nieun  + hieuh	= nieun-hieuh	*/
  { 0x11af11a8, 0x11b0 }, /* jongseong rieul  + kiyeok	= rieul-kiyeok	*/
  { 0x11af11b7, 0x11b1 }, /* jongseong rieul  + mieum	= rieul-mieum	*/
  { 0x11af11b8, 0x11b2 }, /* jongseong rieul  + pieup	= rieul-pieup	*/
  { 0x11af11ba, 0x11b3 }, /* jongseong rieul  + sios	= rieul-sios	*/
  { 0x11af11c0, 0x11b4 }, /* jongseong rieul  + thieuth = rieul-thieuth	*/
  { 0x11af11c1, 0x11b5 }, /* jongseong rieul  + phieuph = rieul-phieuph	*/
  { 0x11af11c2, 0x11b6 }, /* jongseong rieul  + hieuh	= rieul-hieuh	*/
  { 0x11b811ba, 0x11b9 }, /* jongseong pieup  + sios	= pieup-sios	*/
  { 0x11ba11ba, 0x11bb }, /* jongseong sios   + sios	= ssangsios	*/
};

static const HangulJamoCombination hangul_combination_table_full[] = {
  { 0x11001100, 0x1101 }, /* CHOSEONG SSANGKIYEOK           = KIYEOK + KIYEOK */
  { 0x11021100, 0x1113 }, /* CHOSEONG NIEUN-KIYEOK          = NIEUN + KIYEOK */
  { 0x11021102, 0x1114 }, /* CHOSEONG SSANGNIEUN            = NIEUN + NIEUN */
  { 0x11021103, 0x1115 }, /* CHOSEONG NIEUN-TIKEUT          = NIEUN + TIKEUT */
  { 0x11021107, 0x1116 }, /* CHOSEONG NIEUN-PIEUP           = NIEUN + PIEUP */
  { 0x11031100, 0x1117 }, /* CHOSEONG TIKEUT-KIYEOK         = TIKEUT + KIYEOK */
  { 0x11031103, 0x1104 }, /* CHOSEONG SSANGTIKEUT           = TIKEUT + TIKEUT */
  { 0x11051102, 0x1118 }, /* CHOSEONG RIEUL-NIEUN           = RIEUL + NIEUN */
  { 0x11051105, 0x1119 }, /* CHOSEONG SSANGRIEUL            = RIEUL + RIEUL */
  { 0x1105110b, 0x111b }, /* CHOSEONG KAPYEOUNRIEUL         = RIEUL + IEUNG */
  { 0x11051112, 0x111a }, /* CHOSEONG RIEUL-HIEUH           = RIEUL + HIEUH */
  { 0x11061107, 0x111c }, /* CHOSEONG MIEUM-PIEUP           = MIEUM + PIEUP */
  { 0x1106110b, 0x111d }, /* CHOSEONG KAPYEOUNMIEUM         = MIEUM + IEUNG */
  { 0x11071100, 0x111e }, /* CHOSEONG PIEUP-KIYEOK          = PIEUP + KIYEOK */
  { 0x11071102, 0x111f }, /* CHOSEONG PIEUP-NIEUN           = PIEUP + NIEUN */
  { 0x11071103, 0x1120 }, /* CHOSEONG PIEUP-TIKEUT          = PIEUP + TIKEUT */
  { 0x11071107, 0x1108 }, /* CHOSEONG SSANGPIEUP            = PIEUP + PIEUP */
  { 0x11071109, 0x1121 }, /* CHOSEONG PIEUP-SIOS            = PIEUP + SIOS */
  { 0x1107110b, 0x112b }, /* CHOSEONG KAPYEOUNPIEUP         = PIEUP + IEUNG */
  { 0x1107110c, 0x1127 }, /* CHOSEONG PIEUP-CIEUC           = PIEUP + CIEUC */
  { 0x1107110e, 0x1128 }, /* CHOSEONG PIEUP-CHIEUCH         = PIEUP + CHIEUCH */
  { 0x11071110, 0x1129 }, /* CHOSEONG PIEUP-THIEUTH         = PIEUP + THIEUTH */
  { 0x11071111, 0x112a }, /* CHOSEONG PIEUP-PHIEUPH         = PIEUP + PHIEUPH */
  { 0x1108110b, 0x112c }, /* CHOSEONG KAPYEOUNSSANGPIEUP    = SSANGPIEUP + IEUNG */
  { 0x11091100, 0x112d }, /* CHOSEONG SIOS-KIYEOK           = SIOS + KIYEOK */
  { 0x11091102, 0x112e }, /* CHOSEONG SIOS-NIEUN            = SIOS + NIEUN */
  { 0x11091103, 0x112f }, /* CHOSEONG SIOS-TIKEUT           = SIOS + TIKEUT */
  { 0x11091105, 0x1130 }, /* CHOSEONG SIOS-RIEUL            = SIOS + RIEUL */
  { 0x11091106, 0x1131 }, /* CHOSEONG SIOS-MIEUM            = SIOS + MIEUM */
  { 0x11091107, 0x1132 }, /* CHOSEONG SIOS-PIEUP            = SIOS + PIEUP */
  { 0x11091109, 0x110a }, /* CHOSEONG SSANGSIOS             = SIOS + SIOS */
  { 0x1109110b, 0x1135 }, /* CHOSEONG SIOS-IEUNG            = SIOS + IEUNG */
  { 0x1109110c, 0x1136 }, /* CHOSEONG SIOS-CIEUC            = SIOS + CIEUC */
  { 0x1109110e, 0x1137 }, /* CHOSEONG SIOS-CHIEUCH          = SIOS + CHIEUCH */
  { 0x1109110f, 0x1138 }, /* CHOSEONG SIOS-KHIEUKH          = SIOS + KHIEUKH */
  { 0x11091110, 0x1139 }, /* CHOSEONG SIOS-THIEUTH          = SIOS + THIEUTH */
  { 0x11091111, 0x113a }, /* CHOSEONG SIOS-PHIEUPH          = SIOS + PHIEUPH */
  { 0x11091112, 0x113b }, /* CHOSEONG SIOS-HIEUH            = SIOS + HIEUH */
  { 0x110a1109, 0x1134 }, /* CHOSEONG SIOS-SSANGSIOS        = SSANGSIOS + SIOS */
  { 0x110b1100, 0x1141 }, /* CHOSEONG IEUNG-KIYEOK          = IEUNG + KIYEOK */
  { 0x110b1103, 0x1142 }, /* CHOSEONG IEUNG-TIKEUT          = IEUNG + TIKEUT */
  { 0x110b1106, 0x1143 }, /* CHOSEONG IEUNG-MIEUM           = IEUNG + MIEUM */
  { 0x110b1107, 0x1144 }, /* CHOSEONG IEUNG-PIEUP           = IEUNG + PIEUP */
  { 0x110b1109, 0x1145 }, /* CHOSEONG IEUNG-SIOS            = IEUNG + SIOS */
  { 0x110b110b, 0x1147 }, /* CHOSEONG SSANGIEUNG            = IEUNG + IEUNG */
  { 0x110b110c, 0x1148 }, /* CHOSEONG IEUNG-CIEUC           = IEUNG + CIEUC */
  { 0x110b110e, 0x1149 }, /* CHOSEONG IEUNG-CHIEUCH         = IEUNG + CHIEUCH */
  { 0x110b1110, 0x114a }, /* CHOSEONG IEUNG-THIEUTH         = IEUNG + THIEUTH */
  { 0x110b1111, 0x114b }, /* CHOSEONG IEUNG-PHIEUPH         = IEUNG + PHIEUPH */
  { 0x110b1140, 0x1146 }, /* CHOSEONG IEUNG-PANSIOS         = IEUNG + PANSIOS */
  { 0x110c110b, 0x114d }, /* CHOSEONG CIEUC-IEUNG           = CIEUC + IEUNG */
  { 0x110c110c, 0x110d }, /* CHOSEONG SSANGCIEUC            = CIEUC + CIEUC */
  { 0x110e110f, 0x1152 }, /* CHOSEONG CHIEUCH-KHIEUKH       = CHIEUCH + KHIEUKH */
  { 0x110e1112, 0x1153 }, /* CHOSEONG CHIEUCH-HIEUH         = CHIEUCH + HIEUH */
  { 0x11111107, 0x1156 }, /* CHOSEONG PHIEUPH-PIEUP         = PHIEUPH + PIEUP */
  { 0x1111110b, 0x1157 }, /* CHOSEONG KAPYEOUNPHIEUPH       = PHIEUPH + IEUNG */
  { 0x11121112, 0x1158 }, /* CHOSEONG SSANGHIEUH            = HIEUH + HIEUH */
  { 0x11211100, 0x1122 }, /* CHOSEONG PIEUP-SIOS-KIYEOK     = PIEUP-SIOS + KIYEOK */
  { 0x11211103, 0x1123 }, /* CHOSEONG PIEUP-SIOS-TIKEUT     = PIEUP-SIOS + TIKEUT */
  { 0x11211107, 0x1124 }, /* CHOSEONG PIEUP-SIOS-PIEUP      = PIEUP-SIOS + PIEUP */
  { 0x11211109, 0x1125 }, /* CHOSEONG PIEUP-SSANGSIOS       = PIEUP-SIOS + SIOS */
  { 0x1121110c, 0x1126 }, /* CHOSEONG PIEUP-SIOS-CIEUC      = PIEUP-SIOS + CIEUC */
  { 0x11321100, 0x1133 }, /* CHOSEONG SIOS-PIEUP-KIYEOK     = SIOS-PIEUP + KIYEOK */
  { 0x113c113c, 0x113d }, /* CHOSEONG CHITUEUMSSANGSIOS     = CHITUEUMSIOS + CHITUEUMSIOS */
  { 0x113e113e, 0x113f }, /* CHOSEONG CEONGCHIEUMSSANGSIOS  = CEONGCHIEUMSIOS + CEONGCHIEUMSIOS */
  { 0x114e114e, 0x114f }, /* CHOSEONG CHITUEUMSSANGCIEUC    = CHITUEUMCIEUC + CHITUEUMCIEUC */
  { 0x11501150, 0x1151 }, /* CHOSEONG CEONGCHIEUMSSANGCIEUC = CEONGCHIEUMCIEUC + CEONGCHIEUMCIEUC */
  { 0x11611169, 0x1176 }, /* JUNGSEONG A-O                  = A + O */
  { 0x1161116e, 0x1177 }, /* JUNGSEONG A-U                  = A + U */
  { 0x11611175, 0x1162 }, /* JUNGSEONG AE                   = A */
  { 0x11631169, 0x1178 }, /* JUNGSEONG YA-O                 = YA + O */
  { 0x1163116d, 0x1179 }, /* JUNGSEONG YA-YO                = YA + YO */
  { 0x11631175, 0x1164 }, /* JUNGSEONG YAE                  = YA */
  { 0x11651169, 0x117a }, /* JUNGSEONG EO-O                 = EO + O */
  { 0x1165116e, 0x117b }, /* JUNGSEONG EO-U                 = EO + U */
  { 0x11651173, 0x117c }, /* JUNGSEONG EO-EU                = EO + EU */
  { 0x11651175, 0x1166 }, /* JUNGSEONG E                    = EO */
  { 0x11671169, 0x117d }, /* JUNGSEONG YEO-O                = YEO + O */
  { 0x1167116e, 0x117e }, /* JUNGSEONG YEO-U                = YEO + U */
  { 0x11671175, 0x1168 }, /* JUNGSEONG YE                   = YEO */
  { 0x11691161, 0x116a }, /* JUNGSEONG WA                   = O + A */
  { 0x11691165, 0x117f }, /* JUNGSEONG O-EO                 = O + EO */
  { 0x11691166, 0x1180 }, /* JUNGSEONG O-E                  = O + E */
  { 0x11691168, 0x1181 }, /* JUNGSEONG O-YE                 = O + YE */
  { 0x11691169, 0x1182 }, /* JUNGSEONG O-O                  = O + O */
  { 0x1169116e, 0x1183 }, /* JUNGSEONG O-U                  = O + U */
  { 0x11691175, 0x116c }, /* JUNGSEONG OE                   = O */
  { 0x116a1175, 0x116b }, /* JUNGSEONG WAE                  = WA */
  { 0x116d1163, 0x1184 }, /* JUNGSEONG YO-YA                = YO + YA */
  { 0x116d1164, 0x1185 }, /* JUNGSEONG YO-YAE               = YO + YAE */
  { 0x116d1167, 0x1186 }, /* JUNGSEONG YO-YEO               = YO + YEO */
  { 0x116d1169, 0x1187 }, /* JUNGSEONG YO-O                 = YO + O */
  { 0x116d1175, 0x1188 }, /* JUNGSEONG YO-I                 = YO + I */
  { 0x116e1161, 0x1189 }, /* JUNGSEONG U-A                  = U + A */
  { 0x116e1162, 0x118a }, /* JUNGSEONG U-AE                 = U + AE */
  { 0x116e1165, 0x116f }, /* JUNGSEONG WEO                  = U + EO */
  { 0x116e1168, 0x118c }, /* JUNGSEONG U-YE                 = U + YE */
  { 0x116e116e, 0x118d }, /* JUNGSEONG U-U                  = U + U */
  { 0x116e1175, 0x1171 }, /* JUNGSEONG WI                   = U */
  { 0x116f1173, 0x118b }, /* JUNGSEONG U-EO-EU              = WEO + EU */
  { 0x116f1175, 0x1170 }, /* JUNGSEONG WE                   = WEO */
  { 0x11721161, 0x118e }, /* JUNGSEONG YU-A                 = YU + A */
  { 0x11721165, 0x118f }, /* JUNGSEONG YU-EO                = YU + EO */
  { 0x11721166, 0x1190 }, /* JUNGSEONG YU-E                 = YU + E */
  { 0x11721167, 0x1191 }, /* JUNGSEONG YU-YEO               = YU + YEO */
  { 0x11721168, 0x1192 }, /* JUNGSEONG YU-YE                = YU + YE */
  { 0x1172116e, 0x1193 }, /* JUNGSEONG YU-U                 = YU + U */
  { 0x11721175, 0x1194 }, /* JUNGSEONG YU-I                 = YU + I */
  { 0x1173116e, 0x1195 }, /* JUNGSEONG EU-U                 = EU + U */
  { 0x11731173, 0x1196 }, /* JUNGSEONG EU-EU                = EU + EU */
  { 0x11731175, 0x1174 }, /* JUNGSEONG YI                   = EU */
  { 0x1174116e, 0x1197 }, /* JUNGSEONG YI-U                 = YI + U */
  { 0x11751161, 0x1198 }, /* JUNGSEONG I-A                  = I + A */
  { 0x11751163, 0x1199 }, /* JUNGSEONG I-YA                 = I + YA */
  { 0x11751169, 0x119a }, /* JUNGSEONG I-O                  = I + O */
  { 0x1175116e, 0x119b }, /* JUNGSEONG I-U                  = I + U */
  { 0x11751173, 0x119c }, /* JUNGSEONG I-EU                 = I + EU */
  { 0x1175119e, 0x119d }, /* JUNGSEONG I-ARAEA              = I */
  { 0x119e1165, 0x119f }, /* JUNGSEONG ARAEA-EO             = ARAEA + EO */
  { 0x119e116e, 0x11a0 }, /* JUNGSEONG ARAEA-U              = ARAEA + U */
  { 0x119e1175, 0x11a1 }, /* JUNGSEONG ARAEA-I              = ARAEA + I */
  { 0x119e119e, 0x11a2 }, /* JUNGSEONG SSANGARAEA           = ARAEA + ARAEA */
  { 0x11a811a8, 0x11a9 }, /* JONGSEONG SSANGKIYEOK          = KIYEOK + KIYEOK */
  { 0x11a811af, 0x11c3 }, /* JONGSEONG KIYEOK-RIEUL         = KIYEOK + RIEUL */
  { 0x11a811ba, 0x11aa }, /* JONGSEONG KIYEOK-SIOS          = KIYEOK */
  { 0x11aa11a8, 0x11c4 }, /* JONGSEONG KIYEOK-SIOS-KIYEOK   = KIYEOK-SIOS + KIYEOK */
  { 0x11ab11a8, 0x11c5 }, /* JONGSEONG NIEUN-KIYEOK         = NIEUN + KIYEOK */
  { 0x11ab11ae, 0x11c6 }, /* JONGSEONG NIEUN-TIKEUT         = NIEUN + TIKEUT */
  { 0x11ab11ba, 0x11c7 }, /* JONGSEONG NIEUN-SIOS           = NIEUN + SIOS */
  { 0x11ab11bd, 0x11ac }, /* JONGSEONG NIEUN-CIEUC          = NIEUN */
  { 0x11ab11c0, 0x11c9 }, /* JONGSEONG NIEUN-THIEUTH        = NIEUN + THIEUTH */
  { 0x11ab11c2, 0x11ad }, /* JONGSEONG NIEUN-HIEUH          = NIEUN */
  { 0x11ab11eb, 0x11c8 }, /* JONGSEONG NIEUN-PANSIOS        = NIEUN */
  { 0x11ae11a8, 0x11ca }, /* JONGSEONG TIKEUT-KIYEOK        = TIKEUT + KIYEOK */
  { 0x11ae11af, 0x11cb }, /* JONGSEONG TIKEUT-RIEUL         = TIKEUT + RIEUL */
  { 0x11af11a8, 0x11b0 }, /* JONGSEONG RIEUL-KIYEOK         = RIEUL + KIYEOK */
  { 0x11af11ab, 0x11cd }, /* JONGSEONG RIEUL-NIEUN          = RIEUL + NIEUN */
  { 0x11af11ae, 0x11ce }, /* JONGSEONG RIEUL-TIKEUT         = RIEUL + TIKEUT */
  { 0x11af11af, 0x11d0 }, /* JONGSEONG SSANGRIEUL           = RIEUL + RIEUL */
  { 0x11af11b7, 0x11b1 }, /* JONGSEONG RIEUL-MIEUM          = RIEUL */
  { 0x11af11b8, 0x11b2 }, /* JONGSEONG RIEUL-PIEUP          = RIEUL */
  { 0x11af11ba, 0x11b3 }, /* JONGSEONG RIEUL-SIOS           = RIEUL */
  { 0x11af11bf, 0x11d8 }, /* JONGSEONG RIEUL-KHIEUKH        = RIEUL + KHIEUKH */
  { 0x11af11c0, 0x11b4 }, /* JONGSEONG RIEUL-THIEUTH        = RIEUL */
  { 0x11af11c1, 0x11b5 }, /* JONGSEONG RIEUL-PHIEUPH        = RIEUL */
  { 0x11af11c2, 0x11b6 }, /* JONGSEONG RIEUL-HIEUH          = RIEUL */
  { 0x11af11eb, 0x11d7 }, /* JONGSEONG RIEUL-PANSIOS        = RIEUL */
  { 0x11af11f9, 0x11d9 }, /* JONGSEONG RIEUL-YEORINHIEUH    = RIEUL */
  { 0x11b011ba, 0x11cc }, /* JONGSEONG RIEUL-KIYEOK-SIOS    = RIEUL-KIYEOK + SIOS */
  { 0x11b111a8, 0x11d1 }, /* JONGSEONG RIEUL-MIEUM-KIYEOK   = RIEUL-MIEUM + KIYEOK */
  { 0x11b111ba, 0x11d2 }, /* JONGSEONG RIEUL-MIEUM-SIOS     = RIEUL-MIEUM + SIOS */
  { 0x11b211ba, 0x11d3 }, /* JONGSEONG RIEUL-PIEUP-SIOS     = RIEUL-PIEUP + SIOS */
  { 0x11b211bc, 0x11d5 }, /* JONGSEONG RIEUL-KAPYEOUNPIEUP  = RIEUL-PIEUP + IEUNG */
  { 0x11b211c2, 0x11d4 }, /* JONGSEONG RIEUL-PIEUP-HIEUH    = RIEUL-PIEUP + HIEUH */
  { 0x11b311ba, 0x11d6 }, /* JONGSEONG RIEUL-SSANGSIOS      = RIEUL-SIOS + SIOS */
  { 0x11b711a8, 0x11da }, /* JONGSEONG MIEUM-KIYEOK         = MIEUM + KIYEOK */
  { 0x11b711af, 0x11db }, /* JONGSEONG MIEUM-RIEUL          = MIEUM + RIEUL */
  { 0x11b711b8, 0x11dc }, /* JONGSEONG MIEUM-PIEUP          = MIEUM + PIEUP */
  { 0x11b711ba, 0x11dd }, /* JONGSEONG MIEUM-SIOS           = MIEUM + SIOS */
  { 0x11b711bc, 0x11e2 }, /* JONGSEONG KAPYEOUNMIEUM        = MIEUM + IEUNG */
  { 0x11b711be, 0x11e0 }, /* JONGSEONG MIEUM-CHIEUCH        = MIEUM + CHIEUCH */
  { 0x11b711c2, 0x11e1 }, /* JONGSEONG MIEUM-HIEUH          = MIEUM + HIEUH */
  { 0x11b711eb, 0x11df }, /* JONGSEONG MIEUM-PANSIOS        = MIEUM */
  { 0x11b811af, 0x11e3 }, /* JONGSEONG PIEUP-RIEUL          = PIEUP + RIEUL */
  { 0x11b811ba, 0x11b9 }, /* JONGSEONG PIEUP-SIOS           = PIEUP */
  { 0x11b811bc, 0x11e6 }, /* JONGSEONG KAPYEOUNPIEUP        = PIEUP + IEUNG */
  { 0x11b811c1, 0x11e4 }, /* JONGSEONG PIEUP-PHIEUPH        = PIEUP + PHIEUPH */
  { 0x11b811c2, 0x11e5 }, /* JONGSEONG PIEUP-HIEUH          = PIEUP + HIEUH */
  { 0x11ba11a8, 0x11e7 }, /* JONGSEONG SIOS-KIYEOK          = SIOS + KIYEOK */
  { 0x11ba11ae, 0x11e8 }, /* JONGSEONG SIOS-TIKEUT          = SIOS + TIKEUT */
  { 0x11ba11af, 0x11e9 }, /* JONGSEONG SIOS-RIEUL           = SIOS + RIEUL */
  { 0x11ba11b8, 0x11ea }, /* JONGSEONG SIOS-PIEUP           = SIOS + PIEUP */
  { 0x11ba11ba, 0x11bb }, /* JONGSEONG SSANGSIOS            = SIOS + SIOS */
  { 0x11bc11a8, 0x11ec }, /* JONGSEONG IEUNG-KIYEOK         = IEUNG + KIYEOK */
  { 0x11bc11bc, 0x11ee }, /* JONGSEONG SSANGIEUNG           = IEUNG + IEUNG */
  { 0x11bc11bf, 0x11ef }, /* JONGSEONG IEUNG-KHIEUKH        = IEUNG + KHIEUKH */
  { 0x11c111b8, 0x11f3 }, /* JONGSEONG PHIEUPH-PIEUP        = PHIEUPH + PIEUP */
  { 0x11c111bc, 0x11f4 }, /* JONGSEONG KAPYEOUNPHIEUPH      = PHIEUPH + IEUNG */
  { 0x11c211ab, 0x11f5 }, /* JONGSEONG HIEUH-NIEUN          = HIEUH + NIEUN */
  { 0x11c211af, 0x11f6 }, /* JONGSEONG HIEUH-RIEUL          = HIEUH + RIEUL */
  { 0x11c211b7, 0x11f7 }, /* JONGSEONG HIEUH-MIEUM          = HIEUH + MIEUM */
  { 0x11c211b8, 0x11f8 }, /* JONGSEONG HIEUH-PIEUP          = HIEUH + PIEUP */
  { 0x11ce11c2, 0x11cf }, /* JONGSEONG RIEUL-TIKEUT-HIEUH   = RIEUL-TIKEUT + HIEUH */
  { 0x11dd11ba, 0x11de }, /* JONGSEONG MIEUM-SSANGSIOS      = MIEUM-SIOS + SIOS */
  { 0x11ec11a8, 0x11ed }, /* JONGSEONG IEUNG-SSANGKIYEOK    = IEUNG-KIYEOK + KIYEOK */
  { 0x11f011ba, 0x11f1 }, /* JONGSEONG YESIEUNG-SIOS        = YESIEUNG + SIOS */
  { 0x11f011eb, 0x11f2 }, /* JONGSEONG YESIEUNG-PANSIOS     = YESIEUNG + PANSIOS */
};
