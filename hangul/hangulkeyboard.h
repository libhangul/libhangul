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
