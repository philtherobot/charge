#include <boost/test/unit_test.hpp>

#include "stirrup/stirrup.hpp"

BOOST_AUTO_TEST_SUITE(suite_stirrup);
BOOST_AUTO_TEST_SUITE(suite_getenv);

BOOST_AUTO_TEST_CASE(case_get_unset_environment_variable)
{
    const auto value = stirrup::getenv("STIRRUP_SHOULD_NOT_BE_SET");
    BOOST_TEST(value.empty());
}

BOOST_AUTO_TEST_CASE(case_get_environment_variable)
{
    _putenv_s("STIRRUP_TEST_ENVAR", "the value");

    const auto value = stirrup::getenv("STIRRUP_TEST_ENVAR");
    BOOST_TEST(value == "the value");
}

BOOST_AUTO_TEST_SUITE_END();
BOOST_AUTO_TEST_SUITE_END();
