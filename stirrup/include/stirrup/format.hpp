#pragma once

#include "stirrup/string.hpp"

#include <fmt/format.h>
#include <fmt/xchar.h>

namespace stirrup
{

using format_context = fmt::buffer_context<char32_t>;

template <typename Context = stirrup::format_context, typename... Args>
constexpr auto make_format_args(Args&&... args)
-> fmt::format_arg_store<Context, fmt::remove_cvref_t<Args>...> {
    return {FMT_FORWARD(args)...};
}

using format_args = fmt::basic_format_args<stirrup::format_context>;

template <typename... Args>
using format_string = fmt::basic_format_string<char32_t, fmt::type_identity_t<Args>...>;

void vprint(fmt::basic_string_view<char32_t> fmt, stirrup::format_args args);

template <typename... T>
void print(stirrup::format_string<T...> fmt, T&&... args) {
    const auto& vargs = stirrup::make_format_args(args...);
    return stirrup::vprint(fmt, vargs);
}

}
