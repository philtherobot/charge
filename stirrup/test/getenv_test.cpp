#include <boost/test/unit_test.hpp>

#include "stirrup/stirrup.hpp"

BOOST_AUTO_TEST_SUITE(suite_stirrup);
BOOST_AUTO_TEST_SUITE(suite_getenv);

//BOOST_AUTO_TEST_CASE(case_get_existing_environment_variable)
//{
//    // todo-php: test some envar in the system encoding, outside of ASCII and make sure we get UTF-8.
//    const auto value = stirrup::getenv("PATH");
//    BOOST_TEST(!value.empty());
//}

BOOST_AUTO_TEST_CASE(case_get_unset_environment_variable)
{
    const auto value = stirrup::getenv("STIRRUP_SHOULD_NOT_BE_SET");
    BOOST_TEST(value.empty());
}

BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE_END();
