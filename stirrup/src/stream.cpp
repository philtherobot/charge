#include "stirrup/stream.hpp"

#include <algorithm>
#include <array>
#include <iterator>

using std::begin;
using std::end;
using std::move;
using std::size_t;
using std::vector;

namespace stirrup
{

input_stream::input_stream(std::unique_ptr<device> && input_device)
    :
    device_(move(input_device))
{}

vector<char> input_stream::read(size_t read_size)
{
    return device_->read(read_size);
}

output_stream::output_stream(std::unique_ptr<device> && output_device)
    :
    device_(move(output_device))
{}

void output_stream::write(const std::vector<char> & new_data)
{
    device_->write(new_data);
}

memory_input_device::memory_input_device(const vector<char> & buffer)
    : buffer_(buffer), read_position_(begin(buffer_))
{}

vector<char> memory_input_device::read(size_t read_size)
{
    auto const remaining_chars_size = std::distance(read_position_, end(buffer_));
    auto const actual_read_size = std::min(read_size, size_t(remaining_chars_size));
    auto end_read_block = read_position_ + actual_read_size;
    vector<char> result;
    std::copy(read_position_, end_read_block, back_inserter(result));
    read_position_ = end_read_block;
    return result;
}

input_stream create_memory_input_stream(vector<char> const & buffer)
{
    return input_stream(std::make_unique<memory_input_device>(buffer));
}

memory_output_device::memory_output_device(std::vector<char> & buffer)
    :
    buffer_(buffer)
{}

void memory_output_device::write(vector<char> const & new_data)
{
    std::copy(begin(new_data), end(new_data), back_inserter(buffer_));
}

output_stream create_memory_output_stream(vector<char> & buffer)
{
    return output_stream(std::make_unique<memory_output_device>(buffer));
}

}
