#pragma once

#include <string>

#include "stirrup/ReadableStream.hpp"

namespace stirrup
{

class StreamCookerReader: public ReadableStream
{
public:
    explicit StreamCookerReader(ReadableStream & binary_stream_source);

    virtual std::string read();

private:
    std::string cook(std::string const & new_block);
    void in_start_state(char c, std::string & output);
    void in_cr_detected_state(char c, std::string & output);

    ReadableStream & source_;

    enum class State
    {
        START,
        CR_DETECTED
    } state_;
};

}
