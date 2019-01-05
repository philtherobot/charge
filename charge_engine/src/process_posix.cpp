
#include "process.hpp"

#include <charge/types.hpp>

#include <string>


namespace charge
{


class ShellProcess::Implementation
{
public:
    Implementation() {}

    ~Implementation() {}
};


ShellProcess::ShellProcess()
    : impl_(new Implementation())
{}


ShellProcess::~ShellProcess()
{}

void ShellProcess::start(std::string const & shell_command)
{
}


int ShellProcess::exit_code()
{
    return 0;
}


int exec(std::string const & pgm, StringList const & args)
{
    return 0;
}


std::string write_command_string(std::string const & pgm, StringList const & args)
{
    // TODO: this looks like it is platform-specific.
    // See if we can make it platform-specific
    return std::string();
}


} // charge
