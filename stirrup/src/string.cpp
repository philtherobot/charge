
#include "stirrup/string.hpp"

#include <boost/range/algorithm/find_if.hpp>

#include <cctype>


namespace stirrup
{

using std::string;
using std::u8string;

string quote(string const & str)
{
    return string("\"") + str + string("\"");
}

string quote_if_needed(string const & str)
{
    auto isspace = [](unsigned char c)
    { return std::isspace(c); };

    if (boost::range::find_if(str, isspace) != str.end())
    {
        return quote(str);
    }

    return str;
}

std::u8string to_u8string(std::string const & str)
{
    return reinterpret_cast<char8_t const *>(str.data());
}

}
