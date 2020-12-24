#include <filesystem>
#include <vector>
#include <stdio.h>

namespace stirrup
{

class input_stream
{
public:
    explicit input_stream(std::vector<char> const &buffer);
    std::vector<char> read(std::size_t read_size);

private:
    std::vector<char> const &buffer_;
    std::vector<char>::const_iterator position_;
};

input_stream create_memory_input_stream(std::vector<char> const & buffer);

class output_stream
{
public:
    explicit output_stream(std::vector<char> &buffer);

    void write(std::vector<char> const & new_data);

private:
    std::vector<char> & buffer_;
};

output_stream create_memory_output_stream(std::vector<char> & buffer);

class stream_device
{
public:
    virtual ~stream_device() = default;
    virtual std::vector<char> read(std::size_t read_size) = 0;
};

class stream
{
public:
    explicit stream(FILE * file);
    ~stream();

    std::vector<char> read(std::size_t read_size);
    void write(std::vector<char> const & buffer);
    void flush();

private:
    FILE * file_{};
};

stream create_file(std::filesystem::path const & new_file_path);
stream open_file(std::filesystem::path const & existing_file_path);


/*
    class stream
    {
    public:
        virtual ~stream() = default;

        // read X chars or up to EOF, return empty string is EOF (device is empty) reading zero is an error
        virtual std::vector<char> read(size_t) = 0;

        virtual void write(std::vector<char> const & buffer) = 0; // writes

        virtual void seek(std::size_t pos) = 0;
        virtual std::size_t tell() = 0;
    };

    std::vector<char> read_all(stream &strm);  // whole file
    void append(stream &strm, std::vector<char> const & buffer);

    class text_stream
    {
    public:
        explicit text_stream(stream &strm, std::locale encoding)

        // read X chars or up to EOF, return empty string is EOF (device is empty) reading zero is an error
        std::u32string read(size_t) = 0;

        void write(std::u32string const & text);

        virtual void seek(std::size_t pos) = 0;
        virtual std::size_t tell() = 0;
    };



    std::u32string read_all(text_stream &strm);  // whole file
    void append(stream &dev, std::u32string const &text);

    std::u32string read_line(text_stream &strm); // up to next newline

    // class file_stream : public stream { ... };

    stream rw_dev = open_file(U"fn"); // fails if does not exists
    random_access_device create_new_file = create_file(U"fn");  // fails is already exists
    random_access_device overwrite_or_create_file = create_file(U"fn", overwrite);  // never fails

    text_stream & stdin;
    text_stream & stdout;
    text_stream & stderr;

    stream & binary_stdin;
    stream & binary_stdout;
    stream & binary_stderr;
*/
}
