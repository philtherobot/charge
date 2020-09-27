
#include "stirrup/stirrup.hpp"

#include <boost/range/algorithm/find_if.hpp>

#include <cctype>


namespace stirrup
{

std::string quote(std::string const & str)
{
    return std::string("\"") + str + std::string("\"");
}

std::string quote_if_needed(std::string const & str)
{
    auto isspace = [](unsigned char c){ return std::isspace(c); };

    if (boost::range::find_if(str, isspace) != str.end())
    {
        return quote(str);
    }

    return str;
}

}
