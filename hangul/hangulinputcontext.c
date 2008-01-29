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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <limits.h>

#include "hangul.h"
#include "hangulinternals.h"

#define HANGUL_KEYBOARD_TABLE_SIZE 0x80

typedef void   (*HangulOnTranslate)  (HangulInputContext*,
				      int,
				      ucschar*,
				      void*);
typedef bool   (*HangulOnTransition) (HangulInputContext*,
				      ucschar,
				      const ucschar*,
				      void*);

typedef struct _HangulCombinationItem HangulCombinationItem;

struct _HangulKeyboard {
    int type;
    ucschar* table;
};

struct _HangulCombinationItem {
    uint32_t key;
    ucschar code;
};

struct _HangulCombination {
    int size;
    HangulCombinationItem *table;
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

    const HangulKeyboard*    keyboard;
    const HangulCombination* combination;

    HangulBuffer buffer;
    int output_mode;

    ucschar preedit_string[64];
    ucschar commit_string[64];
    ucschar flushed_string[64];

    HangulOnTranslate   on_translate;
    void*               on_translate_data;

    HangulOnTransition  on_transition;
    void*               on_transition_data;

    HangulICFilter filter;
    void *filter_data;
};

#include "hangulkeyboard.h"

static const HangulKeyboard hangul_keyboard_2 = {
    HANGUL_KEYBOARD_TYPE_JAMO,
    (ucschar*)hangul_keyboard_table_2
};

static const HangulKeyboard hangul_keyboard_32 = {
    HANGUL_KEYBOARD_TYPE_JASO,
    (ucschar*)hangul_keyboard_table_32
};

static const HangulKeyboard hangul_keyboard_390 = {
    HANGUL_KEYBOARD_TYPE_JASO,
    (ucschar*)hangul_keyboard_table_390
};

static const HangulKeyboard hangul_keyboard_3final = {
    HANGUL_KEYBOARD_TYPE_JASO,
    (ucschar*)hangul_keyboard_table_3final
};

static const HangulKeyboard hangul_keyboard_3sun = {
    HANGUL_KEYBOARD_TYPE_JASO,
    (ucschar*)hangul_keyboard_table_3sun
};

static const HangulKeyboard hangul_keyboard_3yet = {
    HANGUL_KEYBOARD_TYPE_JASO,
    (ucschar*)hangul_keyboard_table_3yet
};

static const HangulCombination hangul_combination_default = {
    N_ELEMENTS(hangul_combination_table_default),
    (HangulCombinationItem*)hangul_combination_table_default
};

static const HangulCombination hangul_combination_full = {
    N_ELEMENTS(hangul_combination_table_full),
    (HangulCombinationItem*)hangul_combination_table_full
};

static void    hangul_buffer_push(HangulBuffer *buffer, ucschar ch);
static ucschar hangul_buffer_pop (HangulBuffer *buffer);
static ucschar hangul_buffer_peek(HangulBuffer *buffer);

static void    hangul_buffer_clear(HangulBuffer *buffer);
static int     hangul_buffer_get_string(HangulBuffer *buffer, ucschar*buf, int buflen);
static int     hangul_buffer_get_jamo_string(HangulBuffer *buffer, ucschar *buf, int buflen);

static void    hangul_ic_flush_internal(HangulInputContext *hic);

HangulKeyboard*
hangul_keyboard_new()
{
    HangulKeyboard *keyboard = malloc(sizeof(HangulKeyboard));
    if (keyboard != NULL) {
	keyboard->table = malloc(sizeof(ucschar) * HANGUL_KEYBOARD_TABLE_SIZE);
	if (keyboard->table != NULL) {
	    int i;
	    for (i = 0; i < HANGUL_KEYBOARD_TABLE_SIZE; i++)
		keyboard->table[i] = 0;

	    return keyboard;
	}
	free(keyboard);
    }

    return NULL;
}

static ucschar
hangul_keyboard_get_value(const HangulKeyboard *keyboard, int key)
{
    if (keyboard != NULL) {
	if (key >= 0 && key < HANGUL_KEYBOARD_TABLE_SIZE)
	    return keyboard->table[key];
    }

    return 0;
}

void
hangul_keyboard_set_value(HangulKeyboard *keyboard, int key, ucschar value)
{
    if (keyboard != NULL) {
	if (key >= 0 && key < N_ELEMENTS(keyboard->table))
	    keyboard->table[key] = value;
    }
}

static int
hangul_keyboard_get_type(const HangulKeyboard *keyboard)
{
    int type = 0;
    if (keyboard != NULL) {
	type = keyboard->type;
    }
    return type;
}

void
hangul_keyboard_set_type(HangulKeyboard *keyboard, int type)
{
    if (keyboard != NULL) {
	keyboard->type = type;
    }
}

void
hangul_keyboard_delete(HangulKeyboard *keyboard)
{
    if (keyboard != NULL)
	free(keyboard);
}

HangulCombination*
hangul_combination_new()
{
    HangulCombination *combination = malloc(sizeof(HangulCombination));
    if (combination != NULL) {
	combination->size = 0;
	combination->table = NULL;
	return combination;
    }

    return NULL;
}

void
hangul_combination_delete(HangulCombination *combination)
{
    if (combination != NULL) {
	if (combination->table != NULL)
	    free(combination->table);
	free(combination);
    }
}

static uint32_t
hangul_combination_make_key(ucschar first, ucschar second)
{
    return first << 16 | second;
}

bool
hangul_combination_set_data(HangulCombination* combination, 
			    ucschar* first, ucschar* second, ucschar* result,
			    unsigned int n)
{
    if (combination == NULL)
	return false;

    if (n == 0 || n > ULONG_MAX / sizeof(HangulCombinationItem))
	return false;

    combination->table = malloc(sizeof(HangulCombinationItem) * n);
    if (combination->table != NULL) {
	int i;

	combination->size = n;
	for (i = 0; i < n; i++) {
	    combination->table[i].key = hangul_combination_make_key(first[i], second[i]);
	    combination->table[i].code = result[i];
	}
	return true;
    }

    return false;
}

static int 
hangul_combination_cmp(const void* p1, const void* p2)
{
    const HangulCombinationItem *item1 = p1;
    const HangulCombinationItem *item2 = p2;
    return item1->key - item2->key;
}

ucschar
hangul_combination_combine(const HangulCombination* combination,
			   ucschar first, ucschar second)
{
    HangulCombinationItem *res;
    HangulCombinationItem key;

    if (combination == NULL)
	return 0;

    key.key = hangul_combination_make_key(first, second);
    res = bsearch(&key, combination->table, combination->size,
	          sizeof(combination->table[0]), hangul_combination_cmp);
    if (res != NULL)
	return res->code;

    return 0;
}

static bool
hangul_buffer_is_empty(HangulBuffer *buffer)
{
    return buffer->choseong == 0 && buffer->jungseong == 0 &&
	   buffer->jongseong == 0;
}

static bool
hangul_buffer_has_choseong(HangulBuffer *buffer)
{
    return buffer->choseong != 0;
}

static bool
hangul_buffer_has_jungseong(HangulBuffer *buffer)
{
    return buffer->jungseong != 0;
}

static bool
hangul_buffer_has_jongseong(HangulBuffer *buffer)
{
    return buffer->jongseong != 0;
}

static void
hangul_buffer_push(HangulBuffer *buffer, ucschar ch)
{
    if (hangul_is_choseong(ch)) {
	buffer->choseong = ch;
    } else if (hangul_is_jungseong(ch)) {
	buffer->jungseong = ch;
    } else if (hangul_is_jongseong(ch)) {
	buffer->jongseong = ch;
    } else {
    }

    buffer->stack[++buffer->index] = ch;
}

static ucschar
hangul_buffer_pop(HangulBuffer *buffer)
{
    return buffer->stack[buffer->index--];
}

static ucschar
hangul_buffer_peek(HangulBuffer *buffer)
{
    if (buffer->index < 0)
	return 0;

    return buffer->stack[buffer->index];
}

static void
hangul_buffer_clear(HangulBuffer *buffer)
{
    buffer->choseong = 0;
    buffer->jungseong = 0;
    buffer->jongseong = 0;

    buffer->index = -1;
    buffer->stack[0]  = 0;
    buffer->stack[1]  = 0;
    buffer->stack[2]  = 0;
    buffer->stack[3]  = 0;
    buffer->stack[4]  = 0;
    buffer->stack[5]  = 0;
    buffer->stack[6]  = 0;
    buffer->stack[7]  = 0;
    buffer->stack[8]  = 0;
    buffer->stack[9]  = 0;
    buffer->stack[10] = 0;
    buffer->stack[11] = 0;
}

static int
hangul_buffer_get_jamo_string(HangulBuffer *buffer, ucschar *buf, int buflen)
{
    int n = 0;

    if (buffer->choseong || buffer->jungseong || buffer->jongseong) {
	if (buffer->choseong) {
	    buf[n++] = buffer->choseong;
	} else {
	    buf[n++] = HANGUL_CHOSEONG_FILLER;
	}
	if (buffer->jungseong) {
	    buf[n++] = buffer->jungseong;
	} else {
	    buf[n++] = HANGUL_JUNGSEONG_FILLER;
	}
	if (buffer->jongseong) {
	    buf[n++] = buffer->jongseong;
	}
    }

    buf[n] = 0;

    return n;
}

static int
hangul_jaso_to_string(ucschar cho, ucschar jung, ucschar jong,
		      ucschar *buf, int len)
{
    ucschar ch = 0;
    int n = 0;

    if (cho) {
	if (jung) {
	    /* have cho, jung, jong or no jong */
	    ch = hangul_jaso_to_syllable(cho, jung, jong);
	    buf[n++] = ch;
	} else {
	    if (jong) {
		/* have cho, jong */
		ch = hangul_jaso_to_jamo(cho);
		buf[n++] = ch;
		ch = hangul_jaso_to_jamo(jong);
		buf[n++] = ch;
	    } else {
		/* have cho */
		ch = hangul_jaso_to_jamo(cho);
		buf[n++] = ch;
	    }
	}
    } else {
	if (jung) {
	    if (jong) {
		/* have jung, jong */
		ch = hangul_jaso_to_jamo(jung);
		buf[n++] = ch;
		ch = hangul_jaso_to_jamo(jong);
		buf[n++] = ch;
	    } else {
		/* have jung */
		ch = hangul_jaso_to_jamo(jung);
		buf[n++] = ch;
	    }
	} else {
	    if (jong) { 
		/* have jong */
		ch = hangul_jaso_to_jamo(jong);
		buf[n++] = ch;
	    } else {
		/* have nothing */
		buf[n] = 0;
	    }
	}
    }
    buf[n] = 0;

    return n;
}

static int
hangul_buffer_get_string(HangulBuffer *buffer, ucschar *buf, int buflen)
{
    return hangul_jaso_to_string(buffer->choseong,
				 buffer->jungseong,
				 buffer->jongseong,
				 buf, buflen);
}

static bool
hangul_buffer_backspace(HangulBuffer *buffer)
{
    if (buffer->index >= 0) {
	ucschar ch = hangul_buffer_pop(buffer);
	if (ch == 0)
	    return false;

	if (hangul_is_choseong(ch)) {
	    ch = hangul_buffer_peek(buffer);
	    buffer->choseong = hangul_is_choseong(ch) ? ch : 0;
	    return true;
	} else if (hangul_is_jungseong(ch)) {
	    ch = hangul_buffer_peek(buffer);
	    buffer->jungseong = hangul_is_jungseong(ch) ? ch : 0;
	    return true;
	} else if (hangul_is_jongseong(ch)) {
	    ch = hangul_buffer_peek(buffer);
	    buffer->jongseong = hangul_is_jongseong(ch) ? ch : 0;
	    return true;
	}
    }
    return false;
}

static inline bool
hangul_ic_push(HangulInputContext *hic, ucschar c)
{
    ucschar buf[64] = { 0, };
    if (hic->on_transition != NULL) {
	ucschar cho, jung, jong;
	if (hangul_is_choseong(c)) {
	    cho  = c;
	    jung = hic->buffer.jungseong;
	    jong = hic->buffer.jongseong;
	} else if (hangul_is_jungseong(c)) {
	    cho  = hic->buffer.choseong;
	    jung = c;
	    jong = hic->buffer.jongseong;
	} else if (hangul_is_jongseong(c)) {
	    cho  = hic->buffer.choseong;
	    jung = hic->buffer.jungseong;
	    jong = c;
	} else {
	    hangul_ic_flush_internal(hic);
	    return false;
	}

	hangul_jaso_to_string(cho, jung, jong, buf, N_ELEMENTS(buf));
	if (!hic->on_transition(hic, c, buf, hic->on_transition_data)) {
	    hangul_ic_flush_internal(hic);
	    return false;
	}
    } else {
	if (!hangul_is_jaso(c)) {
	    hangul_ic_flush_internal(hic);
	    return false;
	}
    }

    hangul_buffer_push(&hic->buffer, c);
    return true;
}

static inline ucschar
hangul_ic_pop(HangulInputContext *hic)
{
    return hangul_buffer_pop(&hic->buffer);
}

static inline ucschar
hangul_ic_peek(HangulInputContext *hic)
{
    return hangul_buffer_peek(&hic->buffer);
}

static inline void
hangul_ic_save_preedit_string(HangulInputContext *hic)
{
    if (hic->output_mode == HANGUL_OUTPUT_JAMO) {
	hangul_buffer_get_jamo_string(&hic->buffer,
				      hic->preedit_string,
				      N_ELEMENTS(hic->preedit_string));
    } else {
	hangul_buffer_get_string(&hic->buffer,
				 hic->preedit_string,
				 N_ELEMENTS(hic->preedit_string));
    }
}

static inline void
hangul_ic_append_commit_string(HangulInputContext *hic, ucschar ch)
{
    int i;

    for (i = 0; i < N_ELEMENTS(hic->commit_string); i++) {
	if (hic->commit_string[i] == 0)
	    break;
    }

    if (i + 1 < N_ELEMENTS(hic->commit_string)) {
	hic->commit_string[i++] = ch;
	hic->commit_string[i] = 0;
    }
}

static inline void
hangul_ic_save_commit_string(HangulInputContext *hic)
{
    ucschar *string = hic->commit_string;
    int len = N_ELEMENTS(hic->commit_string);

    while (len > 0) {
	if (*string == 0)
	    break;
	len--;
	string++;
    }

    if (hic->output_mode == HANGUL_OUTPUT_JAMO) {
	hangul_buffer_get_jamo_string(&hic->buffer, string, len);
    } else {
	hangul_buffer_get_string(&hic->buffer, string, len);
    }

    hangul_buffer_clear(&hic->buffer);
}

static bool
hangul_ic_process_jamo(HangulInputContext *hic, ucschar ch)
{
    ucschar jong;
    ucschar combined;

    if (!hangul_is_jaso(ch) && ch > 0) {
	hangul_ic_save_commit_string(hic);
	hangul_ic_append_commit_string(hic, ch);
	return true;
    }

    if (hic->buffer.jongseong) {
	if (hangul_is_choseong(ch)) {
	    jong = hangul_choseong_to_jongseong(ch);
	    combined = hangul_combination_combine(hic->combination,
					      hic->buffer.jongseong, jong);
	    if (hangul_is_jongseong(combined)) {
		if (!hangul_ic_push(hic, combined)) {
		    if (!hangul_ic_push(hic, ch)) {
			return false;
		    }
		}
	    } else {
		hangul_ic_save_commit_string(hic);
		if (!hangul_ic_push(hic, ch)) {
		    return false;
		}
	    }
	} else if (hangul_is_jungseong(ch)) {
	    ucschar pop, peek;
	    pop = hangul_ic_pop(hic);
	    peek = hangul_ic_peek(hic);

	    if (hangul_is_jungseong(peek)) {
		hic->buffer.jongseong = 0;
		hangul_ic_save_commit_string(hic);
		hangul_ic_push(hic, hangul_jongseong_to_choseong(pop));
		if (!hangul_ic_push(hic, ch)) {
		    return false;
		}
	    } else {
		ucschar choseong = 0, jongseong = 0; 
		hangul_jongseong_dicompose(hic->buffer.jongseong,
					   &jongseong, &choseong);
		hic->buffer.jongseong = jongseong;
		hangul_ic_save_commit_string(hic);
		hangul_ic_push(hic, choseong);
		if (!hangul_ic_push(hic, ch)) {
		    return false;
		}
	    }
	} else {
	    goto flush;
	}
    } else if (hic->buffer.jungseong) {
	if (hangul_is_choseong(ch)) {
	    if (hic->buffer.choseong) {
		jong = hangul_choseong_to_jongseong(ch);
		if (hangul_is_jongseong(jong)) {
		    if (!hangul_ic_push(hic, jong)) {
			if (!hangul_ic_push(hic, ch)) {
			    return false;
			}
		    }
		} else {
		    hangul_ic_save_commit_string(hic);
		    if (!hangul_ic_push(hic, ch)) {
			return false;
		    }
		}
	    } else {
		if (!hangul_ic_push(hic, ch)) {
		    if (!hangul_ic_push(hic, ch)) {
			return false;
		    }
		}
	    }
	} else if (hangul_is_jungseong(ch)) {
	    combined = hangul_combination_combine(hic->combination,
						  hic->buffer.jungseong, ch);
	    if (hangul_is_jungseong(combined)) {
		if (!hangul_ic_push(hic, combined)) {
		    return false;
		}
	    } else {
		hangul_ic_save_commit_string(hic);
		if (!hangul_ic_push(hic, ch)) {
		    return false;
		}
	    }
	} else {
	    goto flush;
	}
    } else if (hic->buffer.choseong) {
	if (hangul_is_choseong(ch)) {
	    combined = hangul_combination_combine(hic->combination,
						  hic->buffer.choseong, ch);
	    if (!hangul_ic_push(hic, combined)) {
		if (!hangul_ic_push(hic, ch)) {
		    return false;
		}
	    }
	} else {
	    if (!hangul_ic_push(hic, ch)) {
		if (!hangul_ic_push(hic, ch)) {
		    return false;
		}
	    }
	}
    } else {
	if (!hangul_ic_push(hic, ch)) {
	    return false;
	}
    }

    hangul_ic_save_preedit_string(hic);
    return true;

flush:
    hangul_ic_flush_internal(hic);
    return false;
}

static bool
hangul_ic_process_jaso(HangulInputContext *hic, ucschar ch)
{
    if (hangul_is_choseong(ch)) {
	if (hic->buffer.choseong == 0) {
	    if (!hangul_ic_push(hic, ch)) {
		if (!hangul_ic_push(hic, ch)) {
		    return false;
		}
	    }
	} else {
	    ucschar choseong = 0;
	    if (hangul_is_choseong(hangul_ic_peek(hic))) {
		choseong = hangul_combination_combine(hic->combination,
						  hic->buffer.choseong, ch);
	    }
	    if (choseong) {
		if (!hangul_ic_push(hic, choseong)) {
		    if (!hangul_ic_push(hic, choseong)) {
			return false;
		    }
		}
	    } else {
		hangul_ic_save_commit_string(hic);
		if (!hangul_ic_push(hic, ch)) {
		    return false;
		}
	    }
	}
    } else if (hangul_is_jungseong(ch)) {
	if (hic->buffer.jungseong == 0) {
	    if (!hangul_ic_push(hic, ch)) {
		if (!hangul_ic_push(hic, ch)) {
		    return false;
		}
	    }
	} else {
	    ucschar jungseong = 0;
	    if (hangul_is_jungseong(hangul_ic_peek(hic))) {
		jungseong = hangul_combination_combine(hic->combination,
						 hic->buffer.jungseong, ch);
	    }
	    if (jungseong) {
		if (!hangul_ic_push(hic, jungseong)) {
		    if (!hangul_ic_push(hic, jungseong)) {
			return false;
		    }
		}
	    } else {
		hangul_ic_save_commit_string(hic);
		if (!hangul_ic_push(hic, ch)) {
		    if (!hangul_ic_push(hic, ch)) {
			return false;
		    }
		}
	    }
	}
    } else if (hangul_is_jongseong(ch)) {
	if (hic->buffer.jongseong == 0) {
	    if (!hangul_ic_push(hic, ch)) {
		if (!hangul_ic_push(hic, ch)) {
		    return false;
		}
	    }
	} else {
	    ucschar jongseong = 0;
	    if (hangul_is_jongseong(hangul_ic_peek(hic))) {
		jongseong = hangul_combination_combine(hic->combination,
						   hic->buffer.jongseong, ch);
	    }
	    if (jongseong) {
		if (!hangul_ic_push(hic, jongseong)) {
		    if (!hangul_ic_push(hic, jongseong)) {
			return false;
		    }
		}
	    } else {
		hangul_ic_save_commit_string(hic);
		if (!hangul_ic_push(hic, ch)) {
		    if (!hangul_ic_push(hic, ch)) {
			return false;
		    }
		}
	    }
	}
    } else if (ch > 0) {
	hangul_ic_save_commit_string(hic);
	hangul_ic_append_commit_string(hic, ch);
    } else {
	hangul_ic_save_commit_string(hic);
	return false;
    }

    hangul_ic_save_preedit_string(hic);
    return true;
}

bool
hangul_ic_process(HangulInputContext *hic, int ascii)
{
    ucschar c;

    if (hic == NULL)
	return false;

    hic->preedit_string[0] = 0;
    hic->commit_string[0] = 0;

    c = hangul_keyboard_get_value(hic->keyboard, ascii);
    if (hic->on_translate != NULL)
	hic->on_translate(hic, ascii, &c, hic->on_translate_data);

    if (hangul_keyboard_get_type(hic->keyboard) == HANGUL_KEYBOARD_TYPE_JAMO)
	return hangul_ic_process_jamo(hic, c);
    else
	return hangul_ic_process_jaso(hic, c);
}

const ucschar*
hangul_ic_get_preedit_string(HangulInputContext *hic)
{
    if (hic == NULL)
	return NULL;

    return hic->preedit_string;
}

const ucschar*
hangul_ic_get_commit_string(HangulInputContext *hic)
{
    if (hic == NULL)
	return NULL;

    return hic->commit_string;
}

void
hangul_ic_reset(HangulInputContext *hic)
{
    if (hic == NULL)
	return;

    hic->preedit_string[0] = 0;
    hic->commit_string[0] = 0;
    hic->flushed_string[0] = 0;

    hangul_buffer_clear(&hic->buffer);
}

/* append current preedit to the commit buffer.
 * this function does not clear previously made commit string. */
static void
hangul_ic_flush_internal(HangulInputContext *hic)
{
    hic->preedit_string[0] = 0;

    hangul_ic_save_commit_string(hic);
    hangul_buffer_clear(&hic->buffer);
}

const ucschar*
hangul_ic_flush(HangulInputContext *hic)
{
    if (hic == NULL)
	return NULL;

    // get the remaining string and clear the buffer
    hic->preedit_string[0] = 0;
    hic->commit_string[0] = 0;
    hic->flushed_string[0] = 0;
    hangul_buffer_get_string(&hic->buffer, hic->flushed_string,
			     N_ELEMENTS(hic->flushed_string));

    hangul_buffer_clear(&hic->buffer);

    return hic->flushed_string;
}

bool
hangul_ic_backspace(HangulInputContext *hic)
{
    int ret;

    if (hic == NULL)
	return false;

    ret = hangul_buffer_backspace(&hic->buffer);
    if (ret)
	hangul_ic_save_preedit_string(hic);
    return ret;
}

int
hangul_ic_dvorak_to_qwerty(int qwerty)
{
    static const int table[] = {
	'!',	/* ! */
	'Q',	/* " */
	'#',	/* # */
	'$',	/* $ */
	'%',	/* % */
	'&',	/* & */
	'q',	/* ' */
	'(',	/* ( */
	')',	/* ) */
	'*',	/* * */
	'}',	/* + */
	'w',	/* , */
	'\'',	/* - */
	'e',	/* . */
	'[',	/* / */
	'0',	/* 0 */
	'1',	/* 1 */
	'2',	/* 2 */
	'3',	/* 3 */
	'4',	/* 4 */
	'5',	/* 5 */
	'6',	/* 6 */
	'7',	/* 7 */
	'8',	/* 8 */
	'9',	/* 9 */
	'Z',	/* : */
	'z',	/* ; */
	'W',	/* < */
	']',	/* = */
	'E',	/* > */
	'{',	/* ? */
	'@',	/* @ */
	'A',	/* A */
	'N',	/* B */
	'I',	/* C */
	'H',	/* D */
	'D',	/* E */
	'Y',	/* F */
	'U',	/* G */
	'J',	/* H */
	'G',	/* I */
	'C',	/* J */
	'V',	/* K */
	'P',	/* L */
	'M',	/* M */
	'L',	/* N */
	'S',	/* O */
	'R',	/* P */
	'X',	/* Q */
	'O',	/* R */
	':',	/* S */
	'K',	/* T */
	'F',	/* U */
	'>',	/* V */
	'<',	/* W */
	'B',	/* X */
	'T',	/* Y */
	'?',	/* Z */
	'-',	/* [ */
	'\\',	/* \ */
	'=',	/* ] */
	'^',	/* ^ */
	'"',	/* _ */
	'`',	/* ` */
	'a',	/* a */
	'n',	/* b */
	'i',	/* c */
	'h',	/* d */
	'd',	/* e */
	'y',	/* f */
	'u',	/* g */
	'j',	/* h */
	'g',	/* i */
	'c',	/* j */
	'v',	/* k */
	'p',	/* l */
	'm',	/* m */
	'l',	/* n */
	's',	/* o */
	'r',	/* p */
	'x',	/* q */
	'o',	/* r */
	';',	/* s */
	'k',	/* t */
	'f',	/* u */
	'.',	/* v */
	',',	/* w */
	'b',	/* x */
	't',	/* y */
	'/',	/* z */
	'_',	/* { */
	'|',	/* | */
	'+',	/* } */
	'~'	/* ~ */
    };

    if (qwerty >= '!' && qwerty <= '~')
	return table[qwerty - '!'];

    return qwerty;
}

bool
hangul_ic_is_empty(HangulInputContext *hic)
{
    return hangul_buffer_is_empty(&hic->buffer);
}

bool
hangul_ic_has_choseong(HangulInputContext *hic)
{
    return hangul_buffer_has_choseong(&hic->buffer);
}

bool
hangul_ic_has_jungseong(HangulInputContext *hic)
{
    return hangul_buffer_has_jungseong(&hic->buffer);
}

bool
hangul_ic_has_jongseong(HangulInputContext *hic)
{
    return hangul_buffer_has_jongseong(&hic->buffer);
}

void
hangul_ic_set_output_mode(HangulInputContext *hic, int mode)
{
    if (hic == NULL)
	return;

    hic->output_mode = mode;
}

void
hangul_ic_connect_translate (HangulInputContext* hic,
                             HangulOnTranslate callback,
                             void* user_data)
{
    if (hic != NULL) {
	hic->on_translate      = callback;
	hic->on_translate_data = user_data;
    }
}

void
hangul_ic_connect_transition(HangulInputContext* hic,
                             HangulOnTransition callback,
                             void* user_data)
{
    if (hic != NULL) {
	hic->on_transition      = callback;
	hic->on_transition_data = user_data;
    }
}

void hangul_ic_connect_callback(HangulInputContext* hic, const char* event,
				void* callback, void* user_data)
{
    if (hic == NULL || event == NULL)
	return;

    if (strcasecmp(event, "translate") == 0) {
	hic->on_translate      = (HangulOnTranslate)callback;
	hic->on_translate_data = user_data;
    } else if (strcasecmp(event, "transition") == 0) {
	hic->on_transition      = (HangulOnTransition)callback;
	hic->on_transition_data = user_data;
    }
}

void hangul_ic_set_filter(HangulInputContext *hic,
			  HangulICFilter func, void *user_data)
{
    return;
}

void
hangul_ic_set_keyboard(HangulInputContext *hic, const HangulKeyboard* keyboard)
{
    if (hic == NULL || keyboard == NULL)
	return;

    hic->keyboard = keyboard;
}

void
hangul_ic_select_keyboard(HangulInputContext *hic, const char* id)
{
    if (hic == NULL)
	return;

    if (id == NULL)
	id = "2";

    if (strcmp(id, "32") == 0) {
	hic->keyboard = &hangul_keyboard_32;
	hic->combination = &hangul_combination_default;
    } else if (strcmp(id, "39") == 0) {
	hic->keyboard = &hangul_keyboard_390;
	hic->combination = &hangul_combination_default;
    } else if (strcmp(id, "3f") == 0) {
	hic->keyboard = &hangul_keyboard_3final;
	hic->combination = &hangul_combination_default;
    } else if (strcmp(id, "3s") == 0) {
	hic->keyboard = &hangul_keyboard_3sun;
	hic->combination = &hangul_combination_default;
    } else if (strcmp(id, "3y") == 0) {
	hic->keyboard = &hangul_keyboard_3yet;
	hic->combination = &hangul_combination_full;
    } else {
	hic->keyboard = &hangul_keyboard_2;
	hic->combination = &hangul_combination_default;
    }
}

void
hangul_ic_set_combination(HangulInputContext *hic,
			  const HangulCombination* combination)
{
    if (hic == NULL || combination == NULL)
	return;

    hic->combination = combination;
}

HangulInputContext*
hangul_ic_new(const char* keyboard)
{
    HangulInputContext *hic;

    hic = malloc(sizeof(HangulInputContext));
    if (hic == NULL)
	return NULL;

    hangul_ic_set_output_mode(hic, HANGUL_OUTPUT_SYLLABLE);
    hangul_ic_select_keyboard(hic, keyboard);

    hangul_buffer_clear(&hic->buffer);

    hic->preedit_string[0] = 0;
    hic->commit_string[0] = 0;
    hic->flushed_string[0] = 0;

    hic->on_translate      = NULL;
    hic->on_translate_data = NULL;

    hic->on_transition      = NULL;
    hic->on_transition_data = NULL;

    return hic;
}

void
hangul_ic_delete(HangulInputContext *hic)
{
    if (hic == NULL)
	return;

    free(hic);
}
