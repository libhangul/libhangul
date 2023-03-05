/* libhangul
 * Copyright (C) 2004 - 2007 Choe Hwanjin
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

#ifndef libhangul_hangul_h
#define libhangul_hangul_h

#include <stdbool.h>
#include <inttypes.h>

#if defined(_WIN32)
#    if _MSC_VER || __GNUC__
#        define LIBHANGUL_EXPORT __declspec(dllexport)
#        define LIBHANGUL_IMPORT __declspec(dllimport)
#    else
#        define LIBHANGUL_EXPORT
#        define LIBHANGUL_IMPORT
#    endif
#else
#    ifdef __GNUC__
#        define LIBHANGUL_EXPORT __attribute__((visibility ("default")))
#        define LIBHANGUL_IMPORT __attribute__((visibility ("default")))
#    else
#        define LIBHANGUL_EXPORT
#        define LIBHANGUL_IMPORT
#    endif
#endif

#if LIBHANGUL_ENABLE_STATIC
#   define LIBHANGUL_EXTERN_SYMBOL
#else
#   if LIBHANGUL_ENABLE_DLL_EXPORT
#       define LIBHANGUL_EXTERN_SYMBOL LIBHANGUL_EXPORT
#   else
#       define LIBHANGUL_EXTERN_SYMBOL LIBHANGUL_IMPORT
#   endif
#endif

#define LIBHANGUL_API LIBHANGUL_EXTERN_SYMBOL

#ifdef __GNUC__
#define LIBHANGUL_DEPRECATED __attribute__((deprecated))
#else
#define LIBHANGUL_DEPRECATED
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* hangulctype.c */
enum {
    HANGUL_CHOSEONG_FILLER  = 0x115f,   /* hangul choseong filler */
    HANGUL_JUNGSEONG_FILLER = 0x1160    /* hangul jungseong filler */
};

typedef uint32_t ucschar;

LIBHANGUL_API
bool hangul_is_choseong(ucschar c);
LIBHANGUL_API
bool hangul_is_jungseong(ucschar c);
LIBHANGUL_API
bool hangul_is_jongseong(ucschar c);
LIBHANGUL_API
bool hangul_is_choseong_conjoinable(ucschar c);
LIBHANGUL_API
bool hangul_is_jungseong_conjoinable(ucschar c);
LIBHANGUL_API
bool hangul_is_jongseong_conjoinable(ucschar c);
LIBHANGUL_API
bool hangul_is_jamo_conjoinable(ucschar c);
LIBHANGUL_API
bool hangul_is_syllable(ucschar c);
LIBHANGUL_API
bool hangul_is_jamo(ucschar c);
LIBHANGUL_API
bool hangul_is_cjamo(ucschar c);

LIBHANGUL_API
ucschar hangul_jamo_to_cjamo(ucschar ch);

LIBHANGUL_API
const ucschar* hangul_syllable_iterator_prev(const ucschar* str,
					     const ucschar* begin);
LIBHANGUL_API
const ucschar* hangul_syllable_iterator_next(const ucschar* str,
					     const ucschar* end);

LIBHANGUL_API
int     hangul_syllable_len(const ucschar* str, int max_len);

LIBHANGUL_API
ucschar hangul_jamo_to_syllable(ucschar choseong,
				ucschar jungseong,
				ucschar jongseong);
LIBHANGUL_API
void    hangul_syllable_to_jamo(ucschar syllable,
				ucschar* choseong,
				ucschar* jungseong,
				ucschar* jongseong);
LIBHANGUL_API
int     hangul_jamos_to_syllables(ucschar* dest, int destlen,
				  const ucschar* src, int srclen);

/* hangulinputcontext.c */
typedef struct _HangulKeyboard        HangulKeyboard;
typedef struct _HangulCombination     HangulCombination;
typedef struct _HangulBuffer          HangulBuffer;
typedef struct _HangulInputContext    HangulInputContext;

enum {
    HANGUL_OUTPUT_SYLLABLE,
    HANGUL_OUTPUT_JAMO
};

enum {
    HANGUL_KEYBOARD_TYPE_JAMO,
    HANGUL_KEYBOARD_TYPE_JASO,
    HANGUL_KEYBOARD_TYPE_ROMAJA,
    HANGUL_KEYBOARD_TYPE_JAMO_YET,
    HANGUL_KEYBOARD_TYPE_JASO_YET,
};

enum {
    HANGUL_IC_OPTION_AUTO_REORDER,
    HANGUL_IC_OPTION_COMBI_ON_DOUBLE_STROKE,
    HANGUL_IC_OPTION_NON_CHOSEONG_COMBI,
};

/* library */
LIBHANGUL_API
int hangul_init();
LIBHANGUL_API
int hangul_fini();

/* keyboard */
LIBHANGUL_API
HangulKeyboard* hangul_keyboard_new(void);
LIBHANGUL_API
HangulKeyboard* hangul_keyboard_new_from_file(const char* path);
LIBHANGUL_API
void    hangul_keyboard_delete(HangulKeyboard *keyboard);
LIBHANGUL_API
void    hangul_keyboard_set_type(HangulKeyboard *keyboard, int type);

LIBHANGUL_API
unsigned int hangul_keyboard_list_get_count();
LIBHANGUL_API
const char* hangul_keyboard_list_get_keyboard_id(unsigned index_);
LIBHANGUL_API
const char* hangul_keyboard_list_get_keyboard_name(unsigned index_);
LIBHANGUL_API
const HangulKeyboard* hangul_keyboard_list_get_keyboard(const char* id);
LIBHANGUL_API
const char* hangul_keyboard_list_register_keyboard(HangulKeyboard* keyboard);
LIBHANGUL_API
HangulKeyboard* hangul_keyboard_list_unregister_keyboard(const char* id);

/* combination */
LIBHANGUL_API
HangulCombination* hangul_combination_new(void);
LIBHANGUL_API
void hangul_combination_delete(HangulCombination *combination);
LIBHANGUL_API
bool hangul_combination_set_data(HangulCombination* combination, 
		     ucschar* first, ucschar* second, ucschar* result, unsigned int n);

/* input context */
LIBHANGUL_API
HangulInputContext* hangul_ic_new(const char* keyboard);
LIBHANGUL_API
void hangul_ic_delete(HangulInputContext *hic);
LIBHANGUL_API
bool hangul_ic_process(HangulInputContext *hic, int ascii);
LIBHANGUL_API
void hangul_ic_reset(HangulInputContext *hic);
LIBHANGUL_API
bool hangul_ic_backspace(HangulInputContext *hic);

LIBHANGUL_API
bool hangul_ic_is_empty(HangulInputContext *hic);
LIBHANGUL_API
bool hangul_ic_has_choseong(HangulInputContext *hic);
LIBHANGUL_API
bool hangul_ic_has_jungseong(HangulInputContext *hic);
LIBHANGUL_API
bool hangul_ic_has_jongseong(HangulInputContext *hic);
LIBHANGUL_API
bool hangul_ic_is_transliteration(HangulInputContext *hic);

LIBHANGUL_API
bool hangul_ic_get_option(HangulInputContext *hic, int option);
LIBHANGUL_API
void hangul_ic_set_option(HangulInputContext *hic, int option, bool value);
LIBHANGUL_API
void hangul_ic_set_output_mode(HangulInputContext *hic, int mode);
LIBHANGUL_API
void hangul_ic_set_keyboard(HangulInputContext *hic,
			    const HangulKeyboard *keyboard);
LIBHANGUL_API
void hangul_ic_select_keyboard(HangulInputContext *hic,
			       const char* id);
LIBHANGUL_API
void hangul_ic_connect_callback(HangulInputContext* hic, const char* event,
				void* callback, void* user_data);

LIBHANGUL_API
const ucschar* hangul_ic_get_preedit_string(HangulInputContext *hic);
LIBHANGUL_API
const ucschar* hangul_ic_get_commit_string(HangulInputContext *hic);
LIBHANGUL_API
const ucschar* hangul_ic_flush(HangulInputContext *hic);

/* hanja.c */
typedef struct _Hanja Hanja;
typedef struct _HanjaList HanjaList;
typedef struct _HanjaTable HanjaTable;

LIBHANGUL_API
HanjaTable*  hanja_table_load(const char *filename);
LIBHANGUL_API
HanjaList*   hanja_table_match_exact(const HanjaTable* table, const char *key);
LIBHANGUL_API
HanjaList*   hanja_table_match_prefix(const HanjaTable* table, const char *key);
LIBHANGUL_API
HanjaList*   hanja_table_match_suffix(const HanjaTable* table, const char *key);
LIBHANGUL_API
void         hanja_table_delete(HanjaTable *table);

LIBHANGUL_API
int          hanja_list_get_size(const HanjaList *list);
LIBHANGUL_API
const char*  hanja_list_get_key(const HanjaList *list);
LIBHANGUL_API
const Hanja* hanja_list_get_nth(const HanjaList *list, unsigned int n);
LIBHANGUL_API
const char*  hanja_list_get_nth_key(const HanjaList *list, unsigned int n);
LIBHANGUL_API
const char*  hanja_list_get_nth_value(const HanjaList *list, unsigned int n);
LIBHANGUL_API
const char*  hanja_list_get_nth_comment(const HanjaList *list, unsigned int n);
LIBHANGUL_API
void         hanja_list_delete(HanjaList *list);

LIBHANGUL_API
const char*  hanja_get_key(const Hanja* hanja);
LIBHANGUL_API
const char*  hanja_get_value(const Hanja* hanja);
LIBHANGUL_API
const char*  hanja_get_comment(const Hanja* hanja);

#ifdef __cplusplus
}
#endif

void    hangul_keyboard_set_value(HangulKeyboard *keyboard,
	int key, ucschar value) LIBHANGUL_DEPRECATED;
void hangul_ic_set_combination(HangulInputContext *hic,
	const HangulCombination *combination) LIBHANGUL_DEPRECATED;

unsigned    hangul_ic_get_n_keyboards() LIBHANGUL_DEPRECATED;
const char* hangul_ic_get_keyboard_id(unsigned index_) LIBHANGUL_DEPRECATED;
const char* hangul_ic_get_keyboard_name(unsigned index_) LIBHANGUL_DEPRECATED;

#undef LIBHANGUL_EXPORT
#undef LIBHANGUL_IMPORT
#undef LIBHANGUL_EXTERN_SYMBOL
#undef LIBHANGUL_API
#undef LIBHANGUL_DEPRECATED

#endif /* libhangul_hangul_h */
