
#ifndef GUARD_135e65fec96c429eac6e09ed5d344f3e
#define GUARD_135e65fec96c429eac6e09ed5d344f3e

#include "tools.hpp"

#include <boost/filesystem/path.hpp>

#include <iostream>
#include <string>
#include <vector>

namespace charge
{

FileList decode_header_dependencies(std::string const & deps_text);
std::string encode_header_dependencies(FileList const & deps);

} // charge


namespace std
{

std::ostream & operator << (std::ostream & os, charge::FileList const & ss);

}

#endif
