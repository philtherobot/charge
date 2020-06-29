#include "stirrup/stirrup.hpp"

#include <cwchar>
#include <stdexcept>

using namespace std;

namespace stirrup
{

str::str(char const * system_encoded_array)
    : str(reinterpret_cast<str_char const *>(system_encoded_array))
{

}

bool str::operator==(char const * system_encoded_array) const
{
    return *this ==  reinterpret_cast<str_char const *>(system_encoded_array);
}

std::ostream & operator<<(std::ostream & os, str const & s)
{
    // todo-php: convert to system from UTF-8
    os << reinterpret_cast<char const *>(s.c_str());
    return os;
}

}
