
#include "platform.hpp"

namespace charge
{

namespace platform
{


std::string complete_executable_filename(std::string const & fn)
{
    return fn;
}


StringList compiler_detection_order()
{
    return {
        "gcc",
        "clang"
    };
}


} // platform

} // charge
