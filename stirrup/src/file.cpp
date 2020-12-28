#include "stirrup/file.hpp"

#include <array>

using std::vector;

namespace stirrup
{

file::file(FILE * file)
    : file_(file)
{}

file::file(file && other)
{
    move(std::move(other));
}

file::~file()
{
    close();
}

file & file::operator=(file && other)
{
    move(std::move(other));
    return *this;
}

vector<char> file::read(std::size_t user_read_size)
{
    vector<char> result;

    std::array<char, 16 * 1024> buffer;

    auto const actual_read_size = std::min(user_read_size, buffer.size());

    // todo-php: we have to test and implement reading more than the buffer size.
    std::size_t read_count = std::fread(buffer.data(), sizeof(char), actual_read_size, file_);
    result.resize(read_count);
    std::copy(begin(buffer), begin(buffer) + read_count, begin(result));

    return result;
}

void file::write(vector<char> const & buffer)
{
    std::fwrite(buffer.data(), sizeof(char), buffer.size(), file_);
}

void file::flush()
{
    std::fflush(file_);
}

void file::close()
{
    if (file_)
    {
        std::fclose(file_);
        file_ = nullptr;
    }
}

input_stream file::input_stream()
{
    return stirrup::input_stream(std::make_unique<input_device>(*this));
}

output_stream file::output_stream()
{
    return stirrup::output_stream(std::make_unique<output_device>(*this));
}

void file::move(file && other)
{
    close();
    file_ = other.file_;
    other.file_ = nullptr;
}

file::input_device::input_device(file & input_file)
    : file_(input_file)
{}

vector<char> file::input_device::read(std::size_t read_size)
{
    return file_.read(read_size);
}

file::output_device::output_device(file & output_file)
    : file_(output_file)
{}

void file::output_device::write(const vector<char> & new_data)
{
    file_.write(new_data);
}

void file::output_device::flush()
{
    file_.flush();
}

}
