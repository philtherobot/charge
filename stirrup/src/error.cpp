#include "stirrup/error.hpp"

#include "stirrup/string.hpp"

#include <cstring>
#include <sstream>

using namespace std;

namespace stirrup
{

string make_errno_message(string const & from_function, int err)
{
    ostringstream os;
    os << "system (errno) error " << err;
    os << " (" << strerror(err) << ")";
    os << " in function " << from_function;
    return os.str();
}

exception::exception(const std::u32string & message) : std::exception(repr(message).c_str()), message_(message)
{}

std::u32string exception::message() const
{
    return message_;
}

}
