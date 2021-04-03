
#include "stirrup/string.hpp"

#include "stirrup/error.hpp"
#include "stirrup/locale.hpp"

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

std::wstring transcode_wstring_from(char const * str, std::locale const & locale)
{
    locale_block set_and_restore_locale(locale);

    auto const str_size = std::strlen(str);

    std::wstring result;
    result.reserve(str_size);

    mbstate_t state{};

    char const * read_position = str;
    size_t left_count = str_size;

    while (left_count > 0)
    {
        wchar_t c{};
        size_t read_count = mbrtowc(&c, read_position, left_count, &state);

        switch (read_count)
        {
            case 0: return result;
            case size_t(-3):
            {
                result.push_back(c);
                break;
            }
            case size_t(-2): throw logic_error(U"incomplete multi-bytes character");
            case size_t(-1): throw runtime_error(U"error converting from locale encoding");

            default:
            {
                result.push_back(c);
                read_position += read_count;
                left_count -= read_count;
                break;
            }
        }
    }

    return result;
}

wchar_t c32towc(char32_t c32)
{
    if (c32 > std::numeric_limits<wchar_t>::max())
    {
        throw runtime_error(U"symbol cannot be converted");
    }

    return wchar_t(c32);
}

}

string quote(string const & str)
{
    return string("\"") + str + string("\"");
}

string quote_if_needed(string const & str)
{
    auto isspace = [](unsigned char c)
    { return std::isspace(c); };

    if (boost::range::find_if(str, isspace) != str.end())
    {
        return quote(str);
    }

    return str;
}

std::u8string to_u8string(std::string const & str)
{
    return reinterpret_cast<char8_t const *>(str.data());
}

std::u32string transcode_from_wstring(std::wstring const & str)
{
    std::u32string result;
    result.resize(str.size());

    std::transform(
        std::begin(str), std::end(str), std::begin(result), [](wchar_t wc)
        { return char32_t(wc); }
    );

    return result;
}

std::wstring transcode_to_wstring(std::u32string const & str)
{
    std::wstring result;
    result.resize(str.size());
    std::transform(std::begin(str), std::end(str), std::begin(result), c32towc);
    return result;
}

std::u32string transcode_from_locale(char const * str, std::locale const & locale)
{
    return transcode_from_wstring(transcode_wstring_from(str, locale));
}

// todo-php: the convert_from_ascii function is a little particular: what should we do?
// I do not think it is of value to the public.
u32string convert_from_ascii(string const & plain_ascii_string)
{
    u32string result(plain_ascii_string.size(), U' ');

    std::ranges::transform(
        plain_ascii_string,
        begin(result),
        [](char c)
        {
            if (c < 0 || c > 127)
            {
                // todo-php: should convert_string throw if there is a non-ASCII character?
                throw runtime_error(U"convert_from_ascii: input string is not plain ASCII");
            }

            return char32_t(c);
        }
    );

    return result;
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

