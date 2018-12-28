#ifndef GUARD_3e21f362c9b3405c8eb47d6afb878412
#define GUARD_3e21f362c9b3405c8eb47d6afb878412

#include "ReadableStream.hpp"

#include <charge/types.hpp>

#include <memory>
#include <string>

namespace charge
{


class ShellProcess
{
public:
    ShellProcess();
    ~ShellProcess();

    void start(std::string const & shell_command);

    int exit_code();

    std::shared_ptr<ReadableStream> child_stdout_;

private:
    class Implementation;
    std::unique_ptr<Implementation> impl_;
};


int exec(std::string const & pgm, StringList const & args);


// TODO: if this turns out to be windows-only, remove from here
std::string write_command_string(std::string const & pgm, StringList const & args);

} // charge

#endif
