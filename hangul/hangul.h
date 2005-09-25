/* libhangul
 * Copyright (C) 2004 Choe Hwanjin
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef _HANGUL_H_
#define _HANGUL_H_

#include <wchar.h>
#include <stdbool.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/* hangulctype.c */
enum {
    HANGUL_CHOSEONG_FILLER  = 0x115f,   /* hangul choseong filler */
    HANGUL_JUNGSEONG_FILLER = 0x1160    /* hangul jungseong filler */
};

bool hangul_is_syllable(wchar_t ch);
bool hangul_is_choseong(wchar_t ch);
bool hangul_is_jungseong(wchar_t ch);
bool hangul_is_jongseong(wchar_t ch);

wchar_t hangul_choseong_to_cjamo(wchar_t ch);
wchar_t hangul_jungseong_to_cjamo(wchar_t ch);
wchar_t hangul_jongseong_to_cjamo(wchar_t ch);

wchar_t hangul_choseong_to_jongseong(wchar_t ch);
wchar_t hangul_jongseong_to_choseong(wchar_t ch);
void    hangul_jongseong_dicompose(wchar_t ch, wchar_t* jong, wchar_t* cho);

wchar_t hangul_jamo_to_syllable(wchar_t choseong,
				wchar_t jungseong,
				wchar_t jongseong);

/* hangulinputcontext.c */
typedef struct _HangulJamoCombination HangulJamoCombination;
typedef struct _HangulBuffer HangulBuffer;
typedef struct _HangulInputContext HangulInputContext;

typedef enum {
    HANGUL_KEYBOARD_2,
    HANGUL_KEYBOARD_32,
    HANGUL_KEYBOARD_3FINAL,
    HANGUL_KEYBOARD_390,
    HANGUL_KEYBOARD_3NOSHIFT,
    HANGUL_KEYBOARD_3YETGUL
} HangulKeyboardType;

enum {
    HANGUL_OUTPUT_SYLLABLE,
    HANGUL_OUTPUT_JAMO
};

enum {
    HANGUL_INPUT_FILTER_2,
    HANGUL_INPUT_FILTER_3
};

struct _HangulJamoCombination {
    uint32_t key;
    wchar_t code;
};

struct _HangulBuffer {
    wchar_t choseong;
    wchar_t jungseong;
    wchar_t jongseong;

    wchar_t stack[12];
    int     index;
};

struct _HangulInputContext {
    int type;
    const wchar_t *keyboard_table;
    const HangulJamoCombination *combination_table;
    int combination_table_size;
    HangulBuffer buffer;
    int output_mode;

    wchar_t preedit_string[64];
    wchar_t commit_string[64];
};

HangulInputContext* hangul_ic_new(HangulKeyboardType keyboard);
void hangul_ic_delete(HangulInputContext *hic);
bool hangul_ic_filter(HangulInputContext *hic, int ascii);
void hangul_ic_reset(HangulInputContext *hic);
bool hangul_ic_backspace(HangulInputContext *hic);

void hangul_ic_set_output_mode(HangulInputContext *hic, int mode);
void hangul_ic_set_keyboard(HangulInputContext *hic,
			    HangulKeyboardType keyboard);
const wchar_t* hangul_ic_get_preedit_string(HangulInputContext *hic);
const wchar_t* hangul_ic_get_commit_string(HangulInputContext *hic);

/* hanja.c */
enum {
    HANJA_MATCH_EXACT,
    HANJA_MATCH_PREFIX
};

typedef struct _Hanja Hanja;
typedef struct _HanjaList HanjaList;
typedef struct _HanjaTable HanjaTable;

struct _Hanja {
    const char *key;
    const char *value;
    const char *comment;
};

struct _HanjaList {
    const char *key;
    int nitems;
    Hanja **items; 
};

struct _HanjaTable {
    int nmember;
    HanjaList **base;
};

HanjaTable* hanja_table_load(const char *filename);
HanjaList*  hanja_table_match(const HanjaTable* table,
			      int option, const char *key);
void hanja_table_destroy(HanjaTable *table);
void hanja_list_destroy(HanjaList *list);

#ifdef __cplusplus
}
#endif

#endif /* _HANGUL_H_ */
