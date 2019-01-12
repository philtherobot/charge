
#include "process.hpp"

#include "tools.hpp"

#include <charge/exception.hpp>
#include <charge/types.hpp>

#include <boost/range/algorithm/transform.hpp>

#include <iterator>
#include <string>

#include <unistd.h>
#include <sys/wait.h>


namespace charge
{

namespace 
{

pid_t fork()
{
    pid_t child = ::fork();

    if (child == -1)
    {
        throw RuntimeError(make_errno_message("fork", errno));
    }

    return child;
}


int waitpid(pid_t child)
{
    int status = 0;
    pid_t wait_returned = ::waitpid(child, &status, 0);

    if (wait_returned == -1)
    {
        throw RuntimeError(make_errno_message("waitpid", errno));
    }

    return status;
}


void execve(char const * filename, char * const argv[], char * const envp[])
{
    ::execve(filename, argv, envp);

    // It is an error to get here, execve does not return.
    throw RuntimeError(make_errno_message("execve", errno));
}


using CharArray = std::vector<char>;


CharArray vector_from_string(std::string const & s)
{
    CharArray a;
    a.resize(s.size()+1);
    std::copy(s.begin(), s.end(), a.begin());
    return a;
}


std::vector<CharArray> make_execve_arguments(std::string const & pgm, StringList const & args)
{
    std::vector<CharArray> r;

    r.emplace_back( vector_from_string(pgm) );

    boost::range::transform( args, std::back_inserter(r), vector_from_string );

    return r;
}


std::vector<char*> make_charp_vector(std::vector<CharArray> & char_array_vector)
{
    std::vector<char*> r;

    for(CharArray & a: char_array_vector)
    {
        char * p = a.data();
        r.emplace_back(p);
    }

    r.emplace_back( nullptr );

    return r;
}


void exec_forked_child(std::string const & pgm, StringList const & args)
{
    auto execve_args = make_execve_arguments(pgm, args);

    auto v = make_charp_vector(execve_args);

    charge::execve(pgm.c_str(), v.data(), environ);
    // No return here.
}

 
int exec_forked_parent(pid_t child)
{
    // In the parent process.  Monitor child.
    int status = charge::waitpid(child);

    if ( !WIFEXITED(status) )
    {
        throw RuntimeError("waitpid returned for unexpected reason");
    }

    return WEXITSTATUS(status);
}


} // anonymous


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
    pid_t child = charge::fork();

    if (child == 0)
    {
        exec_forked_child(pgm, args);
        // Not expecting any return from the function above.
    }

    return exec_forked_parent(child);
}


std::string write_command_string(std::string const & pgm, StringList const & args)
{
    // TODO: this looks like it is platform-specific.
    // See if we can make it platform-specific
    return std::string();
}


} // charge
