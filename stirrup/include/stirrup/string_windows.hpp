#pragma once

#include "stirrup/error.hpp"

#include <windows.h>

#include <locale>
#include <vector>

namespace stirrup::windows
{

UINT codepage_from_locale(std::locale const & locale);

class cannot_understand_codepage_error : public runtime_error
{
public:
    explicit cannot_understand_codepage_error(std::u32string const &locale_name);
    std::u32string const locale_name_;
};

std::u32string decode_string(std::vector<WCHAR> const & encoded_string);
std::vector<WCHAR> encode_string(std::u32string const & string);

}
