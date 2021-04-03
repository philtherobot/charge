#include <catch2/catch.hpp>

#include "stirrup/string_windows.hpp"

SCENARIO("") 
{
    GIVEN("UTF-16 encoded text")
    {
        vector<WCHAR> hello_top_right_corner{ 'h', 'e', 'l', 'l', 'o', L'\u2510'};
        CHECK(stirrup::windows::decode_string(hello_top_right_corner) == U"hello\u2510");
        CHECK(stirrup::windows::encode_string(U"hello\u2510") == hello_top_right_corner);

        vector<WCHAR> hello_smiley{ 'h', 'e', 'l', 'l', 'o', 0xD83D, 0xDE00};
        CHECK(stirrup::windows::decode_string(hello_smiley) == U"hello\U0001F600");
        CHECK(stirrup::windows::encode_string(U"hello\U0001F600") == hello_smiley);
        CHECK(repr(stirrup::windows::encode_string(U"hello\U0001F600")) == repr(hello_smiley));
    }

}

SCENARIO("getting the Windows codepage from a locale")
{
    GIVEN("a locale")
    {
        CHECK(stirrup::windows::codepage_from_locale(locale(".437")) == 437);
        CHECK(stirrup::windows::codepage_from_locale(locale("English_US.437")) == 437);
        CHECK(stirrup::windows::codepage_from_locale(locale(".1252")) == 1252);
        CHECK(stirrup::windows::codepage_from_locale(locale("C")) == 1252);
        CHECK(stirrup::windows::codepage_from_locale(locale(".utf-8")) == 65001);
        CHECK(stirrup::windows::codepage_from_locale(locale(".UTF-8")) == 65001);
        CHECK(stirrup::windows::codepage_from_locale(locale(".utf8")) == 65001);
        CHECK(stirrup::windows::codepage_from_locale(locale(".UTF8")) == 65001);

        // The following test can only succeed on Windows systems set with the
        // system locale at a region that uses the Windows Western codepage such as "English (Canada)"
        // On Windows 10: Control Panel, Clock and Region, Region, Administrative, Change system locale.
        CHECK(stirrup::windows::codepage_from_locale(locale("")) == 1252);
    }
}
