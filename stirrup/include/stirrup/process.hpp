#pragma once

#include "stirrup/ReadableStream.hpp"
#include "stirrup/string.hpp"

#include <memory>
#include <string>

namespace stirrup
{

class Process {
public:
    virtual void start(std::string const & pgm, string_list const & args) = 0;
    virtual std::shared_ptr<ReadableStream> start_capture_output(std::string const & pgm, string_list const & args) = 0;
    virtual int wait_for_exit_code() = 0;
};

class SystemProcess : public Process
{
public:
    SystemProcess();
    ~SystemProcess();

    virtual void start(std::string const & pgm, string_list const & args);
    virtual std::shared_ptr<ReadableStream> start_capture_output(std::string const & pgm, string_list const & args);
    virtual int wait_for_exit_code();

private:
    class Implementation;
    std::unique_ptr<Implementation> impl_;
};

}
