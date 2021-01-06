
#include "stirrup/string_windows.hpp"

#include "stirrup/format.hpp"
#include "stirrup/string.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <windows.h>

namespace stirrup
{

using namespace windows;

using std::begin;
using std::end;
using std::string;
using std::u8string;
using std::vector;
using std::wstring;
using std::u32string;

namespace
{

int windows_utf8_to_wide(u8string const & str, wchar_t * wide_string_out = nullptr, int wide_string_size = 0)
{
    return MultiByteToWideChar(
        CP_UTF8,
        0,
        reinterpret_cast<char const *>(str.c_str()),
        int(str.size()),
        wide_string_out,
        wide_string_size
    );
}

int windows_wide_to_utf8(const wchar_t * const & str, char8_t * utf8_string_out = nullptr, int utf8_string_size = 0)
{
    return WideCharToMultiByte(
        CP_UTF8,
        0,
        str,
        static_cast<int>(wcslen(str)),
        reinterpret_cast<LPSTR>(utf8_string_out),
        utf8_string_size,
        nullptr,
        nullptr
    );
}

bool is_valid_single_byte_char(wstring::value_type wide_char)
{
    return wide_char >= 0 && wide_char <= 255;
}

string make_to_narrow_error_message(wstring::value_type problematic_wide_char)
{
    std::ostringstream os;
    std::hex(os);
    os.fill('0');
    os << "Character \\u" << std::setw(4) << unsigned int(problematic_wide_char) << " cannot be converted to ASCII.";
    return os.str();
}

void skip_to_dot(std::istringstream & is)
{
    char c = 0;
    do
    {
        is >> c;
    }
    while (is && c != '.');
}

bool is_high_surrogate(WCHAR wc)
{
    return wc >= 0xD800 && wc <= 0xDFFF;
}

char32_t decode_surrogate_pair(WCHAR high, WCHAR low)
{
    char32_t decoded_high = (unsigned int(high) - 0xD800) << 10;
    char32_t decoded_low = unsigned int(low) - 0xDC00;

    return decoded_high + decoded_low + char32_t(0x10000);
}

bool is_supplementary_plane(char32_t c)
{
    return c >= 0x10000;
}

WCHAR high_surrogate(char32_t c)
{
    return ((unsigned int(c) - 0x10000) >> 10) + 0xD800;
}

WCHAR low_surrogate(char32_t c)
{
    return ((unsigned int(c) - 0x10000) & 0x3FF) + 0xDC00;
}

}

string to_narrow_string(const std::u8string & str)
{
    auto const wide_string = to_wide_string(str);
    string result;
    result.reserve(wide_string.size());

    std::ranges::for_each(
        wide_string,
        [&](wstring::value_type wide_char)
        {
            if (!is_valid_single_byte_char(wide_char))
            {
                throw std::runtime_error(make_to_narrow_error_message(wide_char).data());
            }
            result += char(wide_char);
        }
    );

    return result;
}

wstring to_wide_string(u8string const & str)
{
    if (str.empty())
    {
        return std::wstring();
    }

    int size_needed = windows_utf8_to_wide(str);
    std::wstring result(size_needed, 0);
    windows_utf8_to_wide(str, result.data(), size_needed);
    return result;
}

u8string to_u8string(wstring const & str)
{
    int size_needed = windows_wide_to_utf8(str.data());
    u8string result(size_needed, 0);
    windows_wide_to_utf8(str.data(), result.data(), size_needed);
    return result;
}

std::u32string decode_string(std::vector<char> const & encoded_string, std::locale const & locale)
{
    WCHAR buffer[1024];
    DWORD flags = 0;

    int const number_of_decoded_characters =
        MultiByteToWideChar(
            codepage_from_locale(locale),
            flags,
            encoded_string.data(),
            int(encoded_string.size()),
            buffer,
            sizeof(buffer) / sizeof(WCHAR)
        );

    vector<WCHAR> utf_16_string;
    std::copy(buffer, buffer + number_of_decoded_characters, back_inserter(utf_16_string));
    return stirrup::windows::decode_string(utf_16_string);
}

std::vector<char> encode_string(std::u32string const & string, std::locale const & locale)
{
    vector<WCHAR> utf_16_string = stirrup::windows::encode_string(string);

    char buffer[1024];
    DWORD flags = 0;

    int number_of_encoded_characters = WideCharToMultiByte(
        codepage_from_locale(locale),
        flags,
        utf_16_string.data(),
        int(utf_16_string.size()),
        buffer,
        int(std::size(buffer)),
        nullptr,
        nullptr
    );

    vector<char> result;
    result.resize(number_of_encoded_characters);

    std::copy(buffer, buffer + number_of_encoded_characters, std::begin(result));

    return result;
}

namespace windows
{

UINT codepage_from_locale(std::locale const & locale)
{
    auto const name = locale.name();

    if (name.empty()) return GetACP();
    if (name == "C") return 1252;

    std::istringstream is(name);

    skip_to_dot(is);
    UINT number = 0;
    is >> number;
    if (!is)
    {
        is.clear();
        std::string textual_codepage;
        is >> textual_codepage;
        static std::vector<std::string> utf_8_codepages{"utf-8", "UTF-8", "utf8", "UTF8"};
        if (std::ranges::count(utf_8_codepages, textual_codepage) != 0) return 65001;
        throw cannot_understand_codepage_error(convert_from_ascii(name));
    }
    return number;
}

cannot_understand_codepage_error::cannot_understand_codepage_error(std::u32string const & locale_name)
    : runtime_error(fmt::format(U"cannot_understand_codepage_error: \"{}\"", locale_name)), locale_name_(locale_name)
{}

u32string decode_string(vector<WCHAR> const & encoded_string)
{
    u32string result;

    auto encoded_character_it = begin(encoded_string);

    while (encoded_character_it != end(encoded_string))
    {
        if (is_high_surrogate(*encoded_character_it))
        {
            auto low_surrogate_it = encoded_character_it + 1;
            if (low_surrogate_it == end(encoded_string))
            {
                throw runtime_error(U"decode_string: incomplete surrogate pair");
            }

            result.push_back(decode_surrogate_pair(*encoded_character_it, *low_surrogate_it));
            encoded_character_it = low_surrogate_it + 1;
        }
        else
        {
            result.push_back(*encoded_character_it);
            ++encoded_character_it;
        }
    }

    return result;
}

vector<WCHAR> encode_string(u32string const & string)
{
    vector<WCHAR> result;

    std::ranges::for_each(
        string, [&](char32_t c)
        {
            if (is_supplementary_plane(c))
            {
                result.push_back(high_surrogate(c));
                result.push_back(low_surrogate(c));
            }
            else
            {
                result.push_back(WCHAR(c));

            }
        }
    );

    return result;
}

}

}
