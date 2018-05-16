#ifndef GUARD_3e21f362c9b3405c8eb47d6afb878412
#define GUARD_3e21f362c9b3405c8eb47d6afb878412

#include "charge/charge.hpp"

#include <boost/optional.hpp>

#include <cctype>
#include <memory>
#include <stdexcept>
#include <string>

namespace charge
{


class ReadableStream
{
public:
    virtual ~ReadableStream() {}
    virtual boost::optional<std::string> read() = 0;
};


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
std::string write_arguments_string(StringList const & args);

} // charge

#endif
