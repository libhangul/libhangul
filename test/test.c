#include <stdlib.h>
#include <check.h>

#include "../hangul/hangul.h"

#define countof(x)  ((sizeof(x)) / (sizeof(x[0])))

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

Suite* libhangul_suite()
{
    Suite* s = suite_create("libhangul");

    TCase* hangul = tcase_create("hangul");
    tcase_add_test(hangul, test_hangul_ic_process_romaja);
    tcase_add_test(hangul, test_syllable_iterator);
    suite_add_tcase(s, hangul);

    return s;
}

int main()
{
    int number_failed;
    Suite* s = libhangul_suite();
    SRunner* sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
