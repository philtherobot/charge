#pragma once

#include <string>
#include <vector>

namespace stirrup
{

std::string getenv(std::string const & variable_name);

std::string make_errno_message(std::string const & from_function, int err);

std::string quote_if_needed(std::string const & str);

using StringList = std::vector<std::string>;

}


#include "stirrup/process.hpp"
