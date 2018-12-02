
#include "exception.hpp"
#include "tools.hpp"

#include <boost/range/algorithm/find_if.hpp>

#include <cctype>


namespace charge
{

void check_absolute(boost::filesystem::path const & p)
{
	if (!p.is_absolute())
	{
		throw AbsolutePathError(p);
	}
}

std::string quote(std::string const & str)
{
	return std::string("\"") + str + std::string("\"");
}

std::string quote_if_needed(std::string const & str)
{
	if (boost::range::find_if(str, std::isspace) != str.end())
	{
		return quote(str);
	}

	return str;
}

} // charge
