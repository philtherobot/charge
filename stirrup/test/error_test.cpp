#include <catch2/catch.hpp>

#include "stirrup/error.hpp"
#include "stirrup/test/string_string_maker.hpp"

using namespace stirrup;
using std::string;

TEST_CASE("making error messages")
{
    SECTION("make_errno_message")
    {
        CHECK(
            make_errno_message(U"getthis", EINVAL) == std::u32string(U"system error (errno) 22 (Invalid argument) in function getthis")
        );
    }
}

TEST_CASE("throwing exceptions")
{
    SECTION("we can catch a logic_error as the std::logic_error type")
    {
        CHECK_THROWS_AS(
            throw logic_error(U"an error occurred"),
            std::logic_error
        );
    }

    enum
    {
        std_runtime_error_type, std_logic_error_type, std_exception_type
    } type_caught{};

    SECTION("a logic_error is not an std::runtime_error")
    {
        try
        {
            throw logic_error(U"an error occurred");
        }
        catch (std::runtime_error const &)
        {
            type_caught = std_runtime_error_type;
        }
        catch (std::exception const &)
        {
            type_caught = std_exception_type;
        }

        CHECK(type_caught == std_exception_type);
    }

    SECTION("we can catch a runtime_error as the std::runtime_error type")
    {
        CHECK_THROWS_AS(
            throw runtime_error(U"an error occurred"),
            std::runtime_error
        );

    }

    SECTION("a runtime_error is not a std::logic_error")
    {
        try
        {
            throw runtime_error(U"an error occurred");
        }
        catch (std::logic_error const &)
        {
            type_caught = std_logic_error_type;
        }
        catch (std::exception const &)
        {
            type_caught = std_exception_type;
        }

        CHECK(type_caught == std_exception_type);
    }
}
