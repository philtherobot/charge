#include "stirrup/test/FakeReadableStream.hpp"

namespace stirrup
{

void FakeReadableStream::queueReadResult(std::string const & read_result)
{
    read_results_.push_back(read_result);
}

std::string FakeReadableStream::read()
{
    if (read_results_.empty() ) return {};

    auto const next_result = read_results_.front();
    read_results_.pop_front();

    return next_result;
}

}
