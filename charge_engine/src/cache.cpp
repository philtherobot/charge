
#include "cache.hpp"

#include "tools.hpp"

#include <boost/filesystem/operations.hpp>

#include <sstream>

namespace charge
{


boost::filesystem::path get_cache_path(
	std::string const & hostname,
	boost::filesystem::path const & home,
	boost::filesystem::path const & script,

	boost::filesystem::path const & current_path)
{
	auto cache_root = home / ".charge" / "cache";

	auto subdir{ boost::filesystem::absolute(script, current_path) };

	std::string uniq{ hostname + subdir.string() };

	std::ostringstream os;
	os << std::hash<std::string>{}(uniq);

	return cache_root / os.str();
}


} // charge
