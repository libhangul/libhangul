#include <stdio.h>

#include "../hangul/hangul.h"

int
main(int argc, char *argv[])
{
    HanjaTable *table = hanja_table_load(NULL);

    HanjaList *list = hanja_table_match_prefix(table, argv[1]);

    int i, n;
    n = hanja_list_get_size(list);
    for (i = 0; i < n; i++) {
	const char* value = hanja_list_get_nth_value(list, i);
	printf("%s\n", value);
    }

    hanja_list_destroy(list);

    hanja_table_destroy(table);

    return 0;
}
