#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#include "../hangul/hangul.h"

bool filter(ucschar *str, ucschar cho, ucschar jung, ucschar jong, void *data)
{
    //printf("Filter: %x %x %x\n", cho, jung, jong);
    //return jong == 0;
    return true;
}

int
main(int argc, char *argv[])
{
    int n;
    int ascii;
    int keyboard = HANGUL_KEYBOARD_2;
    char commit[32] = { '\0', };
    wchar_t *commit_string;
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
    hangul_ic_set_filter(hic, filter, NULL);

    for (ascii = getchar(); ascii != EOF; ascii = getchar()) {
	int ret = hangul_ic_process(hic, ascii);
	commit_string = (wchar_t*)hangul_ic_get_commit_string(hic);
	n = wcstombs(commit, commit_string, sizeof(commit));
	commit[n] = '\0';
	if (strlen(commit) > 0) {
	    printf("%s", commit);
	}
	if (!ret) {
	    printf("%c", ascii);
	}
    } 

    if (!hangul_ic_is_empty(hic)) {
	const wchar_t *flushed = (wchar_t*)hangul_ic_flush(hic);
	n = wcstombs(commit, flushed, sizeof(commit));
	commit[n] = '\0';
	if (strlen(commit) > 0) {
	    printf("%s", commit);
	}
    }

    hangul_ic_delete(hic);

    return 0;
}
