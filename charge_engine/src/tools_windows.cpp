
#include "tools.hpp"

#include <charge/charge.hpp>

#include <stdlib.h>

namespace charge
{


std::string getenv(std::string const & var)
{
    char * buf = 0;
    int err = _dupenv_s(&buf, NULL, var.c_str());
    if (err != 0)
    {
        throw RuntimeError(make_errno_message("_dupenv_s", err));
    }

    if (!buf) throw Exception("failed to find environment variable " + var);

    std::string r{ buf };
    free(buf);

    return r;
}


std::string hostname()
{
    return getenv("COMPUTERNAME");
}


boost::filesystem::path home_path()
{
    return getenv("USERPROFILE");
}


} // charge
