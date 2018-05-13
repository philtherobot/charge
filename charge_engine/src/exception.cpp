
#include "exception.hpp"

#include <sstream>

namespace charge
{


ErrnoError::ErrnoError(std::string const & function, int code)
	:
	Exception(msg(function, code))
{}


std::string ErrnoError::msg(std::string const & function, int code)
{
	std::ostringstream os;
	os << "system (errno) error " << code;
	os << " in function " << function;
	return os.str();
}

} // charge
