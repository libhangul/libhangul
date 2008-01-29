/* libhangul
 * Copyright (C) 2005 Choe Hwanjin
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

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hangul.h"
#include "hangulinternals.h"

typedef struct _HanjaPair      HanjaPair;
typedef struct _HanjaPairArray HanjaPairArray;

struct _Hanja {
    char *key;
    char *value;
    char *comment;
};

struct _HanjaList {
    char *key;
    unsigned int nitems;
    Hanja **items; 
};

struct _HanjaTable {
    unsigned int nmember;
    HanjaList **base;
};

struct slist {
    void *data;
    struct slist *next;
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

/* utility functions */
static inline void h_free(void *ptr)
{
    if (ptr)
	free(ptr);
}

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

static inline int h_char_len(const char *p)
{
    return utf8_skip_table[*(const unsigned char*)p];
}

static struct slist *
slist_append(struct slist *head, void *data)
{
    struct slist *tail;

    if (data == NULL)
	return head;

    if (head == NULL) {
	head = malloc(sizeof(struct slist));
	if (head != NULL) {
	    head->data = data;
	    head->next = NULL;
	}

	return head;
    }

    for (tail = head; tail->next != NULL; tail = tail->next)
	continue;

    tail->next = malloc(sizeof(struct slist));
    if (tail->next != NULL) {
	tail->next->data = data;
	tail->next->next = NULL;
    }

    return head;
}

static void
slist_delete(struct slist *head)
{
    struct slist *item;
    while (head != NULL) {
	item = head;
	head = head->next;
	free(item);
    }
}

static unsigned int
slist_length(struct slist *head)
{
    unsigned int n = 0;
    while (head != NULL) {
	head = head->next;
	n++;
    }
    return n;
}

/* hanja searching functions */
static Hanja *
hanja_new(const char *key, const char *value, const char *comment)
{
    Hanja *item;

    item = malloc(sizeof(Hanja));
    if (item != NULL) {
	item->key = strdup(key);
	item->value = strdup(value);
	if (comment != NULL)
	    item->comment = strdup(comment);
	else
	    item->comment = strdup("");
    }

    return item;
}

const char*
hanja_get_key(const Hanja* hanja)
{
    if (hanja != NULL)
	return hanja->key;
    return NULL;
}

const char*
hanja_get_value(const Hanja* hanja)
{
    if (hanja != NULL)
	return hanja->value;
    return NULL;
}

const char*
hanja_get_comment(const Hanja* hanja)
{
    if (hanja != NULL)
	return hanja->comment;
    return NULL;
}

static HanjaList *
hanja_list_new_from_slist(const char *key, struct slist *items)
{
    unsigned int nitems;
    HanjaList *list;

    nitems = slist_length(items);
    if (nitems > ULONG_MAX / sizeof(Hanja*))
	return NULL;

    list = malloc(sizeof(HanjaList));
    if (list != NULL) {
	int i;
	list->key = strdup(key);
	list->nitems = nitems;
	list->items = malloc(sizeof(Hanja*) * list->nitems);
	if (list->items != NULL) {
	    for (i = 0; i < list->nitems; i++) {
		list->items[i] = items->data;
		items = items->next;
	    }
	} else {
	    if (list->key != NULL)
		free(list->key);
	    free(list);
	    list = NULL;
	}
    }

    return list;
}

static HanjaTable *
hanja_table_new_from_slist(struct slist *lists)
{
    unsigned int nitems;
    HanjaTable *table;

    nitems = slist_length(lists);
    if (nitems > ULONG_MAX / sizeof(HanjaList*))
	return NULL;

    table = malloc(sizeof(HanjaTable));
    if (table) {
	int i;
	table->nmember = nitems;
	table->base = malloc(sizeof(HanjaList*) * table->nmember);
	if (table->base != NULL) {
	    for (i = 0; i < table->nmember; i++) {
		table->base[i] = lists->data;
		lists = lists->next;
	    }
	} else {
	    free(table);
	    table = NULL;
	}
    }
    return table;
}

HanjaTable*
hanja_table_load(const char *filename)
{
    char *save_ptr = NULL;
    char *key;
    char *value;
    char *comment;
    char listkey[64] = { 0, };
    char buf[1024];

    FILE *file;
    HanjaTable *table;
    HanjaList *list;
    Hanja *item;
    struct slist *items = NULL;
    struct slist *lists = NULL;

    if (filename == NULL)
	filename = LIBHANGUL_DEFAULT_HANJA_DIC;

    file = fopen(filename, "r");
    if (file == NULL) {
	printf("cant open file: %s\n", filename);
	return NULL;
    }
    
    while (fgets(buf, sizeof(buf), file) != NULL) {
	/* skip comments and empty lines */
	if (buf[0] == '#' || buf[0] == '\r' || buf[0] == '\n' || buf[0] == '\0')
	    continue;

	save_ptr = NULL;
	key = strtok_r(buf, ":", &save_ptr);
	value = strtok_r(NULL, ":", &save_ptr);
	comment = strtok_r(NULL, "\r\n", &save_ptr);

	if (strlen(listkey) == 0 ||
	    strncmp(listkey, key, strlen(listkey)) != 0) {
	    if (items != NULL) {
		list = hanja_list_new_from_slist(listkey, items);
		slist_delete(items);
		items = NULL;

		lists = slist_append(lists, list);
	    }

	    strncpy(listkey, key, sizeof(listkey));
	}
	
	item = hanja_new(key, value, comment);
	items = slist_append(items, item);
    }

    if (items != NULL) {
	list = hanja_list_new_from_slist(listkey, items);
	slist_delete(items);
	items = NULL;

	lists = slist_append(lists, list);
    }

    table = hanja_table_new_from_slist(lists);
    slist_delete(lists);
    lists = NULL;

    fclose(file);

    return table;
}

static void
delete_last_char(char *str)
{
    char *end = strchr(str, '\0');
    for (--end; end >= str; --end) {
	if ((*end & 0xc0) != 0x80) {
	    break;
	}
    }

    while (*end != '\0') {
	*end++ = '\0';
    }
}

static int
hanja_table_compare(const void *key, const void *item)
{
    return strncmp((const char*)key, (*((HanjaList**)item))->key, strlen(key));
}

HanjaList*
hanja_table_match_prefix(const HanjaTable* table, const char *key)
{
    char *p;
    char newkey[64] = { '\0', };
    HanjaList **list;
    HanjaList *ret;
    struct slist *items = NULL;

    strncpy(newkey, key, sizeof(newkey));
    p = newkey + h_char_len(newkey);
    *p = '\0';

    list = bsearch(newkey,
		    table->base, table->nmember,
		    sizeof(HanjaList*),
		    hanja_table_compare);
    if (list != NULL) {
	int i;
	strncpy(newkey, key, sizeof(newkey));
	for (; strlen(newkey) > 0; delete_last_char(newkey)) {
	    for (i = 0; i < (*list)->nitems; i++) {
		if (strcmp(newkey, (*list)->items[i]->key) == 0) {
		    items = slist_append(items, (*list)->items[i]);
		}
	    }
	}

	if (items) {
	    ret = hanja_list_new_from_slist(key, items);
	    slist_delete(items);
	    return ret;
	}
    }
    
    return NULL;
}

HanjaList*
hanja_table_match_suffix(const HanjaTable* table, const char *key)
{
    const char *p;
    char newkey[64] = { '\0', };
    HanjaList **list = NULL;
    HanjaList *ret;
    struct slist *items = NULL;

    p = key;
    strncpy(newkey, p, sizeof(newkey));
    newkey[h_char_len(newkey)] = '\0';
    while (strlen(newkey) > 0) {
	list = bsearch(newkey,
		       table->base, table->nmember,
		       sizeof(HanjaList*),
		       hanja_table_compare);

	if (list != NULL) {
	    int i;
	    for (i = 0; i < (*list)->nitems; i++) {
		if (strcmp(p, (*list)->items[i]->key) == 0) {
		    items = slist_append(items, (*list)->items[i]);
		}
	    }
	}

	p += h_char_len(p);
	strncpy(newkey, p, sizeof(newkey));
	newkey[h_char_len(newkey)] = '\0';
    }

    if (items != NULL) {
	ret = hanja_list_new_from_slist(key, items);
	slist_delete(items);
	return ret;
    }

    return NULL;
}

void
hanja_table_delete(HanjaTable *table)
{
    if (table) {
	int i, j;
	for (j = 0; j < table->nmember; j++) {
	    for (i = 0; i < table->base[j]->nitems; i++) {
		h_free((char*)table->base[j]->items[i]->key);
		h_free((char*)table->base[j]->items[i]->value);
		h_free((char*)table->base[j]->items[i]->comment);
		h_free(table->base[j]->items[i]);
	    }
	    h_free((char*)table->base[j]->key);
	    h_free(table->base[j]->items);
	    h_free(table->base[j]);
	}
	h_free(table->base);
	h_free(table);
    }
}

int
hanja_list_get_size(const HanjaList *list)
{
    if (list != NULL)
	return list->nitems;
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
	if (n < list->nitems)
	    return list->items[n];
    }
    return NULL;
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
	h_free(list->items);
	h_free((char*)list->key);
	h_free(list);
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

