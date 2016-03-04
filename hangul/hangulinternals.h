/* libhangul
 * Copyright (C) 2016 Choe Hwanjin
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef libhangul_hangulinternals_h
#define libhangul_hangulinternals_h

#define N_ELEMENTS(array) (sizeof (array) / sizeof ((array)[0]))
#ifndef countof
#define countof(array) (sizeof (array) / sizeof ((array)[0]))
#endif

ucschar hangul_jongseong_get_diff(ucschar prevjong, ucschar jong);

ucschar hangul_choseong_to_jongseong(ucschar ch);
ucschar hangul_jongseong_to_choseong(ucschar ch);
void    hangul_jongseong_decompose(ucschar ch, ucschar* jong, ucschar* cho);

int     hangul_keyboard_get_type(const HangulKeyboard *keyboard);
ucschar hangul_keyboard_combine(const HangulKeyboard* keyboard,
	    unsigned id, ucschar first, ucschar second);
ucschar hangul_keyboard_get_mapping(const HangulKeyboard* keyboard,
	    int tableid, unsigned key);

int hangul_keyboard_list_init();
int hangul_keyboard_list_fini();

const HangulKeyboard* hangul_keyboard_list_get_keyboard(const char* id);

#endif /* libhangul_hangulinternals_h */
