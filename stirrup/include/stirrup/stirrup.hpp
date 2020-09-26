#pragma once

#include <string>

namespace stirrup
{

std::string getenv(std::string const & variable_name);

std::string make_errno_message(std::string const & from_function, int err);

}
