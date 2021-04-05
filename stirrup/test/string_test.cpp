#include "stirrup/error.hpp"
#include "stirrup/locale.hpp"
#include "stirrup/string.hpp"
#include "stirrup/test/installed_locales.hpp"
#include "stirrup/test/string_string_maker.hpp"

#include <catch2/catch.hpp>

using std::string;
using std::u32string;
using namespace stirrup;
using std::locale;

namespace
{

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

}

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

SCENARIO("wchar_t to Unicode transcoding")
{
    CHECK(transcode_from_wstring(L"hello \u503c") == U"hello \u503c");
    CHECK(transcode_to_wstring(U"hello \u503c") == L"hello \u503c");

    if constexpr (sizeof(wchar_t) < sizeof(char32_t))
    {
        // todo-php: check message, it should include the char that cannot be converted
        CHECK_THROWS_AS(transcode_to_wstring(U"hello \U0001F600"), stirrup::runtime_error);
    }
}

SCENARIO("text encoding conversions")
{
    string const windows_western_locale_name = get_installed_locale_for("Windows Western");

    if (windows_western_locale_name.empty())
    {
        WARN("Text encoding tests requiring Windows Western locale skipped.");
    }
    else
    {
        GIVEN("the Windows Western codepage")
        {
            auto const western_codepage = locale(windows_western_locale_name);
            vector<char> inverted_question_mark = {char(0xBF)};
            CHECK(decode_string(inverted_question_mark, western_codepage) == U"\u00BF");
            CHECK(encode_string(U"\u00BF", western_codepage) == inverted_question_mark);
        }
    }

    string const utf8_locale_name = get_installed_locale_for("UTF8");

    if (utf8_locale_name.empty())
    {
        WARN("Text encoding tests requiring UTF-8 locale skipped.");
    }
    else
    {
        GIVEN("UTF-8 encoded text")
        {
            auto const utf8_codepage = locale(utf8_locale_name);

            auto to_binary = [](char8_t const *string) {
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
    }

    // test long strings to overflow the function's buffer
}
