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
        virtual std::u32string read(std::size_t read_size) = 0;
    };

    explicit input_stream(std::unique_ptr<device> && input_device);
    std::u32string read(std::size_t read_size);

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
        virtual void write(std::u32string const & new_data) = 0;
        virtual void flush() = 0;
    };

    explicit output_stream(std::unique_ptr<device> && output_device);
    void write(std::u32string const & new_data);
    void flush();

private:
    std::unique_ptr<device> device_;
};

class memory_input_device: public input_stream::device
{
public:
    explicit memory_input_device(std::u32string const & buffer);
    std::u32string read(std::size_t read_size) override;

private:
    std::u32string const & buffer_;
    std::u32string::const_iterator read_position_;
};

input_stream create_memory_input_stream(std::u32string const & buffer);

class memory_output_device : public output_stream::device
{
public:
    explicit memory_output_device(std::u32string & buffer);

    void write(std::u32string const & new_data) override;
    void flush() override;

private:
    std::u32string & buffer_;
};

output_stream create_memory_output_stream(std::u32string & buffer);

/*
 // todo-php: read_all & read_line
    std::u32string read_all(input_stream &strm);  // whole file
    std::u32string read_line(input_stream &strm); // up to next newline
*/
}
