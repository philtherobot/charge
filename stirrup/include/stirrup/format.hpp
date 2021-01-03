#pragma once

#include "stirrup/string.hpp"

#include <fmt/format.h>

namespace stirrup
{

void vprint(std::u32string_view format,
    fmt::basic_format_args<fmt::buffer_context<fmt::type_identity_t<char32_t>>> args
);

template <typename S, typename ... Args>
void print(S const & format, Args && ... args)
{
    vprint(format, fmt::make_args_checked<Args...>(format, args...));
}
}
