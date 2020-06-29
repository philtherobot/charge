#include <boost/test/unit_test.hpp>
#include <clocale>

#include "stirrup/stirrup.hpp"

using namespace stirrup;

BOOST_TEST_DONT_PRINT_LOG_VALUE(std::wstring)

BOOST_AUTO_TEST_SUITE(suite_stirrup);
BOOST_AUTO_TEST_SUITE(suite_string_construction);

BOOST_AUTO_TEST_SUITE(suite_from_wstring)

//BOOST_AUTO_TEST_CASE(case_ascii)
//{
//    BOOST_TEST(str_construction::from("test") == "test");
//}
//
//BOOST_AUTO_TEST_CASE(case_convert_from_system)
//{
//    scoped_locale here(LC_ALL, "en_US.1252");
//    BOOST_TEST(str_construction::wstring_from_system_string("\xE9") == L"\xE9"); // utf8 = "\xC3\xA9");
//}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();
