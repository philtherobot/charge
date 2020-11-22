#include <boost/test/unit_test.hpp>

#include "stirrup/error.hpp"

using namespace stirrup;

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
