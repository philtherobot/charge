#include <catch2/catch.hpp>

#include "stirrup/format.hpp"
#include "stirrup/test/string_string_maker.hpp"

using namespace stirrup;

TEST_CASE("formatting values to string")
{
    SECTION("format")
    {
        CHECK(fmt::format(U"plain") == U"plain");
        CHECK(fmt::format(U"an integer {}", 21) == U"an integer 21");
        CHECK(fmt::format(U"a string {}", U"value") == U"a string value");
        CHECK(fmt::format(U"named argument {user}", fmt::arg(U"user", U"Philippe")) == U"named argument Philippe");
        CHECK(fmt::format(U"padding -{:10}-", U"hello \u503c") == U"padding -hello \u503c   -");
    }
}

struct point
{
    double x{};
    double y{};
};

template <>
struct fmt::formatter<point, char32_t>
{

    constexpr auto parse(basic_format_parse_context<char32_t> & ctx)
    {
        return ctx.end();
    }

    template <typename FormatContext>
    auto format(const point & p, FormatContext & ctx)
    {
        return format_to(
            ctx.out(),
            U"({:.1f}, {:.1f})",
            p.x,
            p.y
        );
    }
};

TEST_CASE("user types formatting")
{
    SECTION("user types formatting")
    {
        point p{2,3};
        CHECK(fmt::format(U"a point = {}", p) == U"a point = (2.0, 3.0)");
    }
}
