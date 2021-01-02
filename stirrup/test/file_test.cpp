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

vector<char> read_test_file_content(path const & file_path)
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

void populate_test_file(path const & file_path, vector<char> const & file_content)
{
    auto file = std::ofstream(file_path);
    if (!file)
    {
        throw std::runtime_error("failed to open file");
    }

    file.write(file_content.data(), file_content.size());
}

// todo-php: test:
// - open error
// - read, write, flush errors
// - not flushing may cause a misread when reading through another file handle
// - closing flushes (so not misread)


TEST_CASE("file class")
{
    file_sandbox sandbox;

    path const test_file_path = sandbox.path() / U"test_file_\u503c";

    u32string const top_right_corner = U"\u2510";
    auto const user_text = U"hello" + top_right_corner;

    vector<char> top_right_corner_utf_8 = {'\xE2', '\x94', '\x90'};
    auto user_text_utf_8 = vector<char>{'h', 'e', 'l', 'l', 'o'};
    user_text_utf_8.insert(end(user_text_utf_8), begin(top_right_corner_utf_8), end(top_right_corner_utf_8));

    char top_right_corner_cp_437 = '\xBF';
    auto user_text_cp_437 = vector<char>{'h', 'e', 'l', 'l', 'o'};
    user_text_cp_437.insert(end(user_text_cp_437), top_right_corner_cp_437);

    auto test_file_write = [&](char const * locale_name, vector<char> const & encoded_user_text)
        {
            file new_file = create_new_file(test_file_path);
            new_file.set_locale(std::locale(locale_name));

            new_file.write(user_text);

            new_file.flush();

            CHECK(read_test_file_content(test_file_path) == encoded_user_text);
        };

    auto test_file_read = [&](char const * locale_name, vector<char> const & encoded_user_text)
    {
        populate_test_file(test_file_path, encoded_user_text);

        file existing_file = open_file(test_file_path);
        existing_file.set_locale(std::locale(locale_name));

        CHECK(existing_file.read(1024) == user_text);
    };

    SECTION("file::write UTF-8")
    {
        test_file_write(".UTF-8", user_text_utf_8);
    }

    SECTION("file::read UTF-8")
    {
        test_file_read(".UTF-8", user_text_utf_8);
    }

    SECTION("file::write IBM/PC codepage 437")
    {
        test_file_write(".437", user_text_cp_437);
    }

    SECTION("file::read IBM/PC codepage 437")
    {
        test_file_read(".437", user_text_cp_437);
    }

    SECTION("file::input_stream")
    {
        populate_test_file(test_file_path, user_text_utf_8);

        file input_file = open_file(test_file_path);
        input_file.set_locale(std::locale(".UTF-8"));
        input_stream file_input_stream = input_file.input_stream();

        CHECK(file_input_stream.read(1024) == user_text);
    }

    SECTION("file::output_stream")
    {
        file output_file = create_new_file(test_file_path);
        output_file.set_locale(std::locale(".UTF-8"));

        output_stream file_output_stream = output_file.output_stream();
        file_output_stream.write(user_text);
        file_output_stream.flush();

        CHECK(read_test_file_content(test_file_path) == user_text_utf_8);
    }
}
