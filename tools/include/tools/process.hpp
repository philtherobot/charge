#ifndef GUARD_3e21f362c9b3405c8eb47d6afb878412
#define GUARD_3e21f362c9b3405c8eb47d6afb878412

#include <boost/optional.hpp>

#include <memory>
#include <stdexcept>
#include <string>

namespace tools
{


class ReadableStream
{
public:
    virtual ~ReadableStream() {}
    virtual boost::optional<std::string> read() = 0;
};


class Process
{
public:
    Process();
    ~Process();

    void start(std::string const & shell_command);

    int exit_code();

    std::shared_ptr<ReadableStream> child_stdout_;

private:
    class Implementation;
    std::unique_ptr<Implementation> impl_;
};


class Exception : public std::runtime_error
{
public:
	using runtime_error::runtime_error;
};


} // tools

#endif
