#pragma once

#include <algorithm>
#include <locale>
#include <string>
#include <vector>

namespace stirrup
{

using string_list = std::vector<std::string>;
std::string quote(std::string const & str);
std::string quote_if_needed(std::string const & str);

std::string to_narrow_string(std::u8string const & str);
std::wstring to_wide_string(std::u8string const & str);

std::u8string to_u8string(std::string const & str);
std::u8string to_u8string(std::wstring const & str);

std::u32string transcode_from_utf8(std::vector<char8_t> const & utf8_string);
std::vector<char8_t> transcode_to_utf8(std::u32string const & string);

std::u32string transcode_from_wstring(std::wstring const & str);
std::wstring transcode_to_wstring(std::u32string const & str);

// todo-php: first+last or range templated implementation for transcode_from_locale
// it would be useful to implement file::read for example where the data is in an array without a term-zero
std::u32string transcode_from_locale(char const * str, std::locale const & locale = std::locale());

std::u32string decode_string(std::vector<char> const & encoded_string, std::locale const & locale);
std::vector<char> encode_string(std::u32string const & string, std::locale const & locale);

std::u32string convert_from_ascii(std::string const & plain_ascii_string);
std::string convert_to_ascii(std::u32string const & string);

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
