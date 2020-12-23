#include "stirrup/stream.hpp"
#include "stirrup/string.hpp"

using std::filesystem::path;
using std::vector;

namespace stirrup
{

namespace
{

stream open(path const & file_path, wchar_t const * const mode)
{
    return stream(_wfopen(transcode_to_wstring(file_path.u32string()).c_str(), mode));

}

}
stream create_file(path const & new_file_path)
{
    return open(new_file_path, L"w");
}

stream open_file(path const & existing_file_path)
{
    return open(existing_file_path, L"r");
}

}
