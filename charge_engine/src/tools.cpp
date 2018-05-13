
#include "exception.hpp"
#include "tools.hpp"


namespace charge
{


void check_absolute(boost::filesystem::path const & p)
{
	if (!p.is_absolute())
	{
		throw AbsolutePathError(p);
	}
}

} // charge
