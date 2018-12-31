#ifndef GUARD_4ce3b1e47368434ca82642f9597e5762
#define GUARD_4ce3b1e47368434ca82642f9597e5762

#include <charge/types.hpp>

#include <string>

namespace charge
{

namespace platform
{

std::string complete_executable_filename(std::string const & fn);
StringList compiler_detection_order();

} // platform


} // charge

#endif
