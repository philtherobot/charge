
#include "stirrup/string.hpp"

#include <algorithm>
#include <boost/range/algorithm/find_if.hpp>
#include <cctype>
#include <cuchar>
#include <fmt/format.h>

namespace stirrup
{

using std::begin;
using std::size_t;
using std::string;
using std::u32string;
using std::u8string;
using std::vector;

namespace
{

std::string repr_to_hex(char8_t value)
{
    return fmt::format("\\x{:02X}", uint_least8_t(value));
}

std::string repr_to_hex(char16_t value)
{
    return fmt::format("\\u{:04X}", uint_least16_t(value));
}

std::string repr_to_hex(char32_t value)
{
    return fmt::format("\\U{:08X}", uint_least32_t(value));
}

}

std::string repr(char32_t unicode_character)
{
    if (unicode_character < 128)
    {
        char r[2] = {};
        r[0] = char(unicode_character);
        return r;
    }

    if (unicode_character < 0x10000)
    {
        return repr_to_hex(char16_t(unicode_character));
    }

    return repr_to_hex(unicode_character);
}

std::string repr(u32string const & string)
{
    std::string result;

    for (char32_t character: string)
    {
        result += repr(character);
    }

    return result;
}

std::string repr(char32_t const * string)
{
    return repr(u32string(string));
}

std::string repr(char8_t u8_byte)
{
    if (u8_byte < 128)
    {
        char r[2] = {};
        r[0] = char(u8_byte);
        return r;
    }

    return repr_to_hex(char8_t(u8_byte));
}

}

