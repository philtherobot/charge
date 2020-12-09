#pragma once

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

std::string repr(char8_t u8_byte);
std::string repr(char32_t unicode_character);

std::string repr(std::u32string const & string);

}
