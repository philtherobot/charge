#include <catch2/catch.hpp>

#include "stirrup/error.hpp"
#include "stirrup/file.hpp"
#include "stirrup/format.hpp"

#include <filesystem>
#include <fstream>
#include <stdexcept>

using namespace stirrup;

using std::filesystem::path;
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

SCENARIO("File stream")
{
    file_sandbox sandbox;

    GIVEN("no file")
    {
        auto const new_file_path = sandbox.path() / U"new_file_\u503c";

        vector<char> binary_data = {1, 2, 3, 0, 3, 2, 1};

        WHEN("we create a new file and write contents to it")
        {
            file new_file = create_file(new_file_path);

            new_file.write(binary_data);

            new_file.flush();

            auto const file_contents = read_file(new_file_path);

            CHECK(file_contents == binary_data);

            file existing_file = open_file(new_file_path);

            CHECK(existing_file.read(1024) == binary_data);

// todo-php: test:
// - open error
// - read, write, flush errors
// - not flushing may cause a misread when reading through another file handle
// - closing flushes (so not misread)
        }
    }
}
