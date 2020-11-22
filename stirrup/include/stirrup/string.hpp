#pragma once

#include <string>
#include <vector>

namespace stirrup
{
using string_list = std::vector<std::string>;
std::string quote(std::string const & str);
std::string quote_if_needed(std::string const & str);
}
