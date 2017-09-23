/*
 * Lua extension library for libhangul.
 * Author: Byeonggon Lee <gonny952@gmail.com>
 * License: Same as libhangul.
 */

#include <lua.h>
#include <lauxlib.h>
#include <hangul.h>
#include <iconv.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int ucs4_to_utf8(ucschar* inbuf, size_t inbufbytes, char* outbuf, size_t outbufbytes) {
    char* inbuftemp = (char*)inbuf;
    char* outbuftemp = outbuf;
    size_t inbufbytesleft = inbufbytes;
    size_t outbufbytesleft = outbufbytes;

    iconv_t cd = iconv_open("UTF-8", "UCS-4LE");
    if(cd == (iconv_t) - 1) {
        fprintf(stderr, "iconv_open failed with %d\n", errno);
        return -1;
    }

    int rc = iconv(cd, &inbuftemp, &inbufbytesleft, &outbuftemp, &outbufbytesleft);
    if(rc == (size_t) - 1) {
        fprintf(stderr, "LINE %d: iconv failed with -1. errno is %d: %s\n", __LINE__, errno, strerror(errno));
        return -1;
    }

    rc = iconv_close(cd);
    if(rc != 0) {
        fprintf(stderr, "iconv_close failed with %d\n", errno);
        return -1;
    }

    return outbufbytes - outbufbytesleft;
}

static int lua_ucs4_to_utf8(lua_State* L) {
    // ucs4String
    size_t ucs4StringSize;
    ucschar* ucs4String = lua_touserdata(L, -1);
    ucs4StringSize = strlen((char*)ucs4String) * sizeof(ucschar);
    char* utf8String = calloc(ucs4StringSize, 1);
    ucs4_to_utf8(ucs4String, ucs4StringSize, utf8String, ucs4StringSize);
    lua_pushstring(L, utf8String); // ucs4String utf8String
    free(utf8String);
    return 1; // ucs4String utf8String
}

static int lua_hangul_ic_process(lua_State* L) {
    // lhic ascii
    if(lua_gettop(L) != 2)
        luaL_error(L, "Expected 2 parameters");
    lua_getfield(L, -2, "c_object"); // lhic ascii hic
    HangulInputContext* hic = lua_touserdata(L, -1);
    lua_pop(L, 1); // lhic ascii
    lua_pushboolean(L, hangul_ic_process(hic, lua_tointeger(L, -1))); // lhic ascii result
    return 1;
}

static int lua_hangul_ic_get_preedit_string(lua_State* L) {
    // lhic
    if(lua_gettop(L) != 1)
        luaL_error(L, "Expected 1 parameter");
    lua_getfield(L, -1, "c_object"); // lhic hic
    HangulInputContext* hic = lua_touserdata(L, -1);
    lua_pop(L, 1); // lhic
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdiscarded-qualifiers" 
    lua_pushlightuserdata(L, hangul_ic_get_preedit_string(hic)); // lhic result
    #pragma GCC diagnostic pop
    return 1;
}

static int lua_hangul_ic_get_commit_string(lua_State* L) {
    // lhic
    if(lua_gettop(L) != 1)
        luaL_error(L, "Exepected 1 parameter");
    lua_getfield(L, -1, "c_object"); // lhic hic
    HangulInputContext* hic = lua_touserdata(L, -1);
    lua_pop(L, 1); // lhic
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdiscarded-qualifiers" 
    lua_pushlightuserdata(L, hangul_ic_get_commit_string(hic)); // lhic result
    #pragma GCC diagnostic pop
    return 1;
}

static int lua_hangul_ic_reset(lua_State* L) {
    // lhic
    if(lua_gettop(L) != 1)
        luaL_error(L, "Exepected 1 parameter");
    lua_getfield(L, -1, "c_object"); // lhic hic
    HangulInputContext* hic = lua_touserdata(L, -1);
    lua_pop(L, 1); // lhic
    hangul_ic_reset(hic); // lhic
    return 0;
}

static int lua_hangul_ic_flush(lua_State* L) {
    // lhic
    if(lua_gettop(L) != 1)
        luaL_error(L, "Exepected 1 parameter");
    lua_getfield(L, -1, "c_object"); // lhic hic
    HangulInputContext* hic = lua_touserdata(L, -1);
    lua_pop(L, 1); // lhic
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdiscarded-qualifiers" 
    lua_pushlightuserdata(L, hangul_ic_flush(hic)); // lhic result
    #pragma GCC diagnostic pop
    return 1;
}

static int lua_hangul_ic_backspace(lua_State* L) {
    // lhic
    if(lua_gettop(L) != 1)
        luaL_error(L, "Exepected 1 parameter");
    lua_getfield(L, -1, "c_object"); // lhic hic
    HangulInputContext* hic = lua_touserdata(L, -1);
    lua_pop(L, 1); // lhic
    lua_pushboolean(L, hangul_ic_backspace(hic)); // lhic result
    return 1;
}

static int lua_hangul_ic_is_empty(lua_State* L) {
    // lhic
    if(lua_gettop(L) != 1)
        luaL_error(L, "Exepected 1 parameter");
    lua_getfield(L, -1, "c_object"); // lhic hic
    HangulInputContext* hic = lua_touserdata(L, -1);
    lua_pop(L, 1); // lhic
    lua_pushboolean(L, hangul_ic_is_empty(hic)); // lhic result
    return 1;
}

static int lua_hangul_ic_has_choseong(lua_State* L) {
    // lhic
    if(lua_gettop(L) != 1)
        luaL_error(L, "Exepected 1 parameter");
    lua_getfield(L, -1, "c_object"); // lhic hic
    HangulInputContext* hic = lua_touserdata(L, -1);
    lua_pop(L, 1); // lhic
    lua_pushboolean(L, hangul_ic_has_choseong(hic)); // lhic result
    return 1;
}

static int lua_hangul_ic_has_jungseong(lua_State* L) {
    // lhic
    if(lua_gettop(L) != 1)
        luaL_error(L, "Exepected 1 parameter");
    lua_getfield(L, -1, "c_object"); // lhic hic
    HangulInputContext* hic = lua_touserdata(L, -1);
    lua_pop(L, 1); // lhic 
    lua_pushboolean(L, hangul_ic_has_jungseong(hic)); // lhic result
    return 1;
}

static int lua_hangul_ic_has_jongseong(lua_State* L) {
    // lhic
    if(lua_gettop(L) != 1)
        luaL_error(L, "Exepected 1 parameter");
    lua_getfield(L, -1, "c_object"); // lhic hic
    HangulInputContext* hic = lua_touserdata(L, -1);
    lua_pop(L, 1); // lhic
    lua_pushboolean(L, hangul_ic_has_jongseong(hic)); // lhic result
    return 1;
}

static int lua_hangul_ic_select_keyboard(lua_State* L) {
    // lhic id
    if(lua_gettop(L) != 2)
        luaL_error(L, "Expect 2 parameters");
    lua_getfield(L, -2, "c_object"); // lhic id hic
    HangulInputContext* hic = lua_touserdata(L, -1);
    lua_pop(L, 1); // lhic id
    const char* id = lua_tostring(L, -1);
    hangul_ic_select_keyboard(hic, id);
    return 0;
}

static int lua_hangul_ic_is_transliteration(lua_State* L) {
    // lhic
    if(lua_gettop(L) != 1)
        luaL_error(L, "Exepected 1 parameter");
    lua_getfield(L, -1, "c_object"); // lhic hic
    HangulInputContext* hic = lua_touserdata(L, -1);
    lua_pop(L, 1); // lhic
    lua_pushboolean(L, hangul_ic_is_transliteration(hic)); // lhic result
    return 1;
}

static int lua_hangul_ic_delete(lua_State* L) {
    // lhic
    if(lua_gettop(L) != 1)
        luaL_error(L, "Exepected 1 parameter");
    lua_getfield(L, -1, "c_object"); // lhic hic
    HangulInputContext* hic = lua_touserdata(L, -1);
    lua_pop(L, 1); // lhic
    hangul_ic_delete(hic);
    return 0;
}

static const struct luaL_Reg lua_hangul_ic_methods[] = {
    {"process", lua_hangul_ic_process},
    {"get_preedit_string", lua_hangul_ic_get_preedit_string},
    {"get_commit_string", lua_hangul_ic_get_commit_string},
    {"reset", lua_hangul_ic_reset},
    {"flush", lua_hangul_ic_flush},
    {"backspace", lua_hangul_ic_backspace},
    {"is_empty", lua_hangul_ic_is_empty},
    {"has_choseong", lua_hangul_ic_has_choseong},
    {"has_jungseong", lua_hangul_ic_has_jungseong},
    {"has_jongseong", lua_hangul_ic_has_jongseong},
    {"select_keyboard", lua_hangul_ic_select_keyboard},
    {"is_transliteration", lua_hangul_ic_is_transliteration},
    {"delete", lua_hangul_ic_delete},
    {NULL, NULL}
};

static int lua_hangul_ic_new(lua_State* L) {
    // (keyboard)
    lua_newtable(L); // (keyboard) lhic
    HangulInputContext* hic;
    if(lua_gettop(L) == 2) // keyboard lhic
        hic = hangul_ic_new(lua_tostring(L, -1));
    else
        hic = hangul_ic_new("2");
    lua_pushlightuserdata(L, hic); // (keyboard) lhic hic
    lua_setfield(L, -2, "c_object"); // (keyboard) lhic
    luaL_setfuncs(L, lua_hangul_ic_methods, 0); // (keyboard) lhic
    return 1;
}

static const struct luaL_Reg lua_hangul_functions[] = {
    {"ic_new", lua_hangul_ic_new},
    {"ucs4_to_utf8", lua_ucs4_to_utf8},
    {NULL, NULL}
};

int luaopen_hangul(lua_State* L) {
    luaL_newmetatable(L, "lua_hangul_ic"); // lua_hangul_ic
    lua_pushvalue(L, -1); // lua_hangul_ic lua_hangul_ic
    lua_setfield(L, -2, "__index"); // lua_hangul_ic
    luaL_setfuncs(L, lua_hangul_ic_methods, 0); // lua_hangul_ic
    lua_pop(L, 1); //
    luaL_newlib(L, lua_hangul_functions); // library
    return 1;
}
