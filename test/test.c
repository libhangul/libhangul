#include <stdarg.h>
#include <stdlib.h>
#include <check.h>

#include "../hangul/hangul.h"

#define countof(x)  ((sizeof(x)) / (sizeof(x[0])))

static bool
check_preedit(const char* keyboard, const char* input, ...)
{
    HangulInputContext* ic;
    const char* p;
    const ucschar* preedit;
    ucschar code;
    va_list ap;

    ic = hangul_ic_new(keyboard);

    p = input;
    while (*p != '\0') {
	hangul_ic_process(ic, *p);
	p++;
    }

    preedit = hangul_ic_get_preedit_string(ic);

    va_start(ap, input);

    code = va_arg(ap, ucschar);
    while (code != 0) {
	if (*preedit != code)
	    return false;

	code = va_arg(ap, ucschar);
	preedit++;
    }

    va_end(ap);

    hangul_ic_delete(ic);

    return true;
}

static bool
check_commit(const char* keyboard, const char* input, ...)
{
    HangulInputContext* ic;
    const char* p;
    const ucschar* commit;
    ucschar code;
    va_list ap;

    ic = hangul_ic_new(keyboard);

    p = input;
    while (*p != '\0') {
	hangul_ic_process(ic, *p);
	p++;
    }

    commit = hangul_ic_get_commit_string(ic);

    va_start(ap, input);

    code = va_arg(ap, ucschar);
    while (code != 0) {
	if (*commit != code)
	    return false;

	code = va_arg(ap, ucschar);
	commit++;
    }

    va_end(ap);

    hangul_ic_delete(ic);

    return true;
}

START_TEST(test_hangul_ic_process_2)
{
    /* ㄱㅏㅉ */
    fail_unless(check_commit("2", "rkW", 0xac00, 0));
    fail_unless(check_preedit("2", "rkW", 0x3149, 0));
    /* ㅂㅓㅅㅅㅡ */
    fail_unless(check_commit("2", "qjttm", 0xbc97, 0));
    fail_unless(check_preedit("2", "qjttm", 0xc2a4, 0));
    /* ㅂㅓㅆㅡ */
    fail_unless(check_commit("2", "qjTm", 0xbc84, 0));
    fail_unless(check_preedit("2", "qjTm", 0xc4f0, 0));
}
END_TEST

START_TEST(test_hangul_ic_process_2y)
{
    /* ㅎ     */
    fail_unless(check_preedit("2y", "g", 0x314e, 0));
    /*   ㅗ   */
    fail_unless(check_preedit("2y", "h", 0x3157, 0));
    /*     ㅌ */
    fail_unless(check_preedit("2y", "x", 0x314c, 0));
    /* ㅂㅇ   */
    fail_unless(check_preedit("2y", "qd", 0x3178, 0));
    /* ᄼ     */
    fail_unless(check_preedit("2y", "Z", 0x113c, 0x1160, 0));
    /* ᅐ     */
    fail_unless(check_preedit("2y", "V", 0x1150, 0x1160, 0));
    /* ᅝ     */
    fail_unless(check_preedit("2y", "sg", 0x115d, 0x1160, 0));

    /* ㄱㅏㅇ */
    fail_unless(check_preedit("2y", "rkd", 0xac15, 0));
    /* ㄹㅐ   */
    fail_unless(check_preedit("2y", "fo", 0xb798, 0));
    /* ㅎ. ㄴ */
    fail_unless(check_preedit("2y", "gKs", 0x1112, 0x119e, 0x11ab, 0));
    /* ㅂㅂㅇㅏㅁㅅㅅ */ 
    fail_unless(check_preedit("2y", "qqdhatt", 0x112c, 0x1169, 0x11de, 0));
    /* ㅂㅂㅇㅏㅁㅅㅅㅛ */ 
    fail_unless(check_commit("2y", "qqdhatty", 0x112c, 0x1169, 0x11dd, 0));
    fail_unless(check_preedit("2y", "qqdhatty", 0xc1fc, 0));
    /* ㅂㅂㅇㅏㅁㅆㅛ */ 
    fail_unless(check_commit("2y", "qqdhaTy", 0x112c, 0x1169, 0x11b7, 0));
    fail_unless(check_preedit("2y", "qqdhaTy", 0xc448, 0));
    /* 옛이응 처리 */
    /* ㅇㅇㅏㅇㅇㅏ */
    fail_unless(check_commit("2y", "ddkdd", 0x1147, 0x1161, 0x11bc, 0));
    fail_unless(check_preedit("2y", "ddkdd", 0x3147, 0));
    /* ㄱㅏㆁㆁ */
    fail_unless(check_preedit("2y", "rkDD", 0x1100, 0x1161, 0x11ee, 0));
    /* ㄱㅏㆁㆁㅏ */
    fail_unless(check_commit("2y", "rkDDk", 0x1100, 0x1161, 0x11f0, 0));
    fail_unless(check_preedit("2y", "rkDDk", 0x114c, 0x1161, 0));
    /* ㅏㅏㅏㅏ */
    fail_unless(check_preedit("2y", "kkkk", 0x115f, 0x11a2, 0));
}
END_TEST

START_TEST(test_hangul_ic_process_3f)
{
    /* L V T  */
    /* ㅎ     */
    fail_unless(check_preedit("3f", "m", 0x314e, 0));
    /*   ㅗ   */
    fail_unless(check_preedit("3f", "v", 0x3157, 0));
    /*     ㅌ */
    fail_unless(check_preedit("3f", "W", 0x314c, 0));

    /* ㄱㅏㅇ */
    fail_unless(check_preedit("3f", "kfa", 0xac15, 0));
    /* ㄹㅐ   */
    fail_unless(check_preedit("3f", "yr", 0xb798, 0));
    /* ㄴ  ㅁ */
    fail_unless(check_preedit("3f", "hz", 0x1102, 0x1160, 0x11b7, 0));
    /*   ㅜㅅ */ 
    fail_unless(check_preedit("3f", "tq", 0x115f, 0x1165, 0x11ba, 0));
}
END_TEST

START_TEST(test_hangul_ic_process_romaja)
{
    HangulInputContext* ic;
    const ucschar* preedit;
    const ucschar* commit;

    // romaja keyboard test
    ic = hangul_ic_new("ro");

    // basic test: han produces 한
    hangul_ic_process(ic, 'h');
    hangul_ic_process(ic, 'a');
    hangul_ic_process(ic, 'n');

    preedit = hangul_ic_get_preedit_string(ic);
    commit = hangul_ic_get_commit_string(ic);
    fail_unless(preedit[0] == 0xd55c); // 한
    fail_unless(commit[0] == 0);

    hangul_ic_reset(ic);

    // insert ㅇ when a syllable is not started with consonant
    hangul_ic_process(ic, 'a');

    preedit = hangul_ic_get_preedit_string(ic);
    commit = hangul_ic_get_commit_string(ic);
    fail_unless(preedit[0] == 0xc544); // 아
    fail_unless(commit[0] == 0);

    // remove correctly when automatically ㅇ was inserted
    hangul_ic_backspace(ic);

    preedit = hangul_ic_get_preedit_string(ic);
    commit = hangul_ic_get_commit_string(ic);
    fail_unless(preedit[0] == 0);
    fail_unless(commit[0] == 0);

    // append ㅡ when a syllable is not ended with vowel
    hangul_ic_process(ic, 't');
    hangul_ic_process(ic, 't');

    preedit = hangul_ic_get_preedit_string(ic);
    commit = hangul_ic_get_commit_string(ic);
    fail_unless(preedit[0] == 0x314c); // ㅌ
    fail_unless(commit[0] == 0xd2b8);  // 트

    // ng makes trailing ㅇ
    hangul_ic_reset(ic);
    hangul_ic_process(ic, 'g');
    hangul_ic_process(ic, 'a');
    hangul_ic_process(ic, 'n');
    hangul_ic_process(ic, 'g');

    preedit = hangul_ic_get_preedit_string(ic);
    commit = hangul_ic_get_commit_string(ic);
    fail_unless(preedit[0] == 0xac15); // 강
    fail_unless(commit[0] == 0);

    // gangi makes 강이
    hangul_ic_process(ic, 'i');

    preedit = hangul_ic_get_preedit_string(ic);
    commit = hangul_ic_get_commit_string(ic);
    fail_unless(preedit[0] == 0xc774); // 이
    fail_unless(commit[0] == 0xac15);  // 강

    // nanG makes 난ㄱ
    // uppercase makes new syllable
    hangul_ic_process(ic, 'n');
    hangul_ic_process(ic, 'a');
    hangul_ic_process(ic, 'n');
    hangul_ic_process(ic, 'G');

    preedit = hangul_ic_get_preedit_string(ic);
    commit = hangul_ic_get_commit_string(ic);
    fail_unless(preedit[0] == 0x3131); // ㄱ
    fail_unless(commit[0] == 0xb09c);  // 난

    // special operation for x
    // x generate ㅈ for leading consonant
    hangul_ic_reset(ic);
    hangul_ic_process(ic, 'x');
    hangul_ic_process(ic, 'x');

    preedit = hangul_ic_get_preedit_string(ic);
    commit = hangul_ic_get_commit_string(ic);
    fail_unless(preedit[0] == 0x3148); // 지
    fail_unless(commit[0] == 0xc988);

    hangul_ic_reset(ic);
    hangul_ic_process(ic, 'x');
    hangul_ic_process(ic, 'y');

    preedit = hangul_ic_get_preedit_string(ic);
    commit = hangul_ic_get_commit_string(ic);
    fail_unless(preedit[0] == 0xc9c0); // 지
    fail_unless(commit[0] == 0x0);

    // x generate ㄱㅅ for trailing consonant
    // and ㅅ will be transferred to next syllable when next input
    // character is vowel.
    hangul_ic_reset(ic);
    hangul_ic_process(ic, 's');
    hangul_ic_process(ic, 'e');
    hangul_ic_process(ic, 'x');
    hangul_ic_process(ic, 'y');

    preedit = hangul_ic_get_preedit_string(ic);
    commit = hangul_ic_get_commit_string(ic);
    fail_unless(preedit[0] == 0xc2dc); // 시
    fail_unless(commit[0] == 0xc139);  // 섹
    
    hangul_ic_delete(ic);
}
END_TEST

START_TEST(test_syllable_iterator)
{
    ucschar str[] = {
	// L L V V T T
	0x1107, 0x1107, 0x116e, 0x1166, 0x11af, 0x11a8,          // 0
	// L V T
	0x1108, 0x1170, 0x11b0,                                  // 6
	// L L V V T T M
	0x1107, 0x1107, 0x116e, 0x1166, 0x11af, 0x11a8, 0x302E,  // 9
	// L V T M
	0x1108, 0x1170, 0x11b0, 0x302F,                          // 16
	// Lf V
	0x115f, 0x1161,                                          // 20
	// L Vf
	0x110c, 0x1160,                                          // 22
	// L LVT T
	0x1107, 0xbc14, 0x11a8,                                  // 24
	// L LV T
	0x1100, 0xac00, 0x11a8,                                  // 27
	// LVT
	0xc00d,                                                  // 30 
	// other
	'a',                                                     // 31
	0                                                        // 32
    };

    const ucschar* begin = str;
    const ucschar* end = str + countof(str) - 1;
    const ucschar* s = str;

    s = hangul_syllable_iterator_next(s, end);
    fail_unless(s - str == 6,
		"error: next syllable: L L V V T T");

    s = hangul_syllable_iterator_next(s, end);
    fail_unless(s - str == 9,
		"error: next syllable: L V T");

    s = hangul_syllable_iterator_next(s, end);
    fail_unless(s - str == 16,
		"error: next syllable: L L V V T T M");

    s = hangul_syllable_iterator_next(s, end);
    fail_unless(s - str == 20,
		"error: next syllable: L V T M");

    s = hangul_syllable_iterator_next(s, end);
    fail_unless(s - str == 22,
		"error: next syllable: Lf V");

    s = hangul_syllable_iterator_next(s, end);
    fail_unless(s - str == 24,
		"error: next syllable: L Vf");

    s = hangul_syllable_iterator_next(s, end);
    fail_unless(s - str == 27,
		"error: next syllable: L LVT T");

    s = hangul_syllable_iterator_next(s, end);
    fail_unless(s - str == 30,
		"error: next syllable: L LV T");

    s = hangul_syllable_iterator_next(s, end);
    fail_unless(s - str == 31,
		"error: next syllable: LVT");

    s = hangul_syllable_iterator_next(s, end);
    fail_unless(s - str == 32,
		"error: next syllable: other");

    s = end;
    s = hangul_syllable_iterator_prev(s, begin);
    fail_unless(s - str == 31,
		"error: prev syllable: other");

    s = hangul_syllable_iterator_prev(s, begin);
    fail_unless(s - str == 30,
		"error: prev syllable: LVT");

    s = hangul_syllable_iterator_prev(s, begin);
    fail_unless(s - str == 27,
		"error: prev syllable: L LV T");

    s = hangul_syllable_iterator_prev(s, begin);
    fail_unless(s - str == 24,
		"error: prev syllable: L LVT T");

    s = hangul_syllable_iterator_prev(s, begin);
    fail_unless(s - str == 22,
		"error: prev syllable: L Vf");

    s = hangul_syllable_iterator_prev(s, begin);
    fail_unless(s - str == 20,
		"error: prev syllable: Lf V");

    s = hangul_syllable_iterator_prev(s, begin);
    fail_unless(s - str == 16,
		"error: prev syllable: L V T M");

    s = hangul_syllable_iterator_prev(s, begin);
    fail_unless(s - str == 9,
		"error: prev syllable: L L V V T T M");

    s = hangul_syllable_iterator_prev(s, begin);
    fail_unless(s - str == 6,
		"error: prev syllable: L V T");

    s = hangul_syllable_iterator_prev(s, begin);
    fail_unless(s - str == 0,
		"error: prev syllable: L L V V T T");
}
END_TEST

START_TEST(test_hangul_keyboard)
{
    const char* id;
    const char* name;
    unsigned int n;
    unsigned int i;

    n = hangul_ic_get_n_keyboards();
    fail_unless(n != 0,
		"error: there is no builtin hangul keyboard");

    for (i = 0; i < n; ++i) {
	id = hangul_ic_get_keyboard_id(i);
	fail_unless(id != NULL,
		    "error: keyboard id == NULL");
    }

    for (i = 0; i < n; ++i) {
	name = hangul_ic_get_keyboard_name(i);
	fail_unless(name != NULL,
		    "error: keyboard id == NULL");
    }
}
END_TEST

Suite* libhangul_suite()
{
    Suite* s = suite_create("libhangul");

    TCase* hangul = tcase_create("hangul");
    tcase_add_test(hangul, test_hangul_ic_process_2);
    tcase_add_test(hangul, test_hangul_ic_process_2y);
    tcase_add_test(hangul, test_hangul_ic_process_3f);
    tcase_add_test(hangul, test_hangul_ic_process_romaja);
    tcase_add_test(hangul, test_syllable_iterator);
    tcase_add_test(hangul, test_hangul_keyboard);
    suite_add_tcase(s, hangul);

    return s;
}

int main()
{
    int number_failed;
    Suite* s = libhangul_suite();
    SRunner* sr = srunner_create(s);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
