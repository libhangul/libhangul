#include <stdio.h>
#include <string.h>

#include "../hangul/hangul.h"

int
main(int argc, char *argv[])
{
    char* hanja_table_file = NULL;
    char buf[256] = { '\0', };

    if (argc > 1)
	hanja_table_file = argv[1];

    HanjaTable *table = hanja_table_load(hanja_table_file);
    
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
	char* p = strchr(buf, '\n');
	if (p != NULL)
	    *p = '\0';

	HanjaList *list = hanja_table_match_prefix(table, buf);

	int i, n;
	n = hanja_list_get_size(list);
	for (i = 0; i < n; i++) {
	    const char* value = hanja_list_get_nth_value(list, i);
	    printf("%s\n", value);
	}

	hanja_list_delete(list);
    }

    hanja_table_delete(table);

    return 0;
}
