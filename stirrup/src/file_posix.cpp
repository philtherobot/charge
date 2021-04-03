#include "stirrup/file.hpp"
#include "stirrup/string.hpp"

using std::filesystem::path;
using std::vector;

namespace stirrup
{

namespace
{

file open(path const & file_path, char const * const mode)
{
    return file(fopen(encode_string(file_path.u32string(), std::locale()).data(), mode));

}

}
file create_new_file(path const & new_file_path)
{
    return open(new_file_path, "w");
}

file open_file(path const & existing_file_path)
{
    return open(existing_file_path, "r");
}

}
