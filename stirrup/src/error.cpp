#include "stirrup/error.hpp"

#include "stirrup/format.hpp"
#include "stirrup/string.hpp"

#include <cstring>

using std::u32string;

namespace stirrup
{

u32string make_errno_message(u32string const & from_function, int err)
{
    return fmt::format(
        U"system error (errno) {} ({}) in function {}",
        err,
        transcode_from_locale(strerror(err)),
        from_function
    );
}

exception::exception(const std::u32string & message)
    : std::exception(repr(message).c_str()), message_(message)
{}

std::u32string exception::message() const
{
    return message_;
}

}
