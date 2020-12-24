#include "stirrup/file.hpp"
#include "stirrup/string.hpp"

using std::filesystem::path;
using std::vector;

namespace stirrup
{

namespace
{

file open(path const & file_path, wchar_t const * const mode)
{
    return file(_wfopen(transcode_to_wstring(file_path.u32string()).c_str(), mode));

}

}
file create_new_file(path const & new_file_path)
{
    return open(new_file_path, L"w");
}

file open_file(path const & existing_file_path)
{
    return open(existing_file_path, L"r");
}

}
