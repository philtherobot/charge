#include <catch2/catch.hpp>
#include <filesystem>
#include <cstdio>
#include <limits>
#include <vector>

#include "stirrup/error.hpp"
#include "stirrup/string.hpp"
#include "stirrup/format.hpp"

using std::filesystem::path;

namespace stirrup_dev
{

using string_list = std::vector<std::u32string>;
using stirrup::binary_buffer;
using stirrup::decode_utf8;
using stirrup::runtime_error;

class stream
{
public:
    stream() = default;

    stream(stream && other)
        : held_device_(std::move(other.held_device_))
    {}

    stream & operator=(stream && other)
    {
        std::swap(held_device_, other.held_device_);
        return *this;
    }

    template <typename device>
    explicit stream(device && i)
        :
        held_device_(new holder<device>(std::move(i)))
    {}

    std::u32string read(std::size_t sz)
    {
        return held_device_->read(sz);
    }

    void write(std::u32string const & t)
    {
        return held_device_->write(t);
    }


private:
    // TODO: not too happy with name "holder"
    class holder_itf
    {
    public:
        virtual ~holder_itf() = default;
        virtual std::u32string read(std::size_t sz) = 0;
        virtual void write(std::u32string const & t) = 0;
    };

    template <typename implementation>
    class holder: public holder_itf
    {
    public:
        holder(implementation && i)
            : i_(std::move(i))
        {}
        implementation i_;
        std::u32string read(std::size_t sz) override
        { return i_.read(sz); }
        void write(std::u32string const & t) override
        { i_.write(t); };
    };

    std::unique_ptr<holder_itf> held_device_;
};

template <typename output_range_t, typename input_container_t>
void concatenate(output_range_t & o, input_container_t && i)
{
    std::ranges::move(std::move(i), std::back_inserter(o));
}

template <typename fread_t, typename feof_t, typename ferror_t>
class c_stream_reader
{
public:

    c_stream_reader(FILE * fp, std::size_t accumulator_size, fread_t fread, feof_t feof, ferror_t ferror) :
        fp_(fp), accumulator_size_(accumulator_size), fread_(fread), feof_(feof), ferror_(ferror)
    {
    }

    binary_buffer accumulator_;

    std::u32string read(std::size_t const requested_size)
    {
        if (ferror_(fp_))
        {
            // FIXME: have an explanation for a failed stream read.
            // See the other throw below.
            throw runtime_error(U"stream read error");
        }
/*
        if (feof_(fp_) && accumulator_.empty())
        {
            return {};
        }
*/
        std::u32string result;

        result = decode_utf8(accumulator_, requested_size);
        if (result.size() >= requested_size)
        {
            // Read was satisfied from previously accumulated data.
            return result;
        }

        // we got zero or more characters, less than block_size, in result.
        // we can presume accumulator is pretty much empty, save an incomplete code point.

        auto size_missing = requested_size - result.size();

        for (;;)
        {

            // TODO: avoid zero-init on buffer preparation of stdio_device::read
            auto const read_size = accumulator_size_ - accumulator_.size();
            binary_buffer read_buffer;
            read_buffer.resize(read_size);

            auto const nb_bytes = fread_(read_buffer.data(), 1, read_size, fp_);
            read_buffer.resize(nb_bytes);
            concatenate(accumulator_, read_buffer);

            result += decode_utf8(accumulator_, size_missing);
            if (result.size() >= requested_size)
            {
                // Read is now satisfied.
                return result;
            }

            if (nb_bytes != read_size)
            {
                // Reached error or end of file.
                if ( ferror_(fp_) && result.empty() )
                {
                    // The message is to be improved, see above.
                    throw runtime_error(U"stream read error");
                }

                return result;
            }

            // Issue another read.
            size_missing -= result.size();
        }
    }

    std::size_t const accumulator_size_;

private:
    FILE * fp_;
    fread_t fread_;
    feof_t feof_;
    ferror_t ferror_;
};

class stdio_device
{
public:
    explicit stdio_device(FILE * fp)
        : fp_(fp)
    {}

    std::u32string read(std::size_t sz)
    {
#if 0
        binary_buffer b;
        // TODO: avoid zero-init on buffer preparation of stdio_device::read
        b.resize(16*1024);
        // feof, ferror
        // return {} when EOF
        for(;;) {

            auto const nb_bytes = fread(b.data(), 1, b.size(), fp_);

            if(nb_bytes == b.size()) // full gulp
            {
                accumulate
            }
            else // error ?
            {
                if (feof(fp_))
                {
                    accumulate partial read
                    return read_buffer_; // swap with empty
                }

                throw ferror(fp_);
            }

        }
#endif
        return {};
    }

    void write(std::u32string const & t)
    {
        //auto const utf8 = stirrup::encode_to_utf8(t);
        //fwrite(utf8.data(), 1, utf8.size(), fp_);
    }

    FILE * fp_ = nullptr;
    binary_buffer read_buffer_;
};

stream cout(stdio_device(stdout));

stream cerr(stdio_device(stderr));

stream cin(stdio_device(stdin));

std::u32string read_all(stream & s)
{
    std::u32string r;
    for (;;)
    {
        std::size_t const read_block_size = 16 * 1024;
        auto const n = s.read(read_block_size);
        if (n.empty()) break;
        r += n;
    }
    return r;
}

class file
{
public:

    file(file && other)
    {
        std::swap(stream, other.stream);
        std::swap(fp_, other.fp_);
    }

    static file overwrite(path const & fn)
    {
        file f;
        f.fp_ = fopen(fn.string().c_str(), "w+");
        f.stream = stirrup_dev::stream(stdio_device(f.fp_));
        return f;
    }

    ~file()
    {
        if (fp_)
        {
            fclose(fp_);
            fp_ = nullptr;
        }
    }

    void seek();
    std::size_t tell() const;
    std::size_t size() const;

    stream stream;

    std::u32string read(std::size_t sz)
    { return stream.read(sz); }
    void write(std::u32string const & t)
    { stream.write(t); }

private:
    file()
    {}
    FILE * fp_ = nullptr; // to implement seek tell & etc  (or keep the whole stdio_device)
};

std::u32string read_all(file & s)
{
    return read_all(s.stream);
}

int execute(/*pgm + args*/);  // shortcut to run a process

class process_device
{
// holds the
// process handle
// stdout handle
// stdin handle
// stderr handle
};

class process
{
public:

    stream stream;

    process(path & executable, string_list const & args);
    explicit process(std::u32string const & command);

    void start(); // async exec
    int join(); // blocks, returns exit code;

    int operator()();  // full exec
};

std::u32string read_all(process & s)
{
    return read_all(s.stream);
}

// PROBLEM: WE HAVE TO KNOW WHAT WE REDIRECT __BEFORE__ THE PROCESS IS CREATED!!!
// we probably have to do a lazy implementation.
process create_process(/*pgm + args*/);

class pipe;  // it is a device that can go in a stream

pipe operator|(stream && source, stream && dest); // returns a connection between s & d
// s.read() -> d.write, IE d.write(s.read())

pipe operator|(pipe && source, stream && dest); // returns a connection between s & d

void examples()
{
#if 0
    execute( file::open("history") | process(U"grep -F version") );

    execute( process(U"git log") | file::overwrite(U"result") );

    execute( process(U"git log") | process(U"grep -F philippe") | file::overwrite(U"result") );

    ( process(U"git log") | process(U"grep -F philippe") | file::overwrite(U"result") )();  // maybe? or as an alternative>?

    execute( process(U"git log") );

    process(U"git log")();  // starts & joins, returns int

    auto job = background( process(U"git log") | file::overwrite(U"result") );
    // ...
    int exit_code = job.join();
#endif
}

void assert_that(bool e)
{
    if (!e)
    {
        // FIXME: failed assertion exception does not contain description of assertion
        throw stirrup::logic_error(U"assertion failed");
    }
}

}

using namespace stirrup;
using namespace stirrup_dev;

#if 0
SCENARIO("output to the console")
{
    SECTION("writing text")
    {
        cout.write(U"some text");
    }

    SECTION("formatting text")
    {
        print(U"format number {}", 21);
        print(cout, U"format number {}", 21);
    }
}

SCENARIO("input from the console")
{
    SECTION("reading text")
    {
        const auto text = cin.read();
    }
}
#endif


using std::runtime_error;


SCENARIO("reading a C stream")
{
    binary_buffer::value_type const * stream_data = nullptr;
    std::size_t stream_size = 0;
    std::optional<std::size_t> error_when_size_equal_or_smaller;
    bool fake_stream_has_error = false;

    auto set_fake_stream = [&](char const data[])
    {
        stream_data = reinterpret_cast<decltype(stream_data)>(data);
        stream_size = strlen(data);
    };

    auto fake_fread = [&](void * buffer, std::size_t, std::size_t n, FILE *) -> int
    {
        if(error_when_size_equal_or_smaller)
        {
            assert_that(stream_size >= *error_when_size_equal_or_smaller);
            n = std::min(n, stream_size - *error_when_size_equal_or_smaller);
        }

        auto const read_size = std::min(n, stream_size);
        memcpy(buffer, stream_data, read_size);
        stream_data += read_size;
        stream_size -= read_size;

        if(error_when_size_equal_or_smaller)
        {
            fake_stream_has_error = stream_size <= *error_when_size_equal_or_smaller;
        }

        return int(read_size);
    };

    auto fake_feof = [&](FILE *) -> int
    {
        return stream_size == 0;
    };

    auto fake_ferror = [&](FILE *) -> int
    {
        return fake_stream_has_error;
        //return error_when_size_equal_or_smaller && stream_size <= *error_when_size_equal_or_smaller;
    };

    FILE * fp = nullptr;
    c_stream_reader reader(fp, 8, fake_fread, fake_feof, fake_ferror);

    set_fake_stream("Hello world!");

    WHEN("read exactly the stream size")
    {
        CHECK(reader.read(12) == U"Hello world!");
        CHECK(reader.accumulator_.empty());

        CHECK(reader.read(5).empty());
    }

    WHEN("request more bytes that the stream size")
    {
        CHECK(reader.read(20) == U"Hello world!");
        CHECK(reader.accumulator_.empty());

        CHECK(reader.read(5).empty());
    }

    WHEN("request repeatedly less bytes that the stream size")
    {
        CHECK(reader.read(5) == U"Hello");
        CHECK(reader.accumulator_.size() == 3);

        CHECK(reader.read(5) == U" worl");
        CHECK(reader.accumulator_.size() == 2);

        CHECK(reader.read(5) == U"d!");
        CHECK(reader.accumulator_.empty());

        CHECK(reader.read(5).empty());
    }

    WHEN("stream has an error at the beginning")
    {
        set_fake_stream("Hel");
        error_when_size_equal_or_smaller = 3;

        CHECK_THROWS_MATCHES(reader.read(5), stirrup::runtime_error, Catch::Message("stream read error"));
    }

    WHEN("stream has an error after 3 bytes")
    {
        set_fake_stream("1234567890");
        error_when_size_equal_or_smaller = 7;

        CHECK(reader.read(5) == U"123");
        CHECK(reader.accumulator_.empty());

        CHECK_THROWS_MATCHES(reader.read(5), stirrup::runtime_error, Catch::Message("stream read error"));
    }

    WHEN("we atomically fread only part of a UTF-8 code point")
    {
        assert_that(reader.accumulator_size_ == 8);
        set_fake_stream("1234567\xE5\x80\xBC" "123");

        CHECK(reader.read(8) == U"1234567\u503C");
        CHECK(reader.accumulator_ == binary_buffer{ '1', '2', '3' } );
    }

    WHEN("stream ends with a partial UTF-8 code point")
    {
        set_fake_stream("123\xE5\x80");

        CHECK(reader.read(8) == U"123");
        CHECK(reader.accumulator_ == binary_buffer{0xE5, 0x80});

        CHECK(reader.read(8).empty());
        CHECK(reader.accumulator_ == binary_buffer{0xE5, 0x80});
    }

    WHEN("stream contains a partial UTF-8 code point")
    {
        set_fake_stream("123\xE5\x80xyz");
        CHECK_THROWS_MATCHES(reader.read(7), stirrup::runtime_error, Catch::Message("error converting from UTF-8"));
    }
}

path get_temporary_file_path()
{
    char unique_path[L_tmpnam_s];
    errno_t result = tmpnam_s(unique_path, L_tmpnam_s);

    if (result != 0)
    {
        throw stirrup::runtime_error(U"Failed to get a temporary file path.");
    }

    return unique_path;
}

class temporary_directory_fixture
{
public:
    temporary_directory_fixture()
        : path_(get_temporary_file_path())
    {
        std::filesystem::create_directory(path_);
    }

    ~temporary_directory_fixture()
    {
        std::filesystem::remove_all(path_);
    }

    path path_;
};

SCENARIO("output to file")
{
    temporary_directory_fixture fixture;

    CAPTURE(fixture.path_);

    auto file = file::overwrite(fixture.path_ / "output");

    SECTION("writing text")
    {
        file.write(U"some text");
    }

    SECTION("formatting text")
    {
        //print(file, U"format number {}", 21);
    }
}

#if 0
SCENARIO("input from file")
{
    file file;
    SECTION("reading text")
    {
        const auto text = file.read();
        //const auto all = read_all(file);
    }
}
#endif

#if 0
SCENARIO("process channels"){

    SECTION("reading output text")
    {
        process process(U"git", U"log");
        string_channel git_log_output;
        process.set_output(git_log_output);

        // PROBLEM: reading on string channel cannot trigger a cascading read from the process.

        while(const auto text = git_log_output.read(), !text.empty()) {
            // obtain the info from `text`
        }
        process.join(); // as it runs, will read process and write to output
    }

    SECTION("feeding input text from memory")
    {
        process process(U"grep", U"-F", U"version");
        string_channel history_text_channel;
        std::u32string text = U"abc";
        history_text_channel.write(text);
        process.set_input(history_text_channel);
        process.start();

        // PROBLEM: the code that must "push" data through the channel
        // is not getting called.
        process.join();  // as it runs, will pump from input channel, send to process
        // process.execute(); // same here
    }

    SECTION("feeding input text from file")
    {
        process process(U"grep", U"-F", U"version");
        file history_file;
        process.set_input(history_file);
        process.execute();
    }
}

#endif
