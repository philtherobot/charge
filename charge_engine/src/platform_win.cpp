
#include "platform.hpp"

namespace charge
{

namespace platform
{


std::string complete_executable_filename(std::string const & fn)
{
    return fn + ".exe";
}


StringList compiler_detection_order()
{
    return {
        "msvc",
        "gcc"
    };
}


} // platform

} // charge
