#include <catch2/catch.hpp>

#include "stirrup/locale.hpp"

using namespace stirrup;
using std::locale;

SCENARIO("locale_block is protects the locale")
{
    locale test_program_locale;

    GIVEN("the program is in a known locale")
    {
        locale_block protect_the_test_program(locale(".850"));

        REQUIRE(locale() == locale(".850"));

        WHEN("we open a locale_block")
        {
            {
                locale_block we_test_this_block(locale("FR.UTF-8"));

                CHECK(locale() == locale("FR.UTF-8"));
            }

            CHECK(locale() == locale(".850"));
        }

    }

    REQUIRE(locale() == test_program_locale);
}
