#include "stirrup/format.hpp"

#include "stirrup/standard_streams.hpp"

namespace stirrup
{

void vprint(fmt::basic_string_view<char32_t> fmt, stirrup::format_args args)
{
    sout.write(fmt::vformat(fmt, args));
}

}
