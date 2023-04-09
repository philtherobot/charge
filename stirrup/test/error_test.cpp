#include <catch2/catch.hpp>

#include "stirrup/error.hpp"
#include "stirrup/test/string_string_maker.hpp"

using namespace stirrup;
using std::string;

TEST_CASE("handling errno-type errors")
{
    SECTION("make_errno_message")
    {
        CHECK(
            make_errno_message(U"getthis", EINVAL) == U"system error (errno) 22 (Invalid argument) in function getthis"
        );
    }

    SECTION("throw_on_errno")
    {
        CHECK_NOTHROW(throw_on_errno(U"getthis", 0));
        CHECK_THROWS_AS(throw_on_errno(U"getthis", EINVAL), stirrup::runtime_error);
    }
}

TEST_CASE("throwing exceptions")
{
    SECTION("basic exception class works")
    {
        exception e(U"message to the \u503C");

        CHECK(e.message() == U"message to the \u503C");
        CHECK(e.what() == string("message to the \\u503C"));

        CHECK_THROWS_AS(
            throw exception(U"an error occurred"),
            std::exception
        );
    }

    SECTION("we can catch a logic_error as the exception type")
    {
        CHECK_THROWS_AS(
            throw logic_error(U"an error occurred"),
            exception
        );
    }

    enum
    {
        runtime_error_type, logic_error_type, exception_type
    } type_caught{};

    SECTION("a logic_error is not a runtime_error")
    {
        try
        {
            throw logic_error(U"an error occurred");
        }
        catch (runtime_error const &)
        {
            type_caught = runtime_error_type;
        }
        catch (exception const &)
        {
            type_caught = exception_type;
        }

        CHECK(type_caught == exception_type);
    }

    SECTION("we can catch a runtime_error as the exception type")
    {
        CHECK_THROWS_AS(
            throw runtime_error(U"an error occurred"),
            exception
        );

    }

    SECTION("a runtime_error is not a logic_error")
    {
        try
        {
            throw runtime_error(U"an error occurred");
        }
        catch (logic_error const &)
        {
            type_caught = logic_error_type;
        }
        catch (exception const &)
        {
            type_caught = exception_type;
        }

        CHECK(type_caught == exception_type);
    }
}
