#pragma once

#include <deque>

#include "stirrup/readable_stream.hpp"

namespace stirrup
{

class fake_readable_stream: public readable_stream
{
public:

    void queue_read_result(std::string const & read_result);

    std::string read() override;

private:
    std::deque<std::string> read_results_;
};

}
