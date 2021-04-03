#include "stirrup/error.hpp"

#include "stirrup/format.hpp"
#include "stirrup/string.hpp"

#include <cstring>

using std::u32string;

namespace stirrup
{

u32string make_errno_message(u32string const & from_function, int err)
{
    return fmt::format<std::u32string, int&, std::u32string const &, std::u32string const &>(
        U"system error (errno) {} ({}) in function {}",
        err,
        transcode_from_locale(strerror(err)),
        from_function
    );
}

runtime_error::runtime_error(u32string const &message) : 
    std::runtime_error(repr(message).c_str()),
    message_(message) 
{}

u32string runtime_error::message() const
{
    return message_;
}

logic_error::logic_error(u32string const &message) : 
    std::logic_error(repr(message).c_str()), 
    message_(message) 
{}

u32string logic_error::message() const
{
    return message_;
}

}
