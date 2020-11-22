#include <boost/test/unit_test.hpp>

#include "stirrup/string.hpp"

using namespace std;
using namespace stirrup;

namespace
{

class error_message_is
{
public:
    error_message_is(string const & expected_message)
        : expected_message_(expected_message)
    {}

    bool operator()(std::runtime_error const & error)
    {
        return error.what() == expected_message_;
    }

private:
    string expected_message_;
};

}
BOOST_AUTO_TEST_SUITE(suite_stirrup);
BOOST_AUTO_TEST_SUITE(suite_string);

BOOST_AUTO_TEST_CASE(case_quoting)
{
    BOOST_TEST(quote_if_needed("noneed") == "noneed");
    BOOST_TEST(quote_if_needed("must quote") == "\"must quote\"");
    BOOST_TEST(quote_if_needed(" mustquote") == "\" mustquote\"");
    BOOST_TEST(quote_if_needed("mustquote ") == "\"mustquote \"");
    BOOST_TEST(quote_if_needed("") == "");
}

BOOST_AUTO_TEST_CASE(case_conversion_to_narrow_string)
{
    u8string source_string(u8"characters under 127");
    string destination = to_narrow_string(source_string);
    BOOST_CHECK(destination == "characters under 127");
}

BOOST_AUTO_TEST_CASE(case_conversion_to_narrow_string_unicode)
{
    u8string source_string(u8"with unicode \u0345");

    BOOST_CHECK_EXCEPTION(
        to_narrow_string(source_string),
        std::runtime_error,
        error_message_is("Character \\u0345 cannot be converted to ASCII.")
    );
}

BOOST_AUTO_TEST_CASE(case_conversion_to_wide_string_unicode)
{
    u8string source_string(u8"with unicode \u0345");
    BOOST_CHECK(to_wide_string(source_string) == L"with unicode \u0345");
}

BOOST_AUTO_TEST_CASE(case_conversion_from_narrow_string)
{
    string narrow_string("string with narrow characters");
    BOOST_CHECK(to_u8string(narrow_string) == u8"string with narrow characters");
}

BOOST_AUTO_TEST_CASE(case_conversion_from_wide_string)
{
    wstring wide_string(L"with unicode \u0345");
    BOOST_CHECK(to_u8string(wide_string) == u8"with unicode \u0345");
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();
