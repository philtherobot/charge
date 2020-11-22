#pragma once

#include <string>

namespace stirrup
{
std::string make_errno_message(std::string const & from_function, int err);
}
