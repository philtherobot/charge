#include <filesystem>
#include <vector>
#include <stdio.h>

namespace stirrup
{

class file
{
public:
    explicit file(FILE * file);
    ~file();

    std::vector<char> read(std::size_t read_size);
    void write(std::vector<char> const & buffer);
    void flush();

private:
    FILE * file_{};
};

file create_file(std::filesystem::path const & new_file_path);
file open_file(std::filesystem::path const & existing_file_path);

}
