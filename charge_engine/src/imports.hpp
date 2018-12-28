#ifndef GUARD_c6d8fe9db27948adad1c35063817ba50
#define GUARD_c6d8fe9db27948adad1c35063817ba50

#include "config.hpp"

#include <charge/types.hpp>

#include <iostream>

namespace charge
{

class Libraries
{
public:
	StringList header_paths_;
	StringList static_;
	StringList system_;
	StringList lib_paths_;
};


Libraries find_imports(Config const & config, std::istream & is);

} // charge


namespace std
{
std::ostream & operator << (std::ostream & os, charge::Libraries const & libs);
}


#endif
