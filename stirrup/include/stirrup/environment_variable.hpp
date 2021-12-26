#pragma once

#include <string>

namespace stirrup
{
std::u32string getenv(std::u32string const & variable_name);
void putenv(std::u32string const &variable_name, std::u32string const &value);
}
