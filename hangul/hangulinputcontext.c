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
#include <ctype.h>

#include "hangul.h"
#include "hangulkeyboard.h"

#define N_ELEMENTS(array) (sizeof(array) / sizeof(array[0]))

static void    hangul_buffer_push(HangulBuffer *buffer, ucschar ch);
static ucschar hangul_buffer_pop (HangulBuffer *buffer);
static ucschar hangul_buffer_peek(HangulBuffer *buffer);

static void    hangul_buffer_clear(HangulBuffer *buffer);
static int     hangul_buffer_get_string(HangulBuffer *buffer, ucschar*buf, int buflen);
static int     hangul_buffer_get_jamo_string(HangulBuffer *buffer, ucschar *buf, int buflen);

static void    hangul_ic_flush_internal(HangulInputContext *hic);
static ucschar hangul_ic_translate_jamo(HangulInputContext *hic, int ascii);
static ucschar hangul_ic_combine_jamo(HangulInputContext *hic, ucschar	first, ucschar second);

static bool
hangul_buffer_is_empty(HangulBuffer *buffer)
{
    return buffer->choseong == 0 && buffer->jungseong == 0 &&
	   buffer->jongseong == 0;
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

    buf[n] = L'\0';

    return n;
}

static int
hangul_jaso_to_string(ucschar cho, ucschar jung, ucschar jong,
		      ucschar *buf, int len)
{
    ucschar ch = L'\0';
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
		buf[n] = L'\0';
	    }
	}
    }
    buf[n] = L'\0';

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

static ucschar
hangul_ic_translate_jamo(HangulInputContext *hic, int ascii)
{
    ucschar ch = 0;

    if (ascii >= '!' && ascii <= '~') {
	ch = hic->keyboard_table[ascii - '!'];
    }

    return ch;
}

static ucschar
hangul_ic_combine_jamo(HangulInputContext *hic, ucschar first, ucschar second)
{
    int min, max, mid;
    uint32_t key;

    /* make key */
    key = first << 16 | second;

    /* binary search in table */
    min = 0;
    max = hic->combination_table_size - 1;

    while (max >= min) {
	mid = (min + max) / 2;
	if (hic->combination_table[mid].key < key)
	    min = mid + 1;
	else if (hic->combination_table[mid].key > key)
	    max = mid - 1;
	else
	    return hic->combination_table[mid].code;
    }
    return 0;
}

static inline bool
hangul_ic_push(HangulInputContext *hic, ucschar ch)
{
    ucschar buf[64] = { 0, };
    if (hic->filter != NULL) {
	ucschar cho, jung, jong;
	if (hangul_is_choseong(ch)) {
	    cho  = ch;
	    jung = hic->buffer.jungseong;
	    jong = hic->buffer.jongseong;
	} else if (hangul_is_jungseong(ch)) {
	    cho  = hic->buffer.choseong;
	    jung = ch;
	    jong = hic->buffer.jongseong;
	} else if (hangul_is_jongseong(ch)) {
	    cho  = hic->buffer.choseong;
	    jung = hic->buffer.jungseong;
	    jong = ch;
	} else {
	    hangul_ic_flush_internal(hic);
	    return false;
	}

	hangul_jaso_to_string(cho, jung, jong, buf, N_ELEMENTS(buf));
	if (!hic->filter(buf, cho, jung, jong, hic->filter_data)) {
	    hangul_ic_flush_internal(hic);
	    return false;
	}
    } else {
	if (!hangul_is_jaso(ch)) {
	    hangul_ic_flush_internal(hic);
	    return false;
	}
    }

    hangul_buffer_push(&hic->buffer, ch);
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

    if (hic->buffer.jongseong) {
	if (hangul_is_choseong(ch)) {
	    jong = hangul_choseong_to_jongseong(ch);
	    combined = hangul_ic_combine_jamo(hic,
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
	    combined = hangul_ic_combine_jamo(hic, hic->buffer.jungseong, ch);
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
	    combined = hangul_ic_combine_jamo(hic, hic->buffer.choseong, ch);
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
		choseong = hangul_ic_combine_jamo(hic,
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
		jungseong = hangul_ic_combine_jamo(hic,
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
		jongseong = hangul_ic_combine_jamo(hic,
						   hic->buffer.jongseong, ch);
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
    ucschar ch;

    if (hic == NULL)
	return false;

    ch = hangul_ic_translate_jamo(hic, ascii);

    hic->preedit_string[0] = 0;
    hic->commit_string[0] = 0;

    if (hic->type == HANGUL_INPUT_FILTER_JAMO)
	return hangul_ic_process_jamo(hic, ch);
    else
	return hangul_ic_process_jaso(hic, ch);
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

    hangul_buffer_clear(&hic->buffer);
}

/* this function does not clear previously made commit string */
static void
hangul_ic_flush_internal(HangulInputContext *hic)
{
    hic->preedit_string[0] = 0;

    hangul_ic_save_commit_string(hic);
    hangul_buffer_clear(&hic->buffer);
}

void
hangul_ic_flush(HangulInputContext *hic)
{
    if (hic == NULL)
	return;

    hic->commit_string[0] = 0;
    hangul_ic_flush_internal(hic);
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

bool
hangul_ic_is_empty(HangulInputContext *hic)
{
    return hangul_buffer_is_empty(&hic->buffer);
}

void
hangul_ic_set_output_mode(HangulInputContext *hic, int mode)
{
    if (hic == NULL)
	return;

    if (hic->output_mode != HANGUL_KEYBOARD_3YETGUL)
	hic->output_mode = mode;
}

void hangul_ic_set_filter(HangulInputContext *hic,
			  HangulICFilter func, void *user_data)
{
    if (hic == NULL)
	return;

    hic->filter = func;
    hic->filter_data = user_data;
}

void
hangul_ic_set_keyboard(HangulInputContext *hic, HangulKeyboardType keyboard)
{
    if (hic == NULL)
	return;

    switch (keyboard) {
    case HANGUL_KEYBOARD_2:
	hic->type = HANGUL_INPUT_FILTER_JAMO;
	hic->keyboard_table = hangul_keyboard_table_2;
	hic->combination_table = hangul_combination_table_default;
	hic->combination_table_size = N_ELEMENTS(hangul_combination_table_default);
	break;
    case HANGUL_KEYBOARD_32:
	hic->type = HANGUL_INPUT_FILTER_JASO;
	hic->keyboard_table = hangul_keyboard_table_32;
	hic->combination_table = hangul_combination_table_default;
	hic->combination_table_size = N_ELEMENTS(hangul_combination_table_default);
	break;
    case HANGUL_KEYBOARD_3FINAL:
	hic->type = HANGUL_INPUT_FILTER_JASO;
	hic->keyboard_table = hangul_keyboard_table_3final;
	hic->combination_table = hangul_combination_table_default;
	hic->combination_table_size = N_ELEMENTS(hangul_combination_table_default);
	break;
    case HANGUL_KEYBOARD_390:
	hic->type = HANGUL_INPUT_FILTER_JASO;
	hic->keyboard_table = hangul_keyboard_table_390;
	hic->combination_table = hangul_combination_table_default;
	hic->combination_table_size = N_ELEMENTS(hangul_combination_table_default);
	break;
    case HANGUL_KEYBOARD_3NOSHIFT:
	hic->type = HANGUL_INPUT_FILTER_JASO;
	hic->keyboard_table = hangul_keyboard_table_3sun;
	hic->combination_table = hangul_combination_table_default;
	hic->combination_table_size = N_ELEMENTS(hangul_combination_table_default);
	break;
    case HANGUL_KEYBOARD_3YETGUL:
	hic->type = HANGUL_INPUT_FILTER_JASO;
	hic->keyboard_table = hangul_keyboard_table_3yet;
	hic->combination_table = hangul_combination_table_full;
	hic->combination_table_size = N_ELEMENTS(hangul_combination_table_default);
	hic->output_mode = HANGUL_OUTPUT_JAMO;
	break;
    default:
	hic->type = HANGUL_INPUT_FILTER_JAMO;
	hic->keyboard_table = hangul_keyboard_table_2;
	hic->combination_table = hangul_combination_table_default;
	hic->combination_table_size = N_ELEMENTS(hangul_combination_table_default);
	break;
    }
}

HangulInputContext*
hangul_ic_new(HangulKeyboardType keyboard)
{
    HangulInputContext *hic;

    hic = malloc(sizeof(HangulInputContext));
    if (hic == NULL)
	return NULL;

    hangul_ic_set_output_mode(hic, HANGUL_OUTPUT_SYLLABLE);
    hangul_ic_set_keyboard(hic, keyboard);
    hangul_ic_set_filter(hic, NULL, NULL);

    hangul_buffer_clear(&hic->buffer);

    return hic;
}

void
hangul_ic_delete(HangulInputContext *hic)
{
    if (hic == NULL)
	return;

    free(hic);
}
