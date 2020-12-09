#include <catch2/catch.hpp>

#include "stirrup/error.hpp"

using namespace stirrup;
using std::string;

SCENARIO("error: make_errno_message")
{
    CHECK(make_errno_message("getthis", EINVAL) == "system (errno) error 22 (Invalid argument) in function getthis");
}

SCENARIO("error: exception")
{
    exception e(U"message to the \u503C");

    CHECK(e.message() == U"message to the \u503C");
    CHECK(e.what() == string("message to the \\u503C"));
}
