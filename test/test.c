#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#include "../hangul/hangul.h"

int
main(int argc, char *argv[])
{
    int n;
    int ascii;
    int keyboard = HANGUL_KEYBOARD_2;
    char commit[32] = { '\0', };
    HangulInputContext *hic;

    if (argc > 1) {
	keyboard = atoi(argv[1]);
    }

    setlocale(LC_CTYPE, "");

    hic = hangul_ic_new(keyboard);
    if (hic == NULL) {
	printf("hic is null\n");
	return -1;
    }

    for (ascii = getchar(); ascii != EOF; ascii = getchar()) {
	int ret = hangul_ic_filter(hic, ascii, 0);
	n = wcstombs(commit, hangul_ic_get_commit_string(hic), sizeof(commit));
	commit[n] = '\0';
	if (strlen(commit) > 0) {
	    printf("%s", commit);
	}
	if (!ret) {
	    printf("%c", ascii);
	}
    } 
    hangul_ic_reset(hic);
    n = wcstombs(commit, hangul_ic_get_commit_string(hic), sizeof(commit));
    commit[n] = '\0';
    if (strlen(commit) > 0) {
	printf("%s", commit);
    }

    hangul_ic_delete(hic);

    return 0;
}
