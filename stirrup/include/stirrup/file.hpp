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

    // todo-php: file::seek & tell
//    virtual void seek(std::size_t pos) = 0;
//    virtual std::size_t tell() = 0;

private:
    FILE * file_{};
};

file create_new_file(std::filesystem::path const & new_file_path);
file open_file(std::filesystem::path const & existing_file_path);

// todo-php: add a function to overwrite/create a file
// not sure about this design, nor the name, see create_new_file
// could be `create_file(U"path")` and `create_file(U"path", overwrite)`
//file overwrite_file(U"fn");  // never fails

}
