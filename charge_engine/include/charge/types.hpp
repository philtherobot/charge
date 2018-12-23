#ifndef GUARD_55917fb0b80b42f299e50009f5cdc8a9
#define GUARD_55917fb0b80b42f299e50009f5cdc8a9

#include <iostream>
#include <string>
#include <vector>


namespace charge
{
using StringList = std::vector<std::string>;
}

namespace std
{
std::ostream & operator << (std::ostream & os, charge::StringList const & ss);
}

#endif
