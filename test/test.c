#include <stdlib.h>
#include <check.h>

#include "../hangul/hangul.h"

#define countof(x)  ((sizeof(x)) / (sizeof(x[0])))

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
