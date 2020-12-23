
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

std::wstring transcode_wstring_from(char const *str, std::locale const &locale)
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
    if(c32 > std::numeric_limits<wchar_t>::max())
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

u32string transcode_from_utf8(vector<char8_t> const & utf8_string)
{
    u32string result;
    result.reserve(utf8_string.size());

    mbstate_t state{};

    char const * read_position = reinterpret_cast<char const *>(utf8_string.data());
    size_t left_count = utf8_string.size();

    while (left_count > 0)
    {
        char32_t c{};
        size_t read_count = mbrtoc32(&c, read_position, left_count, &state);

        switch (read_count)
        {
            case 0: return result;
            case size_t(-3):
            {
                result.push_back(c);
                break;
            }
            case size_t(-2): throw logic_error(U"incomplete UTF-8 multi-bytes character");
            case size_t(-1): throw runtime_error(U"error converting from UTF-8");

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

vector<char8_t> transcode_to_utf8(u32string const & unicode_string)
{
    vector<char8_t> result;
    result.reserve(unicode_string.size());

    std::mbstate_t state{};

    for (char32_t const c: unicode_string)
    {
        auto const insertion_index = result.size();
        size_t const max_utf8_length = 6;
        result.resize(result.size() + max_utf8_length);

        char * insertion = reinterpret_cast<char *>(result.data() + insertion_index);

        // This works for us because MSVC has a bug that works in our favor.
        // We want conversion to UTF-8.  c32rtomb should convert to the locale's
        // encoding, but it does not: it always does UTF-8.
        // I will need to eventually fix this as this is not portable.
        auto const nb_bytes_output = c32rtomb(insertion, c, &state);
        result.resize(insertion_index + nb_bytes_output);
    }

    return result;
}

std::u32string transcode_from_wstring(std::wstring const &str)
{
    std::u32string result;
    result.resize(str.size());
    std::transform(std::begin(str), std::end(str), std::begin(result), [](wchar_t wc) { return char32_t(wc); });
    return result;
}

std::wstring transcode_to_wstring(std::u32string const &str)
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
