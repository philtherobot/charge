#ifndef GUARD_d09de46c77d346838c67a01e9a74504b
#define GUARD_d09de46c77d346838c67a01e9a74504b

#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>


namespace charge
{
namespace test
{

boost::filesystem::path make_absolute_path(boost::filesystem::path const & generic_path);


template <typename ExceptionType, typename Callable>
boost::optional<ExceptionType> catch_exception(Callable c)
{
    try
    {
        c();
    }
    catch (ExceptionType const & ex)
    {
        return ex;
    }
    return boost::optional<ExceptionType>();
}


} // test
} // charge

#endif

