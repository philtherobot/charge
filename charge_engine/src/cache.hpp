#ifndef GUARD_c32a67b8d56e4694b3957351dd917844
#define GUARD_c32a67b8d56e4694b3957351dd917844

#include <boost/filesystem.hpp>

namespace charge
{

boost::filesystem::path get_cache_path(
	std::string const & hostname,
	boost::filesystem::path const & home,
	boost::filesystem::path const & script,

	boost::filesystem::path const & current_path = 
		boost::filesystem::current_path()
);

} // charge

#endif
