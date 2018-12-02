#ifndef GUARD_5ea4ecbeaba3491a899bb905eb7cd8de
#define GUARD_5ea4ecbeaba3491a899bb905eb7cd8de

#include <boost/filesystem/path.hpp>

#include <string>

namespace charge
{
	
std::string getenv(std::string const & var);

std::string hostname();

boost::filesystem::path home_path();

void check_absolute(boost::filesystem::path const & p);

std::string quote(std::string const & str);
std::string quote_if_needed(std::string const & str);

} // charge

#endif
