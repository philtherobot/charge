#pragma once

#include <deque>

#include "stirrup/ReadableStream.hpp"

namespace stirrup
{

class FakeReadableStream : public ReadableStream
{
public:

    void queueReadResult(std::string const & read_result);

    virtual std::string read() override;

private:
    std::deque<std::string> read_results_;
};

}
