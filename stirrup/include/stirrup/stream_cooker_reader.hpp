#pragma once

#include <string>

#include "stirrup/readable_stream.hpp"

namespace stirrup
{

class stream_cooker_reader: public readable_stream
{
public:
    explicit stream_cooker_reader(readable_stream & binary_stream_source);

    std::string read() override;

private:
    std::string cook(std::string const & new_block);
    void in_start_state(char c, std::string & output);
    void in_cr_detected_state(char c, std::string & output);

    readable_stream & source_;

    enum class state
    {
        START,
        CR_DETECTED
    } state_;
};

}
