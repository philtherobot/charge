#include "stirrup/format.hpp"

#include "stirrup/standard_streams.hpp"

namespace stirrup
{

void vprint(std::u32string_view format,
    fmt::basic_format_args<fmt::buffer_context<fmt::type_identity_t<char32_t>>> args
)
{
    sout.write(fmt::vformat(format, args));
}

}
