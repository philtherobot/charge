
#include <charge/types.hpp>


namespace std
{

std::ostream & operator << (std::ostream & os, charge::StringList const & ss)
{
	os << '(';
	if (ss.size())
	{
		auto set_it = ss.begin();
		os << *set_it;
		while (++set_it != ss.end())
		{
			os << ',' << *set_it;
		}
	}
	os << ')';
	return os;
}

} // std
