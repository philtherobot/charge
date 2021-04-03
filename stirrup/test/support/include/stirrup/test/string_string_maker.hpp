#include <catch2/catch.hpp>
#include <string>
#include "stirrup/string.hpp"

namespace Catch
{

template <>
struct StringMaker<std::u32string>
{
    static std::string convert(std::u32string const & value)
    {
        return stirrup::repr(value);
    }
};

// The follwing conflicts with catch2 things
/* template <std::size_t N>
struct StringMaker<char32_t [N]>
{
    static std::string convert(char32_t const * value)
    {
        return stirrup::repr(std::u32string(value));
    }
};
 */
template <>
struct StringMaker<char8_t>
{
    static std::string convert(char8_t value)
    {
        return stirrup::repr(value);
    }
};

}
