#pragma once

#include <string>
#include <stdexcept>

namespace stirrup
{
std::u32string make_errno_message(std::u32string const & from_function, int err);

class runtime_error : public std::runtime_error
{
public:
    explicit runtime_error(std::u32string const & message);
    virtual ~runtime_error() = default;

    std::u32string message() const;

private:
    std::u32string message_;
};

// Basic assumption in the program is violated.
// There is no possible recovery because the
// program has not planned for the assumption
// to be false.
// Normally, these exceptions are not caught
// because nobody knows what to do in reaction
// to them, except log.  Retrying is pointless
// because if you know retrying can solve to issue, you
// have some idea why things could fail, so that
// would/should be a runtime_error instead.)
class logic_error : public std::logic_error
{
public:
    explicit logic_error(std::u32string const & message);
    virtual ~logic_error() = default;

    std::u32string message() const;

private:
    std::u32string message_;
};

}
