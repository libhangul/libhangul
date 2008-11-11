/* libhangul
 * Copyright (C) 2005-2008 Choe Hwanjin
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef HAVE_MMAP
#include <sys/mman.h>
#endif

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hangul.h"
#include "hangulinternals.h"

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef struct _HanjaIndex     HanjaIndex;

typedef struct _HanjaPair      HanjaPair;
typedef struct _HanjaPairArray HanjaPairArray;

struct _Hanja {
    uint32_t key_offset;
    uint32_t value_offset;
    uint32_t comment_offset;
};

struct _HanjaList {
    char*         key;
    size_t        len;
    size_t        alloc;
    const Hanja** items; 
};

struct _HanjaIndex {
    unsigned offset;
    char     key[8];
};

struct _HanjaTable {
    HanjaIndex*    keytable;
    unsigned       nkeys;
    unsigned       key_size;
    FILE*          file;
};

struct _HanjaPair {
    ucschar first;
    ucschar second;
};

struct _HanjaPairArray {
    ucschar          key;
    const HanjaPair* pairs;
};

#include "hanjacompatible.h"

static const char utf8_skip_table[256] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
    3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,6,6,1,1
};

static inline int utf8_char_len(const char *p)
{
    return utf8_skip_table[*(const unsigned char*)p];
}

static inline const char* utf8_next(const char *str)
{
    int n = utf8_char_len(str);

    while (n > 0) {
	str++;
	if (*str == '\0')
	    return str;
	n--;
    }

    return str;
}

static inline char* utf8_prev(const char *str, const char *p)
{
    for (--p; p >= str; --p) {
	if ((*p & 0xc0) != 0x80)
	    break;
    }
    return (char*)p;
}

/* hanja searching functions */
static Hanja *
hanja_new(const char *key, const char *value, const char *comment)
{
    Hanja* hanja;
    size_t size;
    size_t keylen;
    size_t valuelen;
    size_t commentlen;
    char*  p;

    keylen = strlen(key) + 1;
    valuelen = strlen(value) + 1;
    if (comment != NULL)
	commentlen = strlen(comment) + 1;
    else
	commentlen = 1;

    size = sizeof(*hanja) + keylen + valuelen + commentlen;
    hanja = malloc(size);
    if (hanja == NULL)
	return NULL;

    p = (char*)hanja + sizeof(*hanja);
    strcpy(p, key);
    p += keylen;
    strcpy(p, value);
    p += valuelen;
    if (comment != NULL)
	strcpy(p, comment);
    else
	*p = '\0';
    p += valuelen;

    hanja->key_offset     = sizeof(*hanja);
    hanja->value_offset   = sizeof(*hanja) + keylen;
    hanja->comment_offset = sizeof(*hanja) + keylen + valuelen;

    return hanja;
}

static void
hanja_delete(Hanja* hanja)
{
    free(hanja);
}

const char*
hanja_get_key(const Hanja* hanja)
{
    if (hanja != NULL) {
	const char* p  = (const char*)hanja;
	return p + hanja->key_offset;
    }
    return NULL;
}

const char*
hanja_get_value(const Hanja* hanja)
{
    if (hanja != NULL) {
	const char* p  = (const char*)hanja;
	return p + hanja->value_offset;
    }
    return NULL;
}

const char*
hanja_get_comment(const Hanja* hanja)
{
    if (hanja != NULL) {
	const char* p  = (const char*)hanja;
	return p + hanja->comment_offset;
    }
    return NULL;
}

static HanjaList *
hanja_list_new(const char *key)
{
    HanjaList *list;

    list = malloc(sizeof(*list));
    if (list != NULL) {
	list->key = strdup(key);
	list->len = 0;
	list->alloc = 1;
	list->items = malloc(list->alloc * sizeof(list->items[0]));
	if (list->items == NULL) {
	    free(list);
	    list = NULL;
	}
    }

    return list;
}

static void
hanja_list_reserve(HanjaList* list, size_t n)
{
    size_t size = list->alloc;

    if (n > SIZE_MAX / sizeof(list->items[0]) - list->len)
	return;

    while (size < list->len + n)
	size *= 2;

    if (size > SIZE_MAX / sizeof(list->items[0]))
	return;

    if (list->alloc < list->len + n) {
	const Hanja** data;

	data = realloc(list->items, size * sizeof(list->items[0]));
	if (data != NULL) {
	    list->alloc = size;
	    list->items = data;
	}
    }
}

static void
hanja_list_append_n(HanjaList* list, const Hanja* hanja, int n)
{
    hanja_list_reserve(list, n);

    if (list->alloc >= list->len + n) {
	unsigned int i;
	for (i = 0; i < n ; i++)
	    list->items[list->len + i] = hanja + i;
	list->len += n;
    }
}

static void
hanja_table_match(const HanjaTable* table,
		  const char* key, HanjaList** list)
{
    int low, high, mid;
    int res = -1;

    low = 0;
    high = table->nkeys - 1;

    while (low < high) {
	mid = (low + high) / 2;
	res = strncmp(table->keytable[mid].key, key, table->key_size);
	if (res < 0) {
	    low = mid + 1;
	} else if (res > 0) {
	    high = mid - 1;
	} else {
	    break;
	}
    }

    if (res != 0) {
	mid = low;
	res = strncmp(table->keytable[mid].key, key, table->key_size);
    }

    if (res == 0) {
	unsigned offset;
	char buf[512];

	offset = table->keytable[mid].offset;
	fseek(table->file, offset, SEEK_SET);

	while (fgets(buf, sizeof(buf), table->file) != NULL) {
	    char* save = NULL;
	    char* p = strtok_r(buf, ":", &save);
	    res = strcmp(p, key);
	    if (res == 0) {
		char* value   = strtok_r(NULL, ":", &save);
		char* comment = strtok_r(NULL, "\r\n", &save);

		Hanja* hanja = hanja_new(p, value, comment);

		if (*list == NULL) {
		    *list = hanja_list_new(key);
		}

		hanja_list_append_n(*list, hanja, 1);
	    } else if (res > 0) {
		break;
	    }
	}
    }
}

HanjaTable*
hanja_table_load(const char* filename)
{
    unsigned nkeys;
    char buf[512];
    int key_size = 5;
    char last_key[8] = { '\0', };
    char* save_ptr = NULL;
    char* key;
    long offset;
    unsigned i;
    FILE* file;
    HanjaIndex* keytable;
    HanjaTable* table;

    if (filename == NULL)
	filename = LIBHANGUL_DEFAULT_HANJA_DIC;

    file = fopen(filename, "r");
    if (file == NULL) {
	return NULL;
    }

    nkeys = 0;
    while (fgets(buf, sizeof(buf), file) != NULL) {
	/* skip comments and empty lines */
	if (buf[0] == '#' || buf[0] == '\r' || buf[0] == '\n' || buf[0] == '\0')
	    continue;

	save_ptr = NULL;
	key = strtok_r(buf, ":", &save_ptr);

	if (key == NULL || strlen(key) == 0)
	    continue;

	if (strncmp(last_key, key, key_size) != 0) {
	    nkeys++;
	    strncpy(last_key, key, key_size);
	}
    }

    rewind(file);
    keytable = malloc(nkeys * sizeof(keytable[0]));
    memset(keytable, 0, nkeys * sizeof(keytable[0]));

    i = 0;
    offset = ftell(file);
    while (fgets(buf, sizeof(buf), file) != NULL) {
	/* skip comments and empty lines */
	if (buf[0] == '#' || buf[0] == '\r' || buf[0] == '\n' || buf[0] == '\0')
	    continue;

	save_ptr = NULL;
	key = strtok_r(buf, ":", &save_ptr);

	if (key == NULL || strlen(key) == 0)
	    continue;

	if (strncmp(last_key, key, key_size) != 0) {
	    keytable[i].offset = offset;
	    strncpy(keytable[i].key, key, key_size);
	    strncpy(last_key, key, key_size);
	    i++;
	}
	offset = ftell(file);
    }

    table = malloc(sizeof(*table));
    if (table == NULL) {
	free(keytable);
	fclose(file);
	return NULL;
    }

    table->keytable = keytable;
    table->nkeys = nkeys;
    table->key_size = key_size;
    table->file = file;

    return table;
}

void
hanja_table_delete(HanjaTable *table)
{
    if (table != NULL) {
	free(table->keytable);
	fclose(table->file);
	free(table);
    }
}

HanjaList*
hanja_table_match_exact(const HanjaTable* table, const char *key)
{
    HanjaList* ret = NULL;

    if (key == NULL || key[0] == '\0' || table == NULL)
	return NULL;

    hanja_table_match(table, key, &ret);

    return ret;
}

HanjaList*
hanja_table_match_prefix(const HanjaTable* table, const char *key)
{
    char* p;
    char* newkey;
    HanjaList* ret = NULL;

    if (key == NULL || key[0] == '\0' || table == NULL)
	return NULL;

    newkey = strdup(key);
    if (newkey == NULL)
	return NULL;

    p = strchr(newkey, '\0');
    while (newkey[0] != '\0') {
	hanja_table_match(table, newkey, &ret);
	p = utf8_prev(newkey, p);
	p[0] = '\0';
    }
    free(newkey);

    return ret;
}

HanjaList*
hanja_table_match_suffix(const HanjaTable* table, const char *key)
{
    const char* p;
    HanjaList* ret = NULL;

    if (key == NULL || key[0] == '\0' || table == NULL)
	return NULL;

    p = key;
    while (p[0] != '\0') {
	hanja_table_match(table, p, &ret);
	p = utf8_next(p);
    }

    return ret;
}

int
hanja_list_get_size(const HanjaList *list)
{
    if (list != NULL)
	return list->len;
    return 0;
}

const char*
hanja_list_get_key(const HanjaList *list)
{
    if (list != NULL)
	return list->key;
    return NULL;
}

const Hanja*
hanja_list_get_nth(const HanjaList *list, unsigned int n)
{
    if (list != NULL) {
	if (n < list->len)
	    return list->items[n];
    }
    return NULL;
}

const char*
hanja_list_get_nth_key(const HanjaList *list, unsigned int n)
{
    const Hanja* hanja = hanja_list_get_nth(list, n);
    return hanja_get_key(hanja);
}

const char*
hanja_list_get_nth_value(const HanjaList *list, unsigned int n)
{
    const Hanja* hanja = hanja_list_get_nth(list, n);
    return hanja_get_value(hanja);
}

const char*
hanja_list_get_nth_comment(const HanjaList *list, unsigned int n)
{
    const Hanja* hanja = hanja_list_get_nth(list, n);
    return hanja_get_comment(hanja);
}

void
hanja_list_delete(HanjaList *list)
{
    if (list) {
	size_t i;
	for (i = 0; i < list->len; i++) {
	    hanja_delete((Hanja*)list->items[i]);
	}
	free(list->items);
	free(list->key);
	free(list);
    }
}

static int
compare_pair(const void* a, const void* b)
{
    const ucschar*   c = a;
    const HanjaPair* y = b;

    return *c - y->first;
}

size_t
hanja_compatibility_form(ucschar* hanja, const ucschar* hangul, size_t n)
{
    size_t i;
    size_t nconverted;

    if (hangul == NULL || hanja == NULL)
	return 0;

    nconverted = 0;
    for (i = 0; i < n && hangul[i] != 0 && hanja[i] != 0; i++) {
	HanjaPairArray* p;

	p = bsearch(&hanja[i],
		    hanja_unified_to_compat_table,
		    N_ELEMENTS(hanja_unified_to_compat_table),
		    sizeof(hanja_unified_to_compat_table[0]),
		    compare_pair);
	if (p != NULL) {
	    const HanjaPair* pair = p->pairs;
	    while (pair->first != 0) {
		if (pair->first == hangul[i]) {
		    hanja[i] = pair->second;
		    nconverted++;
		    break;
		}
		pair++;
	    }
	}
    }

    return nconverted;
}

size_t
hanja_unified_form(ucschar* str, size_t n)
{
    size_t i;
    size_t nconverted;

    if (str == NULL)
	return 0;

    nconverted = 0;
    for (i = 0; i < n && str[i] != 0; i++) {
	if (str[i] >= 0xF900 && str[i] <= 0xFA0B) {
	    str[i] = hanja_compat_to_unified_table[str[i] - 0xF900];
	    nconverted++;
	}
    }

    return nconverted;
}

