#include "stirrup/error.hpp"

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

}
