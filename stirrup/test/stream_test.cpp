#include <catch2/catch.hpp>

#include <stirrup/error.hpp>
#include <stirrup/format.hpp>
#include <stirrup/stream.hpp>

#include <filesystem>
#include <fstream>

using namespace stirrup;

using std::begin;
using std::end;
using std::filesystem::path;
using std::string;
using std::vector;

    /*
 * text_device dev = open_text_file(U"simple_name");

    dev.write(U"some text\n");
    dev.write(U"more text\n");

    // for reading only (common)
    // for writing a new file (overwrite or create, and you get read also)
    // for read/write (start at BOF, read/seek/write)

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

SCENARIO("Input memory stream")
{
    GIVEN("a memory stream on an input buffer")
    {
        string input_data = "hello";
        vector<char> input_buffer(begin(input_data), end(input_data));

        input_stream s = create_memory_input_stream(input_buffer);

        WHEN("we read from the memory stream")
        {
            CHECK(s.read(2) == vector<char>{'h', 'e'});
            CHECK(s.read(2) == vector<char>{'l', 'l'});
            CHECK(s.read(2) == vector<char>{'o'});
            CHECK(s.read(2).empty());
            CHECK(s.read(2).empty());
        }
    }

    GIVEN("a memory stream on an output buffer")
    {
        vector<char> output_buffer;

        output_stream s = create_memory_output_stream(output_buffer);

        WHEN("we write to the memory stream")
        {
            s.write(vector<char>{'h', 'e'});
            CHECK(output_buffer == vector<char>{'h', 'e'});

            s.write(vector<char>{'l', 'l', 'o'});
            CHECK(output_buffer == vector<char>{'h', 'e', 'l', 'l', 'o'});
        }
    }
}
