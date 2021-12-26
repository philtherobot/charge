#pragma once

#include <string>
#include <exception>

namespace stirrup
{

std::u32string make_errno_message(std::u32string const & from_function, int err);

void throw_on_errno(std::u32string const & from_function, int err);

class exception : public std::exception
{
public:
    explicit exception(std::u32string const &message);

    virtual std::u32string message() const;

private:
    std::u32string message_;
};


class runtime_error : public exception
{
public:
    using exception::exception;
    virtual ~runtime_error() = default;
};

// Basic assumption in the program is violated.
// There is no possible recovery because the
// program has not planned for the situation
// that was detected.
// Normally, these exceptions are not caught
// because nobody knows what to do in reaction
// to them.
class logic_error : public exception
{
public:
    using exception::exception;
    virtual ~logic_error() = default;
};

}
