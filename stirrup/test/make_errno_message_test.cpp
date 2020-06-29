#include <boost/test/unit_test.hpp>

#include "stirrup/stirrup.hpp"

#include <cwchar>

using namespace stirrup;

// todo-php: we need some way to convert our UTF-8 strings to the system's encoding.
//namespace std
//{
//
//ostream & operator<<(ostream & os, wstring const & wide_text)
//{
//    os << string_construction::from(wide_text);
//    return os;
//}
//
//}

BOOST_AUTO_TEST_SUITE(suite_stirrup);
BOOST_AUTO_TEST_SUITE(suite_make_errno_message);

BOOST_AUTO_TEST_CASE(case_make_messages)
{
    BOOST_TEST(
        make_errno_message("getthis", EINVAL) == "system (errno) error 22 (Invalid argument) in function getthis"
    );
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE_END();
