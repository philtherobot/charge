
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


AbsolutePathError::AbsolutePathError(boost::filesystem::path const & p)
	:
	Exception(msg(p))
{}


std::string AbsolutePathError::msg(boost::filesystem::path const & p)
{
	std::ostringstream os;
	os << "path " << p << " is not absolute";
	return os.str();
}


} // charge
