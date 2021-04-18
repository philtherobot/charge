#include "stirrup/file.hpp"

#include "stirrup/string.hpp"

#include <array>

using std::u32string;
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

u32string file::read(std::size_t user_read_size)
{

    std::array<char, 16 * 1024> buffer;

    auto const actual_read_size = std::min(user_read_size, buffer.size());

    // todo-php: we have to test and implement reading more than the buffer size.
    std::size_t read_count = std::fread(buffer.data(), sizeof(char), actual_read_size, file_);

    vector<char> vec;

    std::copy(
        begin(buffer),
        begin(buffer) + read_count,
        back_inserter(vec)
    );

 // todo-php: bad!  we could have read up to half a utf8 codepoint (like the first code unit of three)
 // decoding will fail.
    return decode_string(vec, locale_);
}

void file::write(u32string const & string)
{
    vector<char> encoded = encode_string(string, locale_);
    std::fwrite(encoded.data(), sizeof(char), encoded.size(), file_);
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

input_stream file::get_input_stream()
{
    return stirrup::input_stream(std::make_unique<input_device>(*this));
}

output_stream file::get_output_stream()
{
    return stirrup::output_stream(std::make_unique<output_device>(*this));
}

void file::move(file && other)
{
    close();
    file_ = other.file_;
    other.file_ = nullptr;
}

void file::set_locale(const std::locale & locale)
{
    locale_ = locale;
}

file::input_device::input_device(file & input_file)
    : file_(input_file)
{}

u32string file::input_device::read(std::size_t read_size)
{
    return file_.read(read_size);
}

file::output_device::output_device(file & output_file)
    : file_(output_file)
{}

void file::output_device::write(const u32string & new_data)
{
    file_.write(new_data);
}

void file::output_device::flush()
{
    file_.flush();
}

}
