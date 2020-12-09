#include <catch2/catch.hpp>

#include "stirrup/string.hpp"
#include "stirrup/test/string_string_maker.hpp"

using std::u32string;
using namespace stirrup;

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

SCENARIO("UTF-8 string transcoding")
{
    WHEN("we transcode a UTF-8 string to a Unicode")
    {
        vector<char8_t> string = {'H', 'e', 'l', 'l', 'o', 0xE5, 0x80, 0xBC};
        CHECK(transcode_from_utf8(string) == U"Hello\u503C");

        vector<char8_t> contains_a_zero = {'H', '\0', 'l', 'l', 'o'};
        CHECK(transcode_from_utf8(contains_a_zero) == U"H\0llo");
    }

    WHEN("we transcode a Unicode string to UTF-8")
    {
        u32string string = U"Hello\u503C";
        CHECK(transcode_to_utf8(string) == vector<char8_t>{'H', 'e', 'l', 'l', 'o', 0xE5, 0x80, 0xBC});

        u32string contains_a_zero{U'H', U'\0', U'l', U'l', U'o'};
        CHECK(transcode_to_utf8(contains_a_zero) == vector<char8_t>{'H', '\0', 'l', 'l', 'o'});
    }
}

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
