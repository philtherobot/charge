#include <boost/test/unit_test.hpp>

#include "stirrup/string.hpp"

#include <cwchar>

using namespace stirrup;

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

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();
