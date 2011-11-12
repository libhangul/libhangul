#ifndef libhangul_hangulinternals_h
#define libhangul_hangulinternals_h

#define N_ELEMENTS(array) (sizeof (array) / sizeof ((array)[0]))

ucschar hangul_jongseong_get_diff(ucschar prevjong, ucschar jong);

ucschar hangul_choseong_to_jongseong(ucschar ch);
ucschar hangul_jongseong_to_choseong(ucschar ch);
void    hangul_jongseong_decompose(ucschar ch, ucschar* jong, ucschar* cho);

#endif /* libhangul_hangulinternals_h */
