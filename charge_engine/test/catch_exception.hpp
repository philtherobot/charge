#ifndef GUARD_34779b5777784cefb60d84e021609cde
#define GUARD_34779b5777784cefb60d84e021609cde

#include <boost/optional.hpp>

namespace charge
{
namespace test
{

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
