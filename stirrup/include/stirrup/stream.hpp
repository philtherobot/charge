#pragma once

#include <memory>
#include <string>
#include <vector>

namespace stirrup
{

class input_stream
{
public:

    class device
    {
    public:
        virtual ~device() = default;
        virtual std::vector<char> read(std::size_t read_size) = 0;
        virtual std::u32string read2(std::size_t read_size) = 0;
    };

    explicit input_stream(std::unique_ptr<device> && input_device);
    std::vector<char> read(std::size_t read_size);
    std::u32string read2(std::size_t read_size);

private:
    std::unique_ptr<device> device_;
};

class output_stream
{
public:

    class device
    {
    public:
        virtual ~device() = default;
        virtual void write(std::vector<char> const & new_data) = 0;
        virtual void write(std::u32string const & new_data) = 0;
        virtual void flush() = 0;
    };

    explicit output_stream(std::unique_ptr<device> && output_device);
    void write(std::vector<char> const & new_data);
    void write(std::u32string const & new_data);
    void flush();

private:
    std::unique_ptr<device> device_;
};

class memory_input_device: public input_stream::device
{
public:
    explicit memory_input_device(std::u32string const & buffer);
    std::vector<char> read(std::size_t read_size) override;
    std::u32string read2(std::size_t read_size) override;

private:
    std::u32string const & buffer_;
    std::u32string::const_iterator read_position_;
};

input_stream create_memory_input_stream(std::u32string const & buffer);

class memory_output_device : public output_stream::device
{
public:
    explicit memory_output_device(std::u32string & buffer);

    void write(std::vector<char> const & new_data) override;
    void write(std::u32string const & new_data) override;
    void flush() override;

private:
    std::u32string & buffer_;
};

output_stream create_memory_output_stream(std::u32string & buffer);

/*

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

    text_stream & stdin;
    text_stream & stdout;
    text_stream & stderr;

    stream & binary_stdin;
    stream & binary_stdout;
    stream & binary_stderr;
*/
}
