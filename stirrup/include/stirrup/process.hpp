#pragma once

#include "stirrup/readable_stream.hpp"
#include "stirrup/string.hpp"

#include <memory>
#include <string>

namespace stirrup
{

class process
{
public:
    virtual ~process() = default;
    virtual void start(std::string const & pgm, string_list const & args) = 0;
    virtual std::shared_ptr<readable_stream> start_capture_output(std::string const & pgm,
        string_list const & args
    ) = 0;
    virtual int wait_for_exit() = 0;
};

class system_process: public process
{
public:
    system_process();
    ~system_process() override; // destructor required because it cannot be generated until class implementation is defined

    void start(std::string const & pgm, string_list const & args) override;
    std::shared_ptr<readable_stream> start_capture_output(std::string const & pgm, string_list const & args) override;
    int wait_for_exit() override;

private:
    class implementation;
    std::unique_ptr<implementation> impl_;
};

}
