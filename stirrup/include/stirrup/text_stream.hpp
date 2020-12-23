#pragma once

#include "stirrup/stream.hpp"

#include <string>

namespace stirrup
{

class text_stream
{
public:
    explicit text_stream(stream & binary_stream_source);

    std::u32string read(std::size_t read_size);
    void write(std::u32string const &str);
    void flush();

private:
    std::string cook(std::string const & new_block);
    void in_start_state(char c, std::string & output);
    void in_cr_detected_state(char c, std::string & output);

    stream & binary_stream_source_;

    enum class state
    {
        START,
        CR_DETECTED
    } state_;
};

}
