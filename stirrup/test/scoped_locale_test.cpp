#include "stirrup/stirrup.hpp"

#include <boost/test/unit_test.hpp>
#include <clocale>

using namespace std;
using namespace stirrup;

BOOST_AUTO_TEST_SUITE(suite_stirrup)
BOOST_AUTO_TEST_SUITE(suite_scoped_locale)

BOOST_AUTO_TEST_CASE(case_scoped_locale)
{
    setlocale(LC_ALL, "fr_CA.utf8");
    BOOST_TEST(setlocale(LC_ALL, nullptr) == string("fr_CA.utf8"));

    {
        scoped_locale here(LC_ALL, "en_US.utf8");

        BOOST_TEST(setlocale(LC_ALL, nullptr) == string("en_US.utf8"));
    }

    BOOST_TEST(setlocale(LC_ALL, nullptr) == string("fr_CA.utf8"));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
