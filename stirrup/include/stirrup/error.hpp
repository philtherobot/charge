#pragma once

#include <string>
#include <exception>

namespace stirrup
{
std::string make_errno_message(std::string const & from_function, int err);

class exception : public std::exception
{
public:
    explicit exception(std::u32string const &message);

    std::u32string message() const;

private:
    std::u32string message_;
};


// Basic assumption in the program is violated.
// There is no possible recovery before the
// program has not planned for the assumption
// to be false.
// Normally, these exceptions are not caught
// because nobody knows what to do in reaction
// to them.
class logic_error : public exception
{
public:
    using exception::exception;
};


class runtime_error : public exception
{
public:
    using exception::exception;
};

}
