#include "stirrup/stream.hpp"

#include <algorithm>
#include <array>
#include <iterator>

using std::begin;
using std::end;
using std::filesystem::path;
using std::size_t;
using std::vector;

namespace stirrup
{

input_stream::input_stream(const vector<char> & buffer)
: buffer_(buffer), position_(begin(buffer_))
{}

vector<char> input_stream::read(size_t read_size)
{
    auto const remaining_chars_size = std::distance(position_, end(buffer_));
    auto const actual_read_size = std::min(read_size, size_t(remaining_chars_size));
    auto end_read_block = position_ + actual_read_size;
    vector<char> result;
    result.resize(actual_read_size);
    std::copy(position_, end_read_block, begin(result));
    position_ = end_read_block;
    return result;
}

output_stream::output_stream(std::vector<char> & buffer)
: buffer_(buffer)
{}

void output_stream::write(vector<char> const & new_data)
{
    std::copy(begin(new_data), end(new_data), back_inserter(buffer_));
}

input_stream create_memory_input_stream(vector<char> const &buffer)
{
    return input_stream(buffer);
}

output_stream create_memory_output_stream(vector<char> &buffer)
{
    return output_stream(buffer);
}

}
