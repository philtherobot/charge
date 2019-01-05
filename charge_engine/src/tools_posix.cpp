
#include "tools.hpp"


namespace charge
{


std::string getenv(std::string const & var)
{
    return std::string();
}


std::string hostname()
{
    return getenv("HOSTNAME");
}


boost::filesystem::path home_path()
{
    return getenv("HOME");
}


} // charge
