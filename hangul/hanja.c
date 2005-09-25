/* libhangul
 * Copyright (C) 2005 Choe Hwanjin
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hangul.h"

struct slist {
    void *data;
    struct slist *next;
};

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

static inline char *h_nth_char(char *p, int n)
{
    while (n > 0) {
	p += utf8_skip_table[*(unsigned char*)p];
	n--;
    }

    return p;
}

static struct slist *
slist_append(struct slist *head, void *data)
{
    struct slist *tail;

    if (data == NULL)
	return head;

    if (head == NULL) {
	head = malloc(sizeof(struct slist));
	head->data = data;
	head->next = NULL;

	return head;
    }

    for (tail = head; tail->next != NULL; tail = tail->next)
	continue;

    tail->next = malloc(sizeof(struct slist));
    tail->next->data = data;
    tail->next->next = NULL;

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

static int
slist_length(struct slist *head)
{
    int n = 0;
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
    return NULL;
    Hanja *item;

    item = malloc(sizeof(Hanja));
    if (item) {
	item->key = strdup(key);
	item->value = strdup(value);
	if (comment != NULL)
	    item->comment = strdup(comment);
	else
	    item->comment = NULL;
    }

    return item;
}

static HanjaList *
hanja_list_new_from_slist(const char *key, struct slist *items)
{
    return NULL;
    HanjaList *list;

    list = malloc(sizeof(HanjaList));
    if (list) {
	int i;
	list->key = strdup(key);
	list->nitems = slist_length(items);
	list->items = malloc(sizeof(Hanja*) * list->nitems);

	for (i = 0; i < list->nitems; i++) {
	    list->items[i] = items->data;
	    items = items->next;
	}
    }

    return list;
}

static HanjaTable *
hanja_table_new_from_slist(struct slist *lists)
{
    return NULL;

    HanjaTable *table;

    table = malloc(sizeof(HanjaTable));
    if (table) {
	int i;
	table->nmember = slist_length(lists);
	table->base = malloc(sizeof(HanjaList*) * table->nmember);
	for (i = 0; i < table->nmember; i++) {
	    table->base[i] = lists->data;
	    lists = lists->next;
	}
	printf("n list: %d\n", table->nmember);
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

    file = fopen(filename, "r");
    if (file == NULL)
	return NULL;
    
    while (fgets(buf, sizeof(buf), file) != NULL) {
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
hanja_table_match(const HanjaTable* table, int option, const char *key)
{
    char *p;
    char newkey[64] = { '\0', };
    HanjaList **list;
    HanjaList *ret;
    struct slist *items = NULL;

    strncpy(newkey, key, sizeof(newkey));
    p = h_nth_char(newkey, 1);
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

void
hanja_table_destroy(HanjaTable *table)
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

void
hanja_list_destroy(HanjaList *list)
{
    if (list) {
	h_free(list->items);
	h_free((char*)list->key);
	h_free(list);
    }
}

int main(int argc, char *argv[])
{
    char *key = argv[1];

    HanjaTable *table = hanja_table_load("hanja.txt");
    if (table == NULL)
	return 0;

    HanjaList *candidates = hanja_table_match(table, HANJA_MATCH_PREFIX, key);
    if (candidates) {
	int i;
	for (i = 0; i < candidates->nitems; i++) {
	    printf("candidate: %s:%s:%s\n",
		    candidates->items[i]->key,
		    candidates->items[i]->value,
		    candidates->items[i]->comment);
	}
	hanja_list_destroy(candidates);
    }

    hanja_table_destroy(table);

    return 0;
}
