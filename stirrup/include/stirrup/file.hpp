#pragma once

#include "stirrup/stream.hpp"

#include <filesystem>
#include <string>
#include <vector>
#include <stdio.h>

namespace stirrup
{

class file
{
public:
    explicit file(FILE * file);
    file(file && other);
    ~file();

    void set_locale(std::locale const &locale);

    file & operator=(file && other);

    std::u32string read(std::size_t read_size);
    void write(std::u32string const & string);
    void flush();
    void close();

    input_stream get_input_stream();
    output_stream get_output_stream();

    // todo-php: file::seek & tell
//    virtual void seek(std::size_t pos) = 0;
//    virtual std::size_t tell() = 0;

private:
    class input_device: public input_stream::device
    {
    public:
        explicit input_device(file & input_file);
        std::u32string read(std::size_t read_size) override;
    private:
        file & file_;
    };

    class output_device: public output_stream::device
    {
    public:
        explicit output_device(file & output_file);
        void write(std::u32string const & new_data) override;
        void flush() override;
    private:
        file & file_;
    };

    void move(file && other);

    FILE * file_{};
    std::locale locale_;
};

file create_new_file(std::filesystem::path const & new_file_path);
file open_file(std::filesystem::path const & existing_file_path);

// todo-php: add a function to overwrite/create a file
// not sure about this design, nor the name, see create_new_file
// could be `create_file(U"path")` and `create_file(U"path", overwrite)`
//file overwrite_file(U"fn");  // never fails

// todo-php: implement append(file &, u32string const&)

}
