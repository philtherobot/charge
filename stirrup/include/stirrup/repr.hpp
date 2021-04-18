#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace stirrup
{

std::string repr(char8_t u8_byte);
std::string repr(char32_t unicode_character);

std::string repr(std::u32string const & string);

std::string repr(char32_t const * string);

template <typename value>
std::string repr(value v)
{
    return std::to_string(v);
}

template <typename value>
std::string repr(std::vector<value> const & container)
{
    std::string result = "{";
    unsigned int index = 0;

    std::ranges::for_each(
        container, [&](value const & val)
        {
            if (index++ != 0)
            {
                result += ",";
            }
            result += repr(val);
        }
    );

    result += "}";
    return result;
}
}
