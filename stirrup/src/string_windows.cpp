
#include "stirrup/string.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <windows.h>

namespace stirrup
{

using std::string;
using std::u8string;
using std::wstring;

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

}
