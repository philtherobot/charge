#include <catch2/catch.hpp>

#include "stirrup/error.hpp"
#include "stirrup/file.hpp"
#include "stirrup/format.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>

using namespace stirrup;

using std::begin;
using std::end;
using std::filesystem::path;
using std::u32string;
using std::vector;

class file_sandbox
{
public:
    file_sandbox()
        : path_(locate_free_path())
    {}

    ~file_sandbox()
    {
        try
        {
            std::filesystem::remove_all(path_);
        }
            // todo-php: that is just for debugging.  what should we do?
            // failure to remove might be because an handle to a file
            // is still opened.  Thus, it means a test failure.
        catch (std::exception e)
        {
            //throw;  cannot throw from destructors
        }
    }

    path path() const
    { return path_; }


private:
    static std::filesystem::path make_sandbox_directory_path(unsigned int counter)
    {
        auto const temp = std::filesystem::temp_directory_path();

        return temp / fmt::format(U"stirrup_578AFA19-C7BB-4DDF-965A-07E98E6E8322_{:06}", counter);
    }

    static std::filesystem::path locate_free_path()
    {
        for (unsigned int name_counter{}; name_counter < 1000 * 1000; ++name_counter)
        {
            auto const path = make_sandbox_directory_path(name_counter);
            if (!std::filesystem::exists(path))
            {
                std::filesystem::create_directory(path);
                return path;
            }
        }

        throw logic_error(U"All the sandboxes are taken.");
    }

    std::filesystem::path path_;
};

vector<char> read_file(path const & file_path)
{
    auto file = std::ifstream(file_path);
    if (!file)
    {
        throw std::runtime_error("failed to open file");
    }

    vector<char> result;
    std::size_t read_size = 20;
    result.resize(read_size);
    file.read(result.data(), read_size);
    result.resize(file.gcount());

    return result;
}

SCENARIO("File object")
{
    file_sandbox sandbox;
    u32string const top_right_corner = U"\u2510";
    u32string data = U"hello" + top_right_corner;

    vector<char> top_right_corner_utf_8 = {'\xE2', '\x94', '\x90'};
    auto expected_utf_8 = vector<char>{'h', 'e', 'l', 'l', 'o'};
    expected_utf_8.insert(end(expected_utf_8), begin(top_right_corner_utf_8), end(top_right_corner_utf_8));

    char top_right_corner_cp_437 = '\xBF';
    auto expected_cp_437 = vector<char>{'h', 'e', 'l', 'l', 'o'};
    expected_cp_437.insert(end(expected_cp_437), top_right_corner_cp_437);

    auto const new_file_path = sandbox.path() / U"new_file_\u503c";

    GIVEN("no file")
    {
        WHEN("we create a new file and write contents to it")
        {
            file new_file = create_new_file(new_file_path);
            new_file.set_locale(std::locale(".UTF-8"));

            new_file.write(data);

            new_file.flush();

            auto const file_contents = read_file(new_file_path);

            CHECK(file_contents == expected_utf_8);

            file existing_file = open_file(new_file_path);
            existing_file.set_locale(std::locale(".UTF-8"));

            CHECK(existing_file.read(1024) == data);

// todo-php: test:
// - open error
// - read, write, flush errors
// - not flushing may cause a misread when reading through another file handle
// - closing flushes (so not misread)
        }

        GIVEN("the file is encoded with the IBM/PC codepage (437)")
        {
            WHEN("we create a new file and write contents to it")
            {
                file new_file = create_new_file(new_file_path);
                new_file.set_locale(std::locale(".437"));

                new_file.write(data);

                new_file.flush();

                auto const file_contents = read_file(new_file_path);

                CHECK(file_contents == expected_cp_437);

                file existing_file = open_file(new_file_path);
                existing_file.set_locale(std::locale(".437"));

                CHECK(existing_file.read(1024) == data);
            }
        }
    }

    GIVEN("file streams")
    {
        GIVEN("a file with data")
        {
            auto const input_file_path = sandbox.path() / U"input_file";

            file input_file = create_new_file(input_file_path);
            input_file.set_locale(std::locale(".UTF-8"));
            input_file.write(data);
            input_file.close();

            input_file = open_file(input_file_path);
            input_file.set_locale(std::locale(".UTF-8"));

            WHEN("we get an input stream device from the file")
            {
                input_stream file_input_stream = input_file.input_stream();

                CHECK(file_input_stream.read(1024) == data);
            }
        }

        GIVEN("a new empty file")
        {
            auto const output_file_path = sandbox.path() / U"output_file";
            file output_file = create_new_file(output_file_path);
            output_file.set_locale(std::locale(".UTF-8"));

            WHEN("we get an output stream device from the file")
            {
                output_stream file_output_stream = output_file.output_stream();

                file_output_stream.write(data);
                file_output_stream.flush();

                auto const file_contents = read_file(output_file_path);

                CHECK(file_contents == expected_utf_8);
            }
        }
    }
}
