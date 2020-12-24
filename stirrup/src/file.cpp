#include "stirrup/file.hpp"

#include <array>

using std::vector;

namespace stirrup
{

file::file(FILE * file)
    : file_(file)
{

}

file::~file()
{
    if (file_)
    {
        std::fclose(file_);
    }
}

vector<char> file::read(std::size_t read_size)
{
    vector<char> result;

    std::array<char, 16 * 1024> buffer;
    std::size_t read_count = std::fread(buffer.data(), sizeof(char), buffer.size(), file_);
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

}
