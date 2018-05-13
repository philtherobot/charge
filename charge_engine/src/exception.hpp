
#ifndef GUARD_03dd4ca8d56a42ce9be6f7e6c5d9657d
#define GUARD_03dd4ca8d56a42ce9be6f7e6c5d9657d


#include "charge/charge.hpp"


namespace charge
{


class ErrnoError : public Exception
{
public:
	explicit ErrnoError(std::string const & function, int code = 0);

private:
	static std::string msg(std::string const & function, int code);
};


class AbsolutePathError : public Exception
{
public:
	explicit AbsolutePathError(boost::filesystem::path const & p);

private:
	static std::string msg(boost::filesystem::path const & p);
};


}


#endif

