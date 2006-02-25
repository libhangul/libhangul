/* libhangul
 * Copyright (C) 2004 - 2006 Choe Hwanjin
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

typedef uint32_t ucschar;

bool hangul_is_choseong(ucschar c);
bool hangul_is_jungseong(ucschar c);
bool hangul_is_jongseong(ucschar c);
bool hangul_is_choseong_conjoinable(ucschar c);
bool hangul_is_jungseong_conjoinable(ucschar c);
bool hangul_is_jongseong_conjoinable(ucschar c);
bool hangul_is_syllable(ucschar c);
bool hangul_is_jaso(ucschar c);
bool hangul_is_jamo(ucschar c);

ucschar hangul_jaso_to_jamo(ucschar ch);
ucschar hangul_choseong_to_jamo(ucschar ch);
ucschar hangul_jungseong_to_jamo(ucschar ch);
ucschar hangul_jongseong_to_jamo(ucschar ch);

ucschar hangul_choseong_to_jongseong(ucschar ch);
ucschar hangul_jongseong_to_choseong(ucschar ch);
void    hangul_jongseong_dicompose(ucschar ch, ucschar* jong, ucschar* cho);

ucschar hangul_jaso_to_syllable(ucschar choseong,
				ucschar jungseong,
				ucschar jongseong);

/* hangulinputcontext.c */
typedef struct _HangulJamoCombination HangulJamoCombination;
typedef struct _HangulBuffer          HangulBuffer;
typedef struct _HangulInputContext    HangulInputContext;
typedef bool (*HangulICFilter) (ucschar*, ucschar, ucschar, ucschar, void*);

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
    HANGUL_INPUT_FILTER_JAMO,
    HANGUL_INPUT_FILTER_JASO
};

struct _HangulJamoCombination {
    uint32_t key;
    ucschar code;
};

struct _HangulBuffer {
    ucschar choseong;
    ucschar jungseong;
    ucschar jongseong;

    ucschar stack[12];
    int     index;
};

struct _HangulInputContext {
    int type;
    const ucschar *keyboard_table;
    const HangulJamoCombination *combination_table;
    int combination_table_size;
    HangulBuffer buffer;
    HangulICFilter filter;
    void *filter_data;
    int output_mode;

    ucschar preedit_string[64];
    ucschar commit_string[64];
};

HangulInputContext* hangul_ic_new(HangulKeyboardType keyboard);
void hangul_ic_delete(HangulInputContext *hic);
bool hangul_ic_process(HangulInputContext *hic, int ascii);
void hangul_ic_reset(HangulInputContext *hic);
void hangul_ic_flush(HangulInputContext *hic);
bool hangul_ic_backspace(HangulInputContext *hic);
bool hangul_ic_is_empty(HangulInputContext *hic);

void hangul_ic_set_output_mode(HangulInputContext *hic, int mode);
void hangul_ic_set_keyboard(HangulInputContext *hic,
			    HangulKeyboardType keyboard);
void hangul_ic_set_filter(HangulInputContext *hic,
			  HangulICFilter func, void *user_data);

const ucschar* hangul_ic_get_preedit_string(HangulInputContext *hic);
const ucschar* hangul_ic_get_commit_string(HangulInputContext *hic);

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
