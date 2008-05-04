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

enum {
    HANGUL_ERROR_NOERROR,
    HANGUL_ERROR_INVALID,
    HANGUL_ERROR_RANGE,
    HANGUL_ERROR_CANTOPEN,
};

enum {
    HANJA_TABLE_TYPE_VECTOR,
    HANJA_TABLE_TYPE_MMAP
};

typedef struct _PtrVector        PtrVector;
typedef struct _HanjaKeyEntry    HanjaKeyEntry;

typedef struct _HanjaVectorTable HanjaVectorTable;
typedef struct _HanjaMMapTable   HanjaMMapTable;

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

typedef void (*HanjaTableDelete)(HanjaTable*);
typedef void (*HanjaTableMatch)(const HanjaTable*, const char*, HanjaList**);

struct _HanjaTable {
    int              type;
    HanjaTableDelete destroy;
    HanjaTableMatch  match;
};

struct _PtrVector {
    void** ptrs; 
    size_t len;
    size_t alloc;
};

struct _HanjaVectorTable {
    HanjaTable     parent;

    PtrVector*     keytable;
};

struct _HanjaKeyEntry {
    uint32_t hanja_offset;
    uint32_t nitems;
};

struct _HanjaMMapTable {
    HanjaTable     parent;

    HanjaKeyEntry* keytable;
    unsigned int   nkeys;
    unsigned int   ndata;

    void*          map;
    size_t         map_length;
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

enum {
    HANJA_STREAM_MEMORY,
    HANJA_STREAM_FILE
};

typedef struct {
    int            type;
    unsigned char* data;
    unsigned char* current;
    size_t         length;
} HanjaMemoryStream;

typedef struct {
    int                  type;
    FILE*                file;
} HanjaFileStream;

typedef union {
    int                  type;
    HanjaMemoryStream    memory;
    HanjaFileStream      file;
} HangulStream;

static void hanja_vector_table_delete(HanjaTable* hanja_table);
static void hanja_vector_table_match(const HanjaTable* hanja_table,
				     const char* key, HanjaList** list);

static void hanja_mmap_table_delete(HanjaTable* hanja_table);
static void hanja_mmap_table_match(const HanjaTable* hanja_table,
				   const char* key, HanjaList** list);

static inline int
hangul_stream_init_as_memory(HangulStream* stream, void* data, size_t length)
{
    stream->type           = HANJA_STREAM_MEMORY;
    stream->memory.data    = data;
    stream->memory.current = data;
    stream->memory.length  = length;
    return 0;
}

static inline int
hangul_stream_init_as_file(HangulStream* stream, FILE* file)
{
    stream->type      = HANJA_STREAM_FILE;
    stream->file.file = file;
    return 0;
}

static inline bool
hangul_stream_check_range(HanjaMemoryStream* stream, unsigned char* p)
{
    if (p >= stream->data && p < stream->data + stream->length)
	return true;
    else
	return false;
}

static inline int
hangul_stream_seek(HangulStream* stream, size_t offset)
{
    if (stream->type == HANJA_STREAM_MEMORY) {
	HanjaMemoryStream* mstream = &stream->memory;
	if (!hangul_stream_check_range(mstream, mstream->current + offset))
	    return HANGUL_ERROR_RANGE;
	    
	stream->memory.current += offset;
	return 0;
    }

    return HANGUL_ERROR_INVALID;
}

static inline int
hangul_stream_read_uint32(HangulStream* stream, uint32_t* value)
{
    if (stream->type == HANJA_STREAM_MEMORY) {
	HanjaMemoryStream* mstream = &stream->memory;
	if (!hangul_stream_check_range(mstream, mstream->current + sizeof(*value)))
	    return HANGUL_ERROR_RANGE;

	memcpy(value, mstream->current, sizeof(*value));
	mstream->current += sizeof(*value);
	return 0;
    }

    return HANGUL_ERROR_INVALID;
}

static inline int
hangul_stream_write(HangulStream* stream, const void* ptr, size_t len)
{
    if (stream->type == HANJA_STREAM_MEMORY) {
	HanjaMemoryStream* mstream = &stream->memory;
	if (!hangul_stream_check_range(mstream, mstream->current + len))
	    return HANGUL_ERROR_RANGE;

	memcpy(mstream->current, ptr, len);
	mstream->current += len;
    } else if (stream->type == HANJA_STREAM_FILE) {
	fwrite(ptr, len, 1, stream->file.file);
    }

    return 0;
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

#ifndef HAVE_MMAP

#define PROT_READ 0
#define MAP_SHARED 0
static void*
mmap(void *start, size_t length, int prot, int flags, int fd, size_t offset)
{
    start = malloc(length);
    if (start != NULL) {
	read(fd, start, length);
    }
    return start;
}

static int
munmap(void *start, size_t length)
{
    free(start);
}

#endif

static PtrVector*
ptr_vector_new(size_t initial_size)
{
    PtrVector* vector;

    if (initial_size == 0)
	initial_size = 2;

    if (initial_size > SIZE_MAX / sizeof(vector->ptrs[0]))
	return NULL;

    vector = malloc(sizeof(*vector));
    vector->len = 0;
    vector->alloc = initial_size;
    vector->ptrs = malloc(initial_size * sizeof(vector->ptrs[0]));;

    if (vector->ptrs == NULL) {
	free(vector);
	return NULL;
    }

    return vector;
}

static void
ptr_vector_delete(PtrVector* vector)
{
    if (vector != NULL) {
	free(vector->ptrs);
	free(vector);
    }
}

static inline size_t
ptr_vector_get_length(PtrVector* vector)
{
    return vector->len;
}

static void
ptr_vector_append(PtrVector* vector, void* data)
{
    if (vector->alloc > SIZE_MAX / sizeof(vector->ptrs[0]) / 2)
	return;

    if (vector->alloc < vector->len + 1) {
	size_t alloc = vector->alloc * 2;
	void** ptrs;

	ptrs = realloc(vector->ptrs, alloc * sizeof(vector->ptrs[0]));
	if (ptrs != NULL) {
	    vector->alloc = alloc;
	    vector->ptrs  = ptrs;
	}
    }

    if (vector->len + 1 <= vector->alloc) {
	vector->ptrs[vector->len] = data;
	vector->len++;
    }
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

static const Hanja*
hanja_keyentry_get_hanja(const HanjaKeyEntry* entry)
{
    const char* p = (const char*)entry;
    return (const Hanja*)(p + entry->hanja_offset);
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
hanja_list_append_nptrs(HanjaList* list, const Hanja** hanja, int n)
{
    hanja_list_reserve(list, n);

    if (list->alloc >= list->len + n) {
	unsigned int i;
	for (i = 0; i < n ; i++)
	    list->items[list->len + i] = hanja[i];
	list->len += n;
    }
}


static PtrVector*
hanja_vectors_from_txt(const char *filename)
{
    char *save_ptr = NULL;
    char *key;
    char *value;
    char *comment;
    char lastkey[64] = { 0, };
    char buf[1024];

    FILE *file;
    PtrVector* keys = NULL;
    PtrVector* data = NULL;

    if (filename == NULL)
	return NULL;

    file = fopen(filename, "r");
    if (file == NULL) {
	return NULL;
    }

    while (fgets(buf, sizeof(buf), file) != NULL) {
	Hanja* hanja;

	/* skip comments and empty lines */
	if (buf[0] == '#' || buf[0] == '\r' || buf[0] == '\n' || buf[0] == '\0')
	    continue;

	save_ptr = NULL;
	key = strtok_r(buf, ":", &save_ptr);
	value = strtok_r(NULL, ":", &save_ptr);
	comment = strtok_r(NULL, "\r\n", &save_ptr);

	if (key == NULL || strlen(key) == 0)
	    continue;

	if (value == NULL || strlen(value) == 0)
	    continue;

	if (comment == NULL)
	    comment = "";

	if (data != NULL && strcmp(key, lastkey) != 0) {
	    if (keys == NULL)
		keys = ptr_vector_new(32);

	    ptr_vector_append(keys, data);
	    strncpy(lastkey, key, sizeof(lastkey));
	    data = NULL;
	}

	hanja = hanja_new(key, value, comment);
	if (hanja != NULL) {
	    if (data == NULL)
		data = ptr_vector_new(1);
	    ptr_vector_append(data, hanja);
	}
    }

    if (data != NULL) {
	if (keys == NULL)
	    keys = ptr_vector_new(1);
	ptr_vector_append(keys, data);
    }

    fclose(file);

    return keys;
}

static void
hanja_vectors_delete(PtrVector* vectors)
{
    unsigned int i, j;
    for (i = 0; i < vectors->len; i++) {
	PtrVector* vector = vectors->ptrs[i];

	for (j = 0; j < vector->len; j++)
	    hanja_delete(vector->ptrs[j]);

	ptr_vector_delete(vector);
    }
    ptr_vector_delete(vectors);
}

static int
hanja_vectors_save(PtrVector* vectors, HangulStream* stream)
{
    unsigned int i, j, k;
    uint32_t nkeys;
    uint32_t ndata;
    uint32_t keytable_size;
    uint32_t datatable_size;
    uint32_t last_offset;

    /* signature */
    hangul_stream_write(stream, "HANJADB\x0", 8);

    nkeys = vectors->len;
    hangul_stream_write(stream, &nkeys, sizeof(nkeys));

    ndata = 0;
    for (i = 0; i < nkeys; i++)
	ndata += ptr_vector_get_length(vectors->ptrs[i]);
    hangul_stream_write(stream, &ndata, sizeof(ndata));

    keytable_size = nkeys * sizeof(HanjaKeyEntry);
    datatable_size = ndata * sizeof(Hanja);

    /* key table */
    last_offset = keytable_size;
    for (i = 0; i < nkeys; i++) {
	HanjaKeyEntry entry;

	entry.hanja_offset = last_offset - i * sizeof(entry);
	entry.nitems       = ptr_vector_get_length(vectors->ptrs[i]);

	hangul_stream_write(stream, &entry, sizeof(entry));

	last_offset += entry.nitems * sizeof(Hanja);
    }

    /* data table */
    last_offset = datatable_size;
    k = 0;
    for (i = 0; i < nkeys; i++) {
	PtrVector* items = vectors->ptrs[i];
	for (j = 0; j < items->len; j++) {
	    const char* key;
	    const char* value;
	    const char* comment;
	    size_t key_len;
	    size_t value_len;
	    size_t comment_len;
	    Hanja hanja;
	    Hanja* item = items->ptrs[j];

	    key     = hanja_get_key(item);
	    value   = hanja_get_value(item);
	    comment = hanja_get_comment(item);

	    hanja.key_offset     = last_offset - k * sizeof(hanja);
	    hanja.value_offset   = last_offset - k * sizeof(hanja);
	    hanja.comment_offset = last_offset - k * sizeof(hanja);

	    key_len     = strlen(key) + 1;
	    value_len   = strlen(value) + 1;
	    comment_len = strlen(comment) + 1;

	    hanja.value_offset   += key_len;
	    hanja.comment_offset += key_len + value_len;

	    hangul_stream_write(stream, &hanja, sizeof(hanja));

	    last_offset += key_len + value_len + comment_len;
	    k++;
	}
    }

    /* data */
    for (i = 0; i < nkeys; i++) {
	PtrVector* items = vectors->ptrs[i];
	for (j = 0; j < items->len; j++) {
	    size_t len;
	    const char* key;
	    const char* value;
	    const char* comment;
	    Hanja* hanja = items->ptrs[j];

	    key     = hanja_get_key(hanja);
	    value   = hanja_get_value(hanja);
	    comment = hanja_get_comment(hanja);

	    len = strlen(key) + 1;
	    hangul_stream_write(stream, key, len);

	    len = strlen(value) + 1;
	    hangul_stream_write(stream, value, len);

	    len = strlen(comment) + 1;
	    hangul_stream_write(stream, comment, len);
	}
    }

    return 0;
}

int
hanja_table_txt_to_bin(const char *txtfilename, const char* binfilename)
{
    PtrVector* vectors;

    vectors = hanja_vectors_from_txt(txtfilename);
    if (vectors != NULL) {
	FILE* file;
	HangulStream stream;

	file = fopen(binfilename, "w");
	if (file != NULL) {
	    hangul_stream_init_as_file(&stream, file);
	    hanja_vectors_save(vectors, &stream);
	    fclose(file);
	}

	hanja_vectors_delete(vectors);
    }

    return 0;
}

HanjaTable*
hanja_vector_table_load(PtrVector* vector)
{
    HanjaVectorTable* table;

    table = malloc(sizeof(*table));
    if (table != NULL) {
	table->parent.type = HANJA_TABLE_TYPE_VECTOR;
	table->parent.destroy = hanja_vector_table_delete;
	table->parent.match = hanja_vector_table_match;
	table->keytable = vector;
    }

    return (HanjaTable*)table;
}

static void
hanja_vector_table_delete(HanjaTable* hanja_table)
{
    if (hanja_table != NULL) {
	HanjaVectorTable* table = (HanjaVectorTable*)hanja_table;
	hanja_vectors_delete(table->keytable);
	free(table);
    }
}

static int
vector_table_cmp(const void* m1, const void* m2)
{
    const char*  key = m1;
    const PtrVector* vector = *(const void**)m2;
    const Hanja* hanja = vector->ptrs[0];
    const char*  hanja_key = hanja_get_key(hanja);

    return strcmp(key, hanja_key);
}

static void
hanja_vector_table_match(const HanjaTable* hanja_table,
		         const char* key, HanjaList** list)
{
    const HanjaVectorTable* table;
    const PtrVector** res;

    table = (const HanjaVectorTable*)hanja_table;
    res = bsearch(key, table->keytable->ptrs, table->keytable->len,
		  sizeof(PtrVector*), vector_table_cmp);
    if (res != NULL && *res != NULL) {
	const Hanja** hanja;

	if (*list == NULL)
	    *list = hanja_list_new(key);

	hanja = (const Hanja**)res[0]->ptrs;
	hanja_list_append_nptrs(*list, hanja, res[0]->len);
    }
}

HanjaMMapTable*
hanja_mmap_table_load(void* data, size_t length)
{
    unsigned int i, j;
    uint32_t nkeys = 0;
    uint32_t ndata = 0;
    HanjaKeyEntry*  keytable = NULL;
    HanjaMMapTable* table = NULL;
    int res = 0;
    const char* end;

    HangulStream stream;

    /* signature */
    if (memcmp("HANJADB\x0", data, 8) != 0)
	goto error;

    res = hangul_stream_init_as_memory(&stream, data, length);

    res = hangul_stream_seek(&stream, 8);

    res = hangul_stream_read_uint32(&stream, &nkeys);
    if (res != 0)
	goto error;

    res = hangul_stream_read_uint32(&stream, &ndata);
    if (res != 0)
	goto error;

    end = (const char*)data + length;

    keytable = (HanjaKeyEntry*)stream.memory.current;
    if ((const char*)keytable > end)
	goto error;

    /* check integrity here.
     * If the data file is wrong, the program may access the wrong address
     * and it will be killed by segmentation fault.
     * So we check it here, before to use. */
    for (i = 0; i < nkeys; i++) {
	const Hanja*         hanja;
	const HanjaKeyEntry* entry;

	entry = &keytable[i];
	if ((const char*)entry > end)
	    goto error;

	hanja = hanja_keyentry_get_hanja(entry);
	if ((const char*)hanja > end)
	    goto error;

	for (j = 0; j < entry->nitems; j++) {
	    const char* key     = hanja_get_key(hanja + j);
	    const char* value   = hanja_get_value(hanja + j);
	    const char* comment = hanja_get_comment(hanja + j);

	    if (key > end)
		goto error;

	    if (value > end)
		goto error;

	    if (comment > end)
		goto error;
	}
    }

    /* the last byte should be nul, or the last comment will be over the
     * boundary */
    end--;
    if (end[0] != '\0')
	goto error;

    table = malloc(sizeof(*table));
    if (table == NULL)
	goto error;

    table->parent.type = HANJA_TABLE_TYPE_MMAP;
    table->parent.destroy = hanja_mmap_table_delete;
    table->parent.match = hanja_mmap_table_match;
    table->keytable = keytable;
    table->nkeys = nkeys;
    table->ndata = ndata;
    table->map = data;
    table->map_length = length;

    return table;

error:
    return NULL;
}

static void
hanja_mmap_table_delete(HanjaTable* hanja_table)
{
    if (hanja_table != NULL) {
	HanjaMMapTable* table = (HanjaMMapTable*)hanja_table;
	if (table->map != NULL) {
	    munmap(table->map, table->map_length);
	}
	free(table);
    }
}

static int
mmap_table_cmp(const void* m1, const void* m2)
{
    const char*  key = m1;
    const Hanja* hanja = hanja_keyentry_get_hanja(m2);
    const char*  hanja_key = hanja_get_key(hanja);

    return strcmp(key, hanja_key);
}

static void
hanja_mmap_table_match(const HanjaTable* hanja_table,
		       const char* key, HanjaList** list)
{
    const HanjaKeyEntry* res;
    const HanjaMMapTable* table;

    table = (const HanjaMMapTable*)hanja_table;
    res = bsearch(key, table->keytable, table->nkeys,
		  sizeof(table->keytable[0]), mmap_table_cmp);
    if (res != NULL) {
	const Hanja* hanja = hanja_keyentry_get_hanja(res); 
	if (*list == NULL)
	    *list = hanja_list_new(key);
	hanja_list_append_n(*list, hanja, res->nitems);
    }
}

HanjaTable*
hanja_table_load_from_txt(const char *filename)
{
    PtrVector*  vectors;
    HanjaTable* table;

    vectors = hanja_vectors_from_txt(filename);
    if (vectors == NULL)
	return NULL;

    table = hanja_vector_table_load(vectors);
    if (table == NULL) {
	hanja_vectors_delete(vectors);
	return NULL;
    }

    return table;
}

HanjaTable*
hanja_table_load_from_bin(const char *filename)
{
    struct stat buf;
    FILE* file;
    void* data;
    size_t length;
    HanjaTable *table = NULL;

    file = fopen(filename, "r");
    if (file == NULL)
	return NULL;

    fstat(fileno(file), &buf);

    length = buf.st_size;
    data = mmap(0, length, PROT_READ, MAP_SHARED, fileno(file), 0);
    fclose(file);

    table = (HanjaTable*)hanja_mmap_table_load(data, length);
    if (table == NULL) {
	munmap(data, length);
	return NULL;
    }

    return table;
}

HanjaTable*
hanja_table_load(const char* filename)
{
    size_t len;
    HanjaTable* table = NULL;

    if (filename == NULL)
	filename = LIBHANGUL_DEFAULT_HANJA_DIC;

    len = strlen(filename);
    if (len > 4 &&
	filename[len - 1] == 't' &&
	filename[len - 2] == 'x' &&
	filename[len - 3] == 't' &&
	filename[len - 4] == '.') {
	table = hanja_table_load_from_txt(filename);
    }

    if (table == NULL)
	table = hanja_table_load_from_bin(filename);

    if (table == NULL)
	table = hanja_table_load_from_txt(filename);

    return table;
}

void
hanja_table_delete(HanjaTable *table)
{
    if (table != NULL) {
	table->destroy(table);
    }
}

HanjaList*
hanja_table_match_exact(const HanjaTable* table, const char *key)
{
    HanjaList* ret = NULL;

    if (key == NULL || key[0] == '\0')
	return NULL;

    if (table == NULL)
	return NULL;

    table->match(table, key, &ret);

    return ret;
}

HanjaList*
hanja_table_match_prefix(const HanjaTable* table, const char *key)
{
    char* p;
    char* newkey;
    HanjaList* ret = NULL;

    if (key == NULL || key[0] == '\0')
	return NULL;

    if (table == NULL)
	return NULL;

    newkey = strdup(key);
    if (newkey == NULL)
	return NULL;

    p = strchr(newkey, '\0');
    while (newkey[0] != '\0') {
	table->match(table, newkey, &ret);
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

    if (key == NULL || key[0] == '\0')
	return NULL;

    if (table == NULL)
	return NULL;

    p = key;
    while (p[0] != '\0') {
	table->match(table, p, &ret);
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

