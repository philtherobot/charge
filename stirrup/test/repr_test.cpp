#include "stirrup/repr.hpp"

#include <catch2/catch.hpp>

using namespace stirrup;
using std::vector;

SCENARIO("Unicode to ASCII representation")
{
    WHEN("representing a Unicode character")
    {
        CHECK(repr(U'a') == "a");
        CHECK(repr(U'\0') == "\0");
        CHECK(repr(char32_t(1)) == "\x01");
        CHECK(repr(U'~') == "~");
        CHECK(repr(U'\x7F') == "\x7F");
        CHECK(repr(U'\x80') == "\\u0080");
        CHECK(repr(U'\uFFFF') == "\\uFFFF");
        CHECK(repr(U'\U00010000') == "\\U00010000");
        CHECK(repr(U'\U0001F600') == "\\U0001F600");
    }

    WHEN("representing a Unicode string")
    {
        CHECK(repr(U"") == "");
        CHECK(repr(U"a") == "a");
        CHECK(repr(U"a\u503c") == "a\\u503C");
        CHECK(repr(U"huge \U0001F600 code point") == "huge \\U0001F600 code point");
    }

    WHEN("representing containers")
    {
        vector<int> v{1, 2, 3};
        CHECK(repr(v) == "{1,2,3}");
    }
}

SCENARIO("UTF-8 to ASCII representation")
{
    WHEN("representing a UTF-8 character")
    {
        CHECK(repr(u8'a') == "a");
        CHECK(repr(u8'\0') == "\0");
        CHECK(repr(char8_t(1)) == "\x01");
        CHECK(repr(u8'~') == "~");
        CHECK(repr(u8'\x7F') == "\x7F");
        CHECK(repr(char8_t(0x80)) == "\\x80");
        CHECK(repr(char8_t(0xFF)) == "\\xFF");
    }
}

