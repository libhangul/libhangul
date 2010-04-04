/* libhangul
 * Copyright (C) 2004 - 2009 Choe Hwanjin
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "hangul.h"

/**
 * @defgroup hangulctype 한글 글자 조작
 * 
 * @section hangulctype 한글 글자 조작
 * libhangul은 한글 각 글자를 구분하고 조작하는데 사용할 수 있는 몇가지 함수를
 * 제공한다.  libhangul의 글자 구분 함수의 인터페이스에서 글자의 기본 단위는
 * UCS4 코드값이다.
 */

/**
 * @file hangulctype.c
 */

/**
 * @ingroup hangulctype
 * @typedef ucschar
 * @brief UCS4 코드 단위의 글자 코드 값
 *
 * UCS4 코드 값을 저장한다. libhangul에서 사용하는 문자열의 기본단위이다.
 * preedit 문자열과 commit 문자열 모두 ucschar 포인터 형으로 전달된다.
 * 이 스트링은  C 스트링과 유사하게 0으로 끝난다.
 * 유니코드 값이 한글의 어떤 범주에 속하는지 확인하는 함수도 모두 ucschar 형을
 * 사용한다.
 */

static const ucschar syllable_base  = 0xac00;
static const ucschar choseong_base  = 0x1100;
static const ucschar jungseong_base = 0x1161;
static const ucschar jongseong_base = 0x11a7;
static const int njungseong = 21;
static const int njongseong = 28;

/**
 * @ingroup hangulctype
 * @brief 초성인지 확인하는 함수
 * @param c UCS4 코드 값
 * @return @a c 가 초성에 해당하면 true를 리턴함, 아니면 false
 * 
 * @a c 로 주어진 UCS4 코드가 초성인지 확인한다.
 * Unicode 5.2 지원
 */
bool
hangul_is_choseong(ucschar c)
{
    return (c >= 0x1100 && c <= 0x115f) ||
	   (c >= 0xa960 && c <= 0xa97c);
;
}

/**
 * @ingroup hangulctype
 * @brief 중성인지 확인하는 함수
 * @param c UCS4 코드 값
 * @return @a c 가 중성에 해당하면 true를 리턴함, 아니면 false
 * 
 * @a c 로 주어진 UCS4 코드가 중성인지 확인한다.
 * Unicode 5.2 지원
 */
bool
hangul_is_jungseong(ucschar c)
{
    return (c >= 0x1161 && c <= 0x11a2) ||
	   (c >= 0xd7b0 && c <= 0xd7c6);
}

/**
 * @ingroup hangulctype
 * @brief 종성인지 확인하는 함수
 * @param c UCS4 코드 값
 * @return @a c 가 종성에 해당하면 true를 리턴함, 아니면 false
 * 
 * @a c 로 주어진 UCS4 코드가 종성인지 확인한다.
 * Unicode 5.2 지원
 */
bool
hangul_is_jongseong(ucschar c)
{
    return (c >= 0x11a8 && c <= 0x11f9) ||
	   (c >= 0xd7cb && c <= 0xd7fb);
}

bool
hangul_is_combining_mark(ucschar c)
{
    return  c == 0x302e || c == 0x302f  ||
	   (c >= 0x0300 && c <= 0x036F) ||
	   (c >= 0x1dc0 && c <= 0x1dff) ||
	   (c >= 0xfe20 && c <= 0xfe2f);
}

/**
 * @ingroup hangulctype
 * @brief 초성이고 조합 가능한지 확인
 */
bool
hangul_is_choseong_conjoinable(ucschar c)
{
    return c >= 0x1100 && c <= 0x1112;
}

/**
 * @ingroup hangulctype
 * @brief 중성이고 조합 가능한지 확인
 */
bool
hangul_is_jungseong_conjoinable(ucschar c)
{
    return c >= 0x1161 && c <= 0x1175;
}

/**
 * @ingroup hangulctype
 * @brief 종성이고 조합 가능한지 확인
 */
bool
hangul_is_jongseong_conjoinable(ucschar c)
{
    return c >= 0x11a7 && c <= 0x11c2;
}

/**
 * @ingroup hangulctype
 * @brief 한글 음절 인지 확
 * @param c UCS4 코드 값
 * @return @a c가 한글 음절 코드이면 true, 그 외에는 false
 *
 * 이 함수는 @a c로 주어진 UCS4 코드가 현대 한글 음절에 해당하는지
 * 확인한다.
 */
bool
hangul_is_syllable(ucschar c)
{
    return c >= 0xac00 && c <= 0xd7a3;
}

/**
 * @ingroup hangulctype
 * @brief 자모 인지 확인
 * @param c UCS4 코드 값
 * @return @a c 가 자모 코드이면 true를 리턴, 그외에는 false
 *
 * @a c 로 주어진 UCS4 코드가 자모 코드인지 확인한다.
 * Unicode 5.2 지원
 */
bool
hangul_is_jamo(ucschar c)
{
    return hangul_is_choseong(c) ||
	   hangul_is_jungseong(c) ||
	   hangul_is_jongseong(c);
}

/** @deprecated 이 함수 대신 hangul_is_jamo()함수를 사용한다. */
bool
hangul_is_jaso(ucschar c)
{
    return hangul_is_jamo(c);
}

/**
 * @ingroup hangulctype
 * @brief 호환 자모인지 확인
 * @param c UCS4 코드 값
 * @return @a c가 호환자모이면 true, 그 외에는 false
 *
 * 이 함수는 @a c로 주어진 UCS4 코드가 호환 자모인지 확인한다.
 */
bool
hangul_is_cjamo(ucschar c)
{
    return c >= 0x3131 && c <= 0x318e;
}

/**
 * @ingroup hangulctype
 * @brief 자모 코드를 대응하는 호환 자모로 변환
 * @param c 변환할 UCS4 코드 값
 * @return @a c 에 대응되는 호환 자모 값, or c
 *
 * 이 함수는 @a c 로 주어진 자모 코드와 같은 형태를 가진 호환 자모 값을
 * 리턴한다.  자모와 같은 형태를 가진 호환 자모가 없는 경우에는 @a c 의 
 * 값을 그대로 리턴한다.
 */
ucschar
hangul_jamo_to_cjamo(ucschar c)
{
    static unsigned short jamo_table[] = {
	0x3131,     /* 0x1100 */
	0x3132,     /* 0x1101 */
	0x3134,     /* 0x1102 */
	0x3137,     /* 0x1103 */
	0x3138,     /* 0x1104 */
	0x3139,     /* 0x1105 */
	0x3141,     /* 0x1106 */
	0x3142,     /* 0x1107 */
	0x3143,     /* 0x1108 */
	0x3145,     /* 0x1109 */
	0x3146,     /* 0x110a */
	0x3147,     /* 0x110b */
	0x3148,     /* 0x110c */
	0x3149,     /* 0x110d */
	0x314a,     /* 0x110e */
	0x314b,     /* 0x110f */
	0x314c,     /* 0x1110 */
	0x314d,     /* 0x1111 */
	0x314e,     /* 0x1112 */
	0x0000,     /* 0x1113 */
	0x3165,     /* 0x1114 */
	0x3166,     /* 0x1115 */
	0x0000,     /* 0x1116 */
	0x0000,     /* 0x1117 */
	0x0000,     /* 0x1118 */
	0x0000,     /* 0x1119 */
	0x3140,     /* 0x111a */
	0x0000,     /* 0x111b */
	0x316e,     /* 0x111c */
	0x3171,     /* 0x111d */
	0x3172,     /* 0x111e */
	0x0000,     /* 0x111f */
	0x3173,     /* 0x1120 */
	0x3144,     /* 0x1121 */
	0x3174,     /* 0x1122 */
	0x3175,     /* 0x1123 */
	0x0000,     /* 0x1124 */
	0x0000,     /* 0x1125 */
	0x0000,     /* 0x1126 */
	0x3176,     /* 0x1127 */
	0x0000,     /* 0x1128 */
	0x3177,     /* 0x1129 */
	0x0000,     /* 0x112a */
	0x3178,     /* 0x112b */
	0x3179,     /* 0x112c */
	0x317a,     /* 0x112d */
	0x317b,     /* 0x112e */
	0x317c,     /* 0x112f */
	0x0000,     /* 0x1130 */
	0x0000,     /* 0x1131 */
	0x317d,     /* 0x1132 */
	0x0000,     /* 0x1133 */
	0x0000,     /* 0x1134 */
	0x0000,     /* 0x1135 */
	0x317e,     /* 0x1136 */
	0x0000,     /* 0x1137 */
	0x0000,     /* 0x1138 */
	0x0000,     /* 0x1139 */
	0x0000,     /* 0x113a */
	0x0000,     /* 0x113b */
	0x0000,     /* 0x113c */
	0x0000,     /* 0x113d */
	0x0000,     /* 0x113e */
	0x0000,     /* 0x113f */
	0x317f,     /* 0x1140 */
	0x0000,     /* 0x1141 */
	0x0000,     /* 0x1142 */
	0x0000,     /* 0x1143 */
	0x0000,     /* 0x1144 */
	0x0000,     /* 0x1145 */
	0x0000,     /* 0x1146 */
	0x3180,     /* 0x1147 */
	0x0000,     /* 0x1148 */
	0x0000,     /* 0x1149 */
	0x0000,     /* 0x114a */
	0x0000,     /* 0x114b */
	0x3181,     /* 0x114c */
	0x0000,     /* 0x114d */
	0x0000,     /* 0x114e */
	0x0000,     /* 0x114f */
	0x0000,     /* 0x1150 */
	0x0000,     /* 0x1151 */
	0x0000,     /* 0x1152 */
	0x0000,     /* 0x1153 */
	0x0000,     /* 0x1154 */
	0x0000,     /* 0x1155 */
	0x0000,     /* 0x1156 */
	0x3184,     /* 0x1157 */
	0x3185,     /* 0x1158 */
	0x3186,     /* 0x1159 */
	0x0000,     /* 0x115a */
	0x0000,     /* 0x115b */
	0x0000,     /* 0x115c */
	0x0000,     /* 0x115d */
	0x0000,     /* 0x115e */
	0x0000,     /* 0x115f */
	0x3164,     /* 0x1160 */
	0x314f,     /* 0x1161 */
	0x3150,     /* 0x1162 */
	0x3151,     /* 0x1163 */
	0x3152,     /* 0x1164 */
	0x3153,     /* 0x1165 */
	0x3154,     /* 0x1166 */
	0x3155,     /* 0x1167 */
	0x3156,     /* 0x1168 */
	0x3157,     /* 0x1169 */
	0x3158,     /* 0x116a */
	0x3159,     /* 0x116b */
	0x315a,     /* 0x116c */
	0x315b,     /* 0x116d */
	0x315c,     /* 0x116e */
	0x315d,     /* 0x116f */
	0x315e,     /* 0x1170 */
	0x315f,     /* 0x1171 */
	0x3160,     /* 0x1172 */
	0x3161,     /* 0x1173 */
	0x3162,     /* 0x1174 */
	0x3163,     /* 0x1175 */
	0x0000,     /* 0x1176 */
	0x0000,     /* 0x1177 */
	0x0000,     /* 0x1178 */
	0x0000,     /* 0x1179 */
	0x0000,     /* 0x117a */
	0x0000,     /* 0x117b */
	0x0000,     /* 0x117c */
	0x0000,     /* 0x117d */
	0x0000,     /* 0x117e */
	0x0000,     /* 0x117f */
	0x0000,     /* 0x1180 */
	0x0000,     /* 0x1181 */
	0x0000,     /* 0x1182 */
	0x0000,     /* 0x1183 */
	0x3187,     /* 0x1184 */
	0x3188,     /* 0x1185 */
	0x0000,     /* 0x1186 */
	0x0000,     /* 0x1187 */
	0x3189,     /* 0x1188 */
	0x0000,     /* 0x1189 */
	0x0000,     /* 0x118a */
	0x0000,     /* 0x118b */
	0x0000,     /* 0x118c */
	0x0000,     /* 0x118d */
	0x0000,     /* 0x118e */
	0x0000,     /* 0x118f */
	0x0000,     /* 0x1190 */
	0x318a,     /* 0x1191 */
	0x318b,     /* 0x1192 */
	0x0000,     /* 0x1193 */
	0x318c,     /* 0x1194 */
	0x0000,     /* 0x1195 */
	0x0000,     /* 0x1196 */
	0x0000,     /* 0x1197 */
	0x0000,     /* 0x1198 */
	0x0000,     /* 0x1199 */
	0x0000,     /* 0x119a */
	0x0000,     /* 0x119b */
	0x0000,     /* 0x119c */
	0x0000,     /* 0x119d */
	0x318d,     /* 0x119e */
	0x0000,     /* 0x119f */
	0x0000,     /* 0x11a0 */
	0x318e,     /* 0x11a1 */
	0x0000,     /* 0x11a2 */
	0x0000,     /* 0x11a3 */
	0x0000,     /* 0x11a4 */
	0x0000,     /* 0x11a5 */
	0x0000,     /* 0x11a6 */
	0x0000,     /* 0x11a7 */
	0x3131,	    /* 0x11a8 */
	0x3132,	    /* 0x11a9 */
	0x3133,	    /* 0x11aa */
	0x3134,	    /* 0x11ab */
	0x3135,	    /* 0x11ac */
	0x3136,	    /* 0x11ad */
	0x3137,	    /* 0x11ae */
	0x3139,	    /* 0x11af */
	0x313a,	    /* 0x11b0 */
	0x313b,	    /* 0x11b1 */
	0x313c,	    /* 0x11b2 */
	0x313d,	    /* 0x11b3 */
	0x313e,	    /* 0x11b4 */
	0x313f,	    /* 0x11b5 */
	0x3140,	    /* 0x11b6 */
	0x3141,	    /* 0x11b7 */
	0x3142,	    /* 0x11b8 */
	0x3144,	    /* 0x11b9 */
	0x3145,	    /* 0x11ba */
	0x3146,	    /* 0x11bb */
	0x3147,	    /* 0x11bc */
	0x3148,	    /* 0x11bd */
	0x314a,	    /* 0x11be */
	0x314b,	    /* 0x11bf */
	0x314c,	    /* 0x11c0 */
	0x314d,	    /* 0x11c1 */
	0x314e,	    /* 0x11c2 */
	0x0000,     /* 0x11c3 */
	0x0000,     /* 0x11c4 */
	0x0000,     /* 0x11c5 */
	0x0000,     /* 0x11c6 */
	0x3167,     /* 0x11c7 */
	0x3168,     /* 0x11c8 */
	0x0000,     /* 0x11c9 */
	0x0000,     /* 0x11ca */
	0x0000,     /* 0x11cb */
	0x3169,     /* 0x11cc */
	0x0000,     /* 0x11cd */
	0x316a,     /* 0x11ce */
	0x0000,     /* 0x11cf */
	0x0000,     /* 0x11d0 */
	0x0000,     /* 0x11d1 */
	0x0000,     /* 0x11d2 */
	0x316b,     /* 0x11d3 */
	0x0000,     /* 0x11d4 */
	0x0000,     /* 0x11d5 */
	0x0000,     /* 0x11d6 */
	0x316c,     /* 0x11d7 */
	0x0000,     /* 0x11d8 */
	0x316d,     /* 0x11d9 */
	0x0000,     /* 0x11da */
	0x0000,     /* 0x11db */
	0x0000,     /* 0x11dc */
	0x316f,     /* 0x11dd */
	0x0000,     /* 0x11de */
	0x3170,     /* 0x11df */
	0x0000,     /* 0x11e0 */
	0x0000,     /* 0x11e1 */
	0x0000,     /* 0x11e2 */
	0x0000,     /* 0x11e3 */
	0x0000,     /* 0x11e4 */
	0x0000,     /* 0x11e5 */
	0x0000,     /* 0x11e6 */
	0x0000,     /* 0x11e7 */
	0x0000,     /* 0x11e8 */
	0x0000,     /* 0x11e9 */
	0x0000,     /* 0x11ea */
	0x0000,     /* 0x11eb */
	0x0000,     /* 0x11ec */
	0x0000,     /* 0x11ed */
	0x0000,     /* 0x11ee */
	0x0000,     /* 0x11ef */
	0x0000,     /* 0x11f0 */
	0x3182,     /* 0x11f1 */
	0x3183,     /* 0x11f2 */
	0x0000,     /* 0x11f3 */
	0x0000,     /* 0x11f4 */
	0x0000,     /* 0x11f5 */
	0x0000,     /* 0x11f6 */
	0x0000,     /* 0x11f7 */
	0x0000,     /* 0x11f8 */
	0x0000,     /* 0x11f9 */
	0x0000,     /* 0x11fa */
	0x0000,     /* 0x11fb */
	0x0000,     /* 0x11fc */
	0x0000,     /* 0x11fd */
	0x0000,     /* 0x11fe */
	0x0000,     /* 0x11ff */
    };

    static unsigned short jamo_ext_A_table[] = {
	0x0000,     /* 0xa960 */
	0x0000,     /* 0xa961 */
	0x0000,     /* 0xa962 */
	0x0000,     /* 0xa963 */
	0x313a,     /* 0xa964 */
	0x0000,     /* 0xa965 */
	0x316a,     /* 0xa966 */
	0x0000,     /* 0xa967 */
	0x313b,     /* 0xa968 */
	0x313c,     /* 0xa969 */
	0x0000,     /* 0xa96a */
	0x0000,     /* 0xa96b */
	0x313d,     /* 0xa96c */
	0x0000,     /* 0xa96d */
	0x0000,     /* 0xa96e */
	0x0000,     /* 0xa96f */
	0x0000,     /* 0xa970 */
	0x316f,     /* 0xa971 */
	0x0000,     /* 0xa972 */
	0x0000,     /* 0xa973 */
	0x0000,     /* 0xa974 */
	0x0000,     /* 0xa975 */
	0x0000,     /* 0xa976 */
	0x0000,     /* 0xa977 */
	0x0000,     /* 0xa978 */
	0x0000,     /* 0xa979 */
	0x0000,     /* 0xa97a */
	0x0000,     /* 0xa97b */
	0x0000,     /* 0xa97c */
    };

    static unsigned short jamo_ext_B_table[] = {
	0x0000,     /* 0xd7b0 */
	0x0000,     /* 0xd7b1 */
	0x0000,     /* 0xd7b2 */
	0x0000,     /* 0xd7b3 */
	0x0000,     /* 0xd7b4 */
	0x0000,     /* 0xd7b5 */
	0x0000,     /* 0xd7b6 */
	0x0000,     /* 0xd7b7 */
	0x0000,     /* 0xd7b8 */
	0x0000,     /* 0xd7b9 */
	0x0000,     /* 0xd7ba */
	0x0000,     /* 0xd7bb */
	0x0000,     /* 0xd7bc */
	0x0000,     /* 0xd7bd */
	0x0000,     /* 0xd7be */
	0x0000,     /* 0xd7bf */
	0x0000,     /* 0xd7c0 */
	0x0000,     /* 0xd7c1 */
	0x0000,     /* 0xd7c2 */
	0x0000,     /* 0xd7c3 */
	0x0000,     /* 0xd7c4 */
	0x0000,     /* 0xd7c5 */
	0x0000,     /* 0xd7c6 */
	0x0000,     /* 0xd7cb */
	0x0000,     /* 0xd7cc */
	0x3138,     /* 0xd7cd */
	0x0000,     /* 0xd7ce */
	0x0000,     /* 0xd7cf */
	0x0000,     /* 0xd7d0 */
	0x0000,     /* 0xd7d1 */
	0x0000,     /* 0xd7d2 */
	0x0000,     /* 0xd7d3 */
	0x0000,     /* 0xd7d4 */
	0x0000,     /* 0xd7d5 */
	0x0000,     /* 0xd7d6 */
	0x0000,     /* 0xd7d7 */
	0x0000,     /* 0xd7d8 */
	0x0000,     /* 0xd7d9 */
	0x0000,     /* 0xd7da */
	0x0000,     /* 0xd7db */
	0x0000,     /* 0xd7dc */
	0x0000,     /* 0xd7dd */
	0x0000,     /* 0xd7de */
	0x0000,     /* 0xd7df */
	0x0000,     /* 0xd7e0 */
	0x0000,     /* 0xd7e1 */
	0x0000,     /* 0xd7e2 */
	0x3173,     /* 0xd7e3 */
	0x0000,     /* 0xd7e4 */
	0x0000,     /* 0xd7e5 */
	0x3143,     /* 0xd7e6 */
	0x3175,     /* 0xd7e7 */
	0x3176,     /* 0xd7e8 */
	0x0000,     /* 0xd7e9 */
	0x0000,     /* 0xd7ea */
	0x0000,     /* 0xd7eb */
	0x0000,     /* 0xd7ec */
	0x0000,     /* 0xd7ed */
	0x0000,     /* 0xd7ee */
	0x317e,     /* 0xd7ef */
	0x0000,     /* 0xd7f0 */
	0x0000,     /* 0xd7f1 */
	0x0000,     /* 0xd7f2 */
	0x0000,     /* 0xd7f3 */
	0x0000,     /* 0xd7f4 */
	0x0000,     /* 0xd7f5 */
	0x0000,     /* 0xd7f6 */
	0x0000,     /* 0xd7f7 */
	0x0000,     /* 0xd7f8 */
	0x3149,     /* 0xd7f9 */
	0x0000,     /* 0xd7fa */
	0x0000,     /* 0xd7fb */
    };

    ucschar ret = 0;

    if (c >= 0x1100 && c <= 0x11ff) {
	ret = jamo_table[c - 0x1100];
    } else if (c >= 0xa960 && c <= 0xa97c) {
	ret = jamo_ext_A_table[c - 0xa960];
    } else if (c >= 0xd7b0 && c <= 0xd7fb) {
	ret = jamo_ext_B_table[c - 0xd7b0];
    }

    if (ret == 0)
	ret = c;

    return ret;
}

/** @deprecated 이 함수 대신 hangul_jamo_to_cjamo()함수를 사용한다. */
ucschar
hangul_jaso_to_jamo(ucschar c)
{
    return hangul_jamo_to_cjamo(c);
}

ucschar
hangul_choseong_to_jongseong(ucschar c)
{
    static ucschar table[] = {
	0x11a8,  /* choseong kiyeok      -> jongseong kiyeok      */
	0x11a9,  /* choseong ssangkiyeok -> jongseong ssangkiyeok */
	0x11ab,  /* choseong nieun       -> jongseong nieun       */
	0x11ae,  /* choseong tikeut      -> jongseong tikeut      */
	0x0,     /* choseong ssangtikeut -> jongseong tikeut      */
	0x11af,  /* choseong rieul       -> jongseong rieul       */
	0x11b7,  /* choseong mieum       -> jongseong mieum       */
	0x11b8,  /* choseong pieup       -> jongseong pieup       */
	0x0,     /* choseong ssangpieup  -> jongseong pieup       */
	0x11ba,  /* choseong sios        -> jongseong sios        */
	0x11bb,  /* choseong ssangsios   -> jongseong ssangsios   */
	0x11bc,  /* choseong ieung       -> jongseong ieung       */
	0x11bd,  /* choseong cieuc       -> jongseong cieuc       */
	0x0,     /* choseong ssangcieuc  -> jongseong cieuc       */
	0x11be,  /* choseong chieuch     -> jongseong chieuch     */
	0x11bf,  /* choseong khieukh     -> jongseong khieukh     */
	0x11c0,  /* choseong thieuth     -> jongseong thieuth     */
	0x11c1,  /* choseong phieuph     -> jongseong phieuph     */
	0x11c2   /* choseong hieuh       -> jongseong hieuh       */
    };
    if (c < 0x1100 || c > 0x1112)
	return 0;
    return table[c - 0x1100];
}

ucschar
hangul_jongseong_to_choseong(ucschar c)
{
    static ucschar table[] = {
      0x1100,  /* jongseong kiyeok        -> choseong kiyeok       */
      0x1101,  /* jongseong ssangkiyeok   -> choseong ssangkiyeok  */
      0x1109,  /* jongseong kiyeok-sios   -> choseong sios         */
      0x1102,  /* jongseong nieun         -> choseong nieun        */
      0x110c,  /* jongseong nieun-cieuc   -> choseong cieuc        */
      0x1112,  /* jongseong nieun-hieuh   -> choseong hieuh        */
      0x1103,  /* jongseong tikeut        -> choseong tikeut       */
      0x1105,  /* jongseong rieul         -> choseong rieul        */
      0x1100,  /* jongseong rieul-kiyeok  -> choseong kiyeok       */
      0x1106,  /* jongseong rieul-mieum   -> choseong mieum        */
      0x1107,  /* jongseong rieul-pieup   -> choseong pieup        */
      0x1109,  /* jongseong rieul-sios    -> choseong sios         */
      0x1110,  /* jongseong rieul-thieuth -> choseong thieuth      */
      0x1111,  /* jongseong rieul-phieuph -> choseong phieuph      */
      0x1112,  /* jongseong rieul-hieuh   -> choseong hieuh        */
      0x1106,  /* jongseong mieum         -> choseong mieum        */
      0x1107,  /* jongseong pieup         -> choseong pieup        */
      0x1109,  /* jongseong pieup-sios    -> choseong sios         */
      0x1109,  /* jongseong sios          -> choseong sios         */
      0x110a,  /* jongseong ssangsios     -> choseong ssangsios    */
      0x110b,  /* jongseong ieung         -> choseong ieung        */
      0x110c,  /* jongseong cieuc         -> choseong cieuc        */
      0x110e,  /* jongseong chieuch       -> choseong chieuch      */
      0x110f,  /* jongseong khieukh       -> choseong khieukh      */
      0x1110,  /* jongseong thieuth       -> choseong thieuth      */
      0x1111,  /* jongseong phieuph       -> choseong phieuph      */
      0x1112   /* jongseong hieuh         -> choseong hieuh        */
    };
    if (c < 0x11a8 || c > 0x11c2)
	return 0;
    return table[c - 0x11a8];
}

void
hangul_jongseong_dicompose(ucschar c, ucschar* jong, ucschar* cho)
{
    static ucschar table[][2] = {
    { 0,      0x1100 }, /* jong kiyeok	      = cho  kiyeok               */
    { 0x11a8, 0x1100 }, /* jong ssangkiyeok   = jong kiyeok + cho kiyeok  */
    { 0x11a8, 0x1109 }, /* jong kiyeok-sios   = jong kiyeok + cho sios    */
    { 0,      0x1102 }, /* jong nieun	      = cho  nieun                */
    { 0x11ab, 0x110c }, /* jong nieun-cieuc   = jong nieun  + cho cieuc   */
    { 0x11ab, 0x1112 }, /* jong nieun-hieuh   = jong nieun  + cho hieuh   */
    { 0,      0x1103 }, /* jong tikeut	      = cho  tikeut               */
    { 0,      0x1105 }, /* jong rieul         = cho  rieul                */
    { 0x11af, 0x1100 }, /* jong rieul-kiyeok  = jong rieul  + cho kiyeok  */
    { 0x11af, 0x1106 }, /* jong rieul-mieum   = jong rieul  + cho mieum   */
    { 0x11af, 0x1107 }, /* jong rieul-pieup   = jong rieul  + cho pieup   */
    { 0x11af, 0x1109 }, /* jong rieul-sios    = jong rieul  + cho sios    */
    { 0x11af, 0x1110 }, /* jong rieul-thieuth = jong rieul  + cho thieuth */
    { 0x11af, 0x1111 }, /* jong rieul-phieuph = jong rieul  + cho phieuph */
    { 0x11af, 0x1112 }, /* jong rieul-hieuh   = jong rieul  + cho hieuh   */
    { 0,      0x1106 }, /* jong mieum         = cho  mieum                */
    { 0,      0x1107 }, /* jong pieup         = cho  pieup                */
    { 0x11b8, 0x1109 }, /* jong pieup-sios    = jong pieup  + cho sios    */
    { 0,      0x1109 }, /* jong sios          = cho  sios                 */
    { 0x11ba, 0x1109 }, /* jong ssangsios     = jong sios   + cho sios    */
    { 0,      0x110b }, /* jong ieung         = cho  ieung                */
    { 0,      0x110c }, /* jong cieuc         = cho  cieuc                */
    { 0,      0x110e }, /* jong chieuch       = cho  chieuch              */
    { 0,      0x110f }, /* jong khieukh       = cho  khieukh              */
    { 0,      0x1110 }, /* jong thieuth       = cho  thieuth              */
    { 0,      0x1111 }, /* jong phieuph       = cho  phieuph              */
    { 0,      0x1112 }  /* jong hieuh         = cho  hieuh                */
    };

    *jong = table[c - 0x11a8][0];
    *cho  = table[c - 0x11a8][1];
}

/**
 * @ingroup hangulctype
 * @brief 자모 코드를 조합하여 한글 음절로 변환
 * @param choseong 초성이 될 UCS4 코드 값
 * @param jungseong 중성이 될 UCS4 코드 값
 * @param jongseong 종성이 될 UCS4 코드 값
 * @return @a choseong @a jungseong @a jongseong을 조합한 현대 한글 음절 코드,
 *         또는 0
 *
 * 이 함수는 @a choseong @a jungseong @a jongseong으로 주어진 코드 값을 각각
 * 초성, 중성, 종성으로 하는 현대 한글 음절 코드를 구한다.
 * @a choseong @a jungseong @a jongseong 이 조합 가능한 코드가 아니라면 
 * 0을 리턴한다. 종성이 없는 글자를 만들기 위해서는 jongseong에 0을 주면 된다.
 */
ucschar
hangul_jamo_to_syllable(ucschar choseong, ucschar jungseong, ucschar jongseong)
{
    ucschar c;

    /* we use 0x11a7 like a Jongseong filler */
    if (jongseong == 0)
	jongseong = 0x11a7;         /* Jongseong filler */

    if (!hangul_is_choseong_conjoinable(choseong))
	return 0;
    if (!hangul_is_jungseong_conjoinable(jungseong))
	return 0;
    if (!hangul_is_jongseong_conjoinable(jongseong))
	return 0;

    choseong  -= choseong_base;
    jungseong -= jungseong_base;
    jongseong -= jongseong_base;

    c = ((choseong * njungseong) + jungseong) * njongseong + jongseong
	+ syllable_base;
    return c;
}

/** @deprecated 이 함수 대신 hangul_jamo_to_syllable()을 사용한다. */
ucschar
hangul_jaso_to_syllable(ucschar choseong, ucschar jungseong, ucschar jongseong)
{
    return hangul_jamo_to_syllable(choseong, jungseong, jongseong);
}

/**
 * @ingroup hangulctype
 * @brief 음절을 자모로 분해
 * @param syllable 분해할 음절
 * @retval choseong 음절에서 초성 부분의 코드
 * @retval jungseong 음절에서 중성 부분의 코드
 * @retval jongseong 음절에서 종성 부분의 코드, 종성이 없으면 0을 반환한다
 * @return 없음
 *
 * 이 함수는 @a syllable 로 주어진 음절 코드를 분해하여 자모 코드를 반환한다.
 * 반환하는 값은 @a choseong, @a jungseong, @a jongseong 의 포인터에 대입하여
 * 리턴한다. 종성이 없는 음절인 경우에는 @a jongseong 에 0을 반환한다.
 */
void
hangul_syllable_to_jamo(ucschar syllable,
			ucschar* choseong,
			ucschar* jungseong,
			ucschar* jongseong)
{
    if (jongseong != NULL)
	*jongseong = 0;
    if (jungseong != NULL)
	*jungseong = 0;
    if (choseong != NULL)
	*choseong = 0;

    if (!hangul_is_syllable(syllable))
	return;

    syllable -= syllable_base;
    if (jongseong != NULL) {
	if (syllable % njongseong != 0)
	    *jongseong = jongseong_base + syllable % njongseong;
    }
    syllable /= njongseong;

    if (jungseong != NULL) {
	*jungseong = jungseong_base + syllable % njungseong;
    }
    syllable /= njungseong;

    if (choseong != NULL) {
	*choseong = choseong_base + syllable;
    }
}

/** @deprecated 이 함수 대신 hangul_syllable_to_jamo함수를 사용한다. */
void
hangul_syllable_to_jaso(ucschar syllable,
			ucschar* choseong,
			ucschar* jungseong,
			ucschar* jongseong)
{
    return hangul_syllable_to_jamo(syllable, choseong, jungseong, jongseong);
}

static inline bool 
is_syllable_boundary(ucschar prev, ucschar next)
{
    if (hangul_is_choseong(prev)) {
	if (hangul_is_choseong(next))
	    return false;
	if (hangul_is_jungseong(next))
	    return false;
	if (hangul_is_syllable(next))
	    return false;
	if (hangul_is_combining_mark(next))
	    return false;
	if (next == HANGUL_JUNGSEONG_FILLER)
	    return false;
    } else if (prev == HANGUL_CHOSEONG_FILLER) {
	if (hangul_is_jungseong(next))
	    return false;
	if (next == HANGUL_JUNGSEONG_FILLER)
	    return false;
    } else if (hangul_is_jungseong(prev)) {
	if (hangul_is_jungseong(next))
	    return false;
	if (hangul_is_jongseong(next))
	    return false;
	if (hangul_is_combining_mark(next))
	    return false;
    } else if (prev == HANGUL_JUNGSEONG_FILLER) {
	if (hangul_is_jongseong(next))
	    return false;
    } else if (hangul_is_jongseong(prev)) {
	if (hangul_is_jongseong(next))
	    return false;
	if (hangul_is_combining_mark(next))
	    return false;
    } else if (hangul_is_syllable(prev)) {
	if ((prev - syllable_base) % njongseong == 0) {
	    // 종성이 없는 음절: LV
	    if (hangul_is_jungseong(next))
		return false;
	    if (hangul_is_jongseong(next))
		return false;
	} else {
	    // 종성이 있는 음절: LVT
	    if (hangul_is_jongseong(next))
		return false;
	}
	if (hangul_is_combining_mark(next))
	    return false;
    }
    
    return true;
}

static inline ucschar
choseong_compress(ucschar a, ucschar b)
{
    if (a == 0)
	return b;

    if (a == 0x1100 && b == 0x1100)
	return 0x1101;
    if (a == 0x1103 && b == 0x1103)
	return 0x1104;
    if (a == 0x1107 && b == 0x1107)
	return 0x1108;
    if (a == 0x1109 && b == 0x1109)
	return 0x110A;
    if (a == 0x110c && b == 0x110c)
	return 0x110d;
    return 0;
}

static inline ucschar
jungseong_compress(ucschar a, ucschar b)
{
    if (a == 0)
	return b;

    if (a == 0x1169) {
	if (b == 0x1161)
	    return 0x116a;
	if (b == 0x1162)
	    return 0x116b;
	if (b == 0x1175)
	    return 0x116c;
    }
    if (a == 0x116e) {
	if (b == 0x1165)
	    return 0x116f;
	if (b == 0x1166)
	    return 0x1170;
	if (b == 0x1175)
	    return 0x1171;
    }
    if (b == 0x1175) {
	if (a == 0x1173)
	    return 0x1174;
	if (a == 0x1161)
	    return 0x1162;
	if (a == 0x1163)
	    return 0x1164;
	if (a == 0x1165)
	    return 0x1166;
	if (a == 0x1167)
	    return 0x1168;
    }

    return 0;
}

static inline ucschar
jongseong_compress(ucschar a, ucschar b)
{
    if (a == 0)
	return b;
    
    if (a == 0x11a8) {
	if (b == 0x11a8)
	    return 0x11a9;
	if (b == 0x11ba)
	    return 0x11aa;
    }
    if (a == 0x11ab) {
	if (b == 0x11b0)
	    return 0x11ab;
	if (b == 0x11c2)
	    return 0x11ad;
    }
    if (a == 0x11af) {
	if (b == 0x11a8)
	    return 0x11b0;
	if (b == 0x11b7)
	    return 0x11b1;
	if (b == 0x11b8)
	    return 0x11b2;
	if (b == 0x11ba)
	    return 0x11b3;
	if (b == 0x11c0)
	    return 0x11b4;
	if (b == 0x11c1)
	    return 0x11b5;
	if (b == 0x11c2)
	    return 0x11b6;
    }
    if (a == 0x11b8 && b == 0x11ba)
	return 0x11b9;
    if (a == 0x11ba && b == 0x11ba)
	return 0x11bb;

    return 0;
}

static inline ucschar
build_syllable(const ucschar* str, size_t len)
{
    int i;
    ucschar cho = 0, jung = 0, jong = 0;

    i = 0;
    while (i < len && hangul_is_choseong_conjoinable(str[i])) {
	cho = choseong_compress(cho, str[i]);
	if (cho == 0)
	    return 0;
	i++;
    }

    while (i < len && hangul_is_jungseong_conjoinable(str[i])) {
	jung = jungseong_compress(jung, str[i]);
	if (jung == 0)
	    return 0;
	i++;
    }

    while (i < len && hangul_is_jongseong_conjoinable(str[i])) {
	jong = jongseong_compress(jong, str[i]);
	if (jong == 0)
	    return 0;
	i++;
    }

    if (i < len)
	return 0;

    return hangul_jamo_to_syllable(cho, jung, jong);
}

/**
 * @ingroup hangulctype
 * @brief 한 음절에 해당하는 코드의 갯수를 구하는 함수
 * @param str 음절의 길이를 구할 스트링
 * @param max_len @a str 에서 읽을 길이의 제한값
 * @return 한 음절에 해당하는 코드의 갯수
 *
 * 이 함수는 @a str 에서 한 음절에 해당하는 코드의 갯수를 구한다. 
 * 한 음절에 해당하는 코드의 갯수가 @a max_len 보다 많다면 @a max_len 을 
 * 반환한다. 한 음절이라고 판단하는 기준은 L*V*T+ 패턴에 따른다. 이 패턴은
 * regular expression의 컨벤션을 따른 것으로, 1개 이상의 초성과 중성, 0개
 * 이상의 종성이 모인 자모 스트링을 한 음절로 인식한다는 뜻이다. 예를 들면
 * 다음과 같은 자모 스트링도 한 음절로 인식한다.
 *
 *  예) "ㅂ ㅂ ㅜ ㅔ ㄹ ㄱ" -> "쀍"
 * 
 * 따라서 위 경우에는 6을 반환하게 된다. 
 *
 * 일반적으로는 방점(U+302E, U+302F)까지 한 음절로 인식하겠지만, 이 함수는
 * 음절과 자모간 변환을 편리하게 하기 위해 구현된 것으로 방점은 다른 음절로 
 * 인식한다.
 *
 * @a str 이 자모 코드에 해당하지 않는 경우에는 1을 반환한다.
 *
 * 이 함수는 자모 스트링에서 총 음절의 갯수를 구하는 함수가 아님에 주의한다.
 */
int
hangul_syllable_len(const ucschar* str, int max_len)
{
    int i = 0;

    if (max_len == 0)
	return 0;

    if (str[i] != 0) {
	for (i = 1; i < max_len; i++) {
	    if (str[i] == 0)
		break;

	    if (is_syllable_boundary(str[i - 1], str[i]))
		break;
	}
    }

    return i;
}

/**
 * @ingroup hangulctype
 * @brief @a iter를 기준으로 이전 음절의 첫자모 글자에 대한 포인터를 구하는 함수
 * @param iter 현재 위치
 * @param begin 스트링의 시작위치, 포인터가 이동할 한계값
 * @return 이전 음절의 첫번째 자모에 대한 포인터
 *
 * 이 함수는 @a iter로 주어진 자모 스트링의 포인터를 기준으로 이전 음절의 
 * 첫번째 자모에 대한 포인터를 리턴한다. 음절을 찾기위해서 begin보다 
 * 앞쪽으로 이동하지 않는다. 
 *
 * 한 음절이라고 판단하는 기준은 L*V*T+M? 패턴에 따른다.
 */
const ucschar*
hangul_syllable_iterator_prev(const ucschar* iter, const ucschar* begin)
{
    if (iter > begin)
	iter--;

    while (iter > begin) {
	ucschar prev = iter[-1];
	ucschar curr = iter[0];
	if (is_syllable_boundary(prev, curr))
	    break;
	iter--;
    }

    return iter;
}

/**
 * @ingroup hangulctype
 * @brief @a iter를 기준으로 다음 음절의 첫자모 글자에 대한 포인터를 구하는 함수
 * @param iter 현재 위치
 * @param end 스트링의 끝위치, 포인터가 이동할 한계값
 * @return 다음 음절의 첫번째 자모에 대한 포인터
 *
 * 이 함수는 @a iter로 주어진 자모 스트링의 포인터를 기준으로 다음 음절의 
 * 첫번째 자모에 대한 포인터를 리턴한다. 음절을 찾기위해서 end를 넘어
 * 이동하지 않는다. 
 *
 * 한 음절이라고 판단하는 기준은 L*V*T+M? 패턴에 따른다.
 */
const ucschar*
hangul_syllable_iterator_next(const ucschar* iter, const ucschar* end)
{
    if (iter < end)
	iter++;

    while (iter < end) {
	ucschar prev = iter[-1];
	ucschar curr = iter[0];
	if (is_syllable_boundary(prev, curr))
	    break;
	iter++;
    }

    return iter;
}

/**
 * @ingroup hangulctype
 * @brief 자모 스트링을 음절 스트링으로 변환
 * @param dest 음절형으로 변환된 결과가 저장될 버퍼
 * @param destlen 결과를 저장할 버퍼의 길이(ucschar 코드 단위)
 * @param src 변환할 자모 스트링
 * @param srclen 변환할 자모 스트링의 길이(ucschar 코드 단위)
 * @return @a destlen 에 저장한 코드의 갯수
 *
 * 이 함수는 L+V+T*M? 패턴에 따라 자모 스트링 변환을 시도한다. 한 음절을 
 * 판단하는 기준은 @ref hangul_syllable_len 을 참조한다.
 * 만일 @a src 가 적절한 음절형태로 변환이 불가능한 경우에는 자모 스트링이
 * 그대로 복사된다.
 *
 * 이 함수는 자모 스트링 @a src 를 음절형으로 변환하여 @a dest 에 저장한다.
 * @a srclen 에 지정된 갯수만큼 읽고, @a destlen 에 지정된 길이 이상 쓰지
 * 않는다.  @a srclen 이 -1이라면 @a src 는 0으로 끝나는 스트링으로 가정하고
 * 0을 제외한 길이까지 변환을 시도한다. 따라서 변환된 결과 스트링은 0으로 
 * 끝나지 않는다. 만일 0으로 끝나는 스트링을 만들고 싶다면 다음과 같이 한다.
 *
 * @code
 * int n = hangul_jamos_to_syllables(dest, destlen, src, srclen);
 * dest[n] = 0;
 * @endcode
 */
int
hangul_jamos_to_syllables(ucschar* dest, int destlen, const ucschar* src, int srclen)
{
    ucschar* d;
    const ucschar* s;

    int inleft;
    int outleft;
    int n;

    if (srclen < 0) {
	s = src;
	while (*s != 0)
	    s++;
	srclen = s - src;
    }

    s = src;
    d = dest;
    inleft = srclen;
    outleft = destlen;

    n = hangul_syllable_len(s, inleft);
    while (n > 0 && inleft > 0 && outleft > 0) {
	ucschar c = build_syllable(s, n);
	if (c != 0) {
	    *d = c;
	    d++;
	    outleft--;
	} else {
	    int i;
	    for (i = 0; i < n && i < outleft; i++) {
		d[i] = s[i];
	    }
	    d += i;
	    outleft -= i;
	}

	s += n;
	inleft -= n;
	n = hangul_syllable_len(s, inleft);
    }

    return destlen - outleft;
}
