#ifndef GUARD_5ea4ecbeaba3491a899bb905eb7cd8de
#define GUARD_5ea4ecbeaba3491a899bb905eb7cd8de

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>

#include <string>
#include <vector>

namespace charge
{

using FileList = std::vector<boost::filesystem::path>;

std::string getenv(std::string const & var);

std::string hostname();

boost::filesystem::path home_path();

void check_absolute(boost::filesystem::path const & p);

std::string quote(std::string const & str);
std::string quote_if_needed(std::string const & str);

boost::optional<std::string> consume_line(std::string & buf_inout);

} // charge

#endif
