
#include "helpers.hpp"

namespace charge
{
namespace test
{

boost::filesystem::path make_absolute_path(boost::filesystem::path const & generic_path)
{
    if (generic_path.has_root_name()) return generic_path;

    using boost::filesystem::path;

    auto abs_path =
        path("C:\\") /
        generic_path.relative_path()
    ;

    abs_path.make_preferred();

    return abs_path;
}

} // test
} // charge

