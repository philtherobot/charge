#include "stirrup/test/fake_readable_stream.hpp"

namespace stirrup
{

void fake_readable_stream::queue_read_result(std::string const & read_result)
{
    read_results_.push_back(read_result);
}

std::string fake_readable_stream::read()
{
    if (read_results_.empty()) return {};

    auto next_result = read_results_.front();
    read_results_.pop_front();

    return next_result;
}

}
