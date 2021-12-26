#include <catch2/catch.hpp>

#include "stirrup/environment_variable.hpp"
#include "stirrup/string.hpp"

namespace Catch {
template<>
struct StringMaker<std::u32string> {
    static std::string convert( std::u32string const& value ) {
        return stirrup::repr(value);
    }
};

template<int SZ>
struct StringMaker<char32_t[SZ]> {
    static std::string convert(char32_t const* value) {
        return stirrup::repr(std::u32string(value));
    }
};

}

SCENARIO("stirrup environment variables")
{
    GIVEN("an environment variable that is not set")
    {
        WHEN("the value is read")
        {
            auto const value = stirrup::getenv(U"STIRRUP_TEST_SHOULD_NOT_BE_SET");
            THEN("the value is empty")
            {
                CHECK(value.empty());
            }
        }
    }

    GIVEN("an environment variable that has a value")
    {
        stirrup::putenv(U"STIRRUP_TEST_ENVAR", U"the value");

        WHEN("the value is read")
        {
            auto const value = stirrup::getenv(U"STIRRUP_TEST_ENVAR");
            THEN("the value is empty")
            {
                CHECK(value == U"the value");
            }
        }
    }
}
