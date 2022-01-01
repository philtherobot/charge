#include <catch2/catch.hpp>

#include "stirrup/error.hpp"
#include "stirrup/locale.hpp"
#include "stirrup/string.hpp"
#include "stirrup/string_windows.hpp"
#include "stirrup/test/string_string_maker.hpp"

using std::u32string;
using std::u8string;
using namespace stirrup;
using namespace stirrup::windows;
using std::locale;

//namespace
//{
//
//class error_message_is
//{
//public:
//    error_message_is(string const & expected_message)
//        : expected_message_(expected_message)
//    {}
//
//    bool operator()(std::runtime_error const & error)
//    {
//        return error.what() == expected_message_;
//    }
//
//private:
//    string expected_message_;
//};
//
//}
//BOOST_AUTO_TEST_SUITE(suite_stirrup);
//BOOST_AUTO_TEST_SUITE(suite_string);
//
//BOOST_AUTO_TEST_CASE(case_quoting)
//{
//    BOOST_TEST(quote_if_needed("noneed") == "noneed");
//    BOOST_TEST(quote_if_needed("must quote") == "\"must quote\"");
//    BOOST_TEST(quote_if_needed(" mustquote") == "\" mustquote\"");
//    BOOST_TEST(quote_if_needed("mustquote ") == "\"mustquote \"");
//    BOOST_TEST(quote_if_needed("") == "");
//}
//
//BOOST_AUTO_TEST_CASE(case_conversion_to_narrow_string)
//{
//    u8string source_string(u8"characters under 127");
//    string destination = to_narrow_string(source_string);
//    BOOST_CHECK(destination == "characters under 127");
//}
//
//BOOST_AUTO_TEST_CASE(case_conversion_to_narrow_string_unicode)
//{
//    u8string source_string(u8"with unicode \u0345");
//
//    BOOST_CHECK_EXCEPTION(
//        to_narrow_string(source_string),
//        std::runtime_error,
//        error_message_is("Character \\u0345 cannot be converted to ASCII.")
//    );
//}
//
//BOOST_AUTO_TEST_CASE(case_conversion_to_wide_string_unicode)
//{
//    u8string source_string(u8"with unicode \u0345");
//    BOOST_CHECK(to_wide_string(source_string) == L"with unicode \u0345");
//}
//
//BOOST_AUTO_TEST_CASE(case_conversion_from_narrow_string)
//{
//    string narrow_string("string with narrow characters");
//    BOOST_CHECK(to_u8string(narrow_string) == u8"string with narrow characters");
//}
//
//BOOST_AUTO_TEST_CASE(case_conversion_from_wide_string)
//{
//    wstring wide_string(L"with unicode \u0345");
//    BOOST_CHECK(to_u8string(wide_string) == u8"with unicode \u0345");
//}
//
//BOOST_AUTO_TEST_SUITE_END();
//
//BOOST_AUTO_TEST_SUITE_END();

#include <stirrup/string.hpp>

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

SCENARIO("wchar_t/Unicode transcoding")
{
    CHECK(transcode_from(L"hello \u503c") == U"hello \u503c");
    CHECK(transcode_to_wstring(U"hello \u503c") == L"hello \u503c");

    CHECK_THROWS_MATCHES(
        transcode_to_wstring(U"hello \U0001F600"),
        stirrup::runtime_error,
        Catch::Message("symbol cannot be converted"));
}

SCENARIO("UTF-8 decoding from binary data")
{
    GIVEN("ASCII data")
    {
        binary_buffer buffer = {'H', 'e', 'l', 'l', 'o'};

        WHEN("decoding all the data")
        {
            auto const string = decode_utf8(buffer, 10);
            CHECK(string == U"Hello");
            CHECK(buffer.empty());
        }

        WHEN("decoding only part of the data")
        {
            auto const string = decode_utf8(buffer, 3);
            CHECK(string == U"Hel");
            CHECK(buffer == binary_buffer{'l', 'o'});
        }
    }

    GIVEN("UTF-8 data")
    {
        binary_buffer buffer = {'H', 'e', 'l', 'l', 'o', 0xE5, 0x80, 0xBC};

        WHEN("decoding all the data")
        {
            auto const string = decode_utf8(buffer, 6);
            CHECK(string == U"Hello\u503C");
            CHECK(buffer.empty());
        }

        WHEN("decoding only part of the data")
        {
            auto const string = decode_utf8(buffer, 5);
            CHECK(string == U"Hello");
            CHECK(buffer == binary_buffer{0xE5, 0x80, 0xBC});
        }
    }

    GIVEN("incomplete code point in UTF-8 data")
    {
        binary_buffer buffer = {'H', 'e', 'l', 'l', 'o', 0xE5, 0x80};

        WHEN("decoding all the data")
        {
            auto const string = decode_utf8(buffer, 6);
            CHECK(string == U"Hello");
            CHECK(buffer == binary_buffer{0xE5, 0x80});
        }
    }
}

// Start obsolete or under reevaluation
// most probably obsolete
SCENARIO("locale to Unicode transcoding")
{
    auto const initial_locale = std::locale();

    auto character_d8 = "\xD8";

    auto windows_latin1_locale_name = ".850";
    CHECK(transcode_from_locale(character_d8, std::locale(windows_latin1_locale_name)) == U"\u00CF");

    CHECK(initial_locale == std::locale());

    auto windows_greek_locale_name = ".1253";
    CHECK(transcode_from_locale(character_d8, std::locale(windows_greek_locale_name)) == U"\u03A8");

    CHECK(initial_locale == std::locale());

    {
        locale_block set_and_restore_locale(std::locale(".850"));
        CHECK(transcode_from_locale(character_d8) == U"\u00CF");
    }
}

// most probably obsolete
SCENARIO("text encoding conversions")
{
    GIVEN("the IBM/PC codepage (437)")
    {
        auto const ibm_codepage = locale(".437");
        vector<char> top_right_corner = {char(0xBF)};
        CHECK(decode_string(top_right_corner, ibm_codepage) == U"\u2510");
        CHECK(encode_string(U"\u2510", ibm_codepage) == top_right_corner);
    }

    GIVEN("the Windows Western codepage (1252)")
    {
        auto const western_codepage = locale(".1252");
        vector<char> inverted_question_mark = {char(0xBF)};
        CHECK(decode_string(inverted_question_mark, western_codepage) == U"\u00BF");
        CHECK(encode_string(U"\u00BF", western_codepage) == inverted_question_mark);
    }

    GIVEN("the Windows Cyrillic codepage (1251)")
    {
        auto const cyrillic_codepage = locale(".1251");
        vector<char> small_letter_yi = {char(0xBF)};
        CHECK(decode_string(small_letter_yi, cyrillic_codepage) == U"\u0457");
        CHECK(encode_string(U"\u0457", cyrillic_codepage) == small_letter_yi);
    }

    GIVEN("UTF-8 encoded text")
    {
        auto const utf8_codepage = locale(".UTF-8");

        auto to_binary = [](char8_t const * string)
        {
            vector<char> result;
            std::ranges::copy(std::u8string(string), back_inserter(result));
            return result;
        };

        auto hello_top_right_corner = to_binary(u8"hello\u2510");
        CHECK(decode_string(hello_top_right_corner, utf8_codepage) == U"hello\u2510");
        CHECK(encode_string(U"hello\u2510", utf8_codepage) == hello_top_right_corner);

        auto hello_smiley = to_binary(u8"hello\U0001F600");
        CHECK(decode_string(hello_smiley, utf8_codepage) == U"hello\U0001F600");
        CHECK(encode_string(U"hello\U0001F600", utf8_codepage) == hello_smiley);
    }
        // test long strings to overflow the function's buffer

    GIVEN("UTF-16 encoded text")
    {
        vector<WCHAR> hello_top_right_corner{'h', 'e', 'l', 'l', 'o', L'\u2510'};
        CHECK(stirrup::windows::decode_string(hello_top_right_corner) == U"hello\u2510");
        CHECK(stirrup::windows::encode_string(U"hello\u2510") == hello_top_right_corner);

        vector<WCHAR> hello_smiley{'h', 'e', 'l', 'l', 'o', 0xD83D, 0xDE00};
        CHECK(stirrup::windows::decode_string(hello_smiley) == U"hello\U0001F600");
        CHECK(stirrup::windows::encode_string(U"hello\U0001F600") == hello_smiley);
        CHECK(repr(stirrup::windows::encode_string(U"hello\U0001F600")) == repr(hello_smiley));
    }
}

// most probably obsolete
// Note: the following is specific to Windows
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
