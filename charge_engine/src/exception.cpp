
#include <charge/exception.hpp>

#include <sstream>

using namespace std::string_literals;

namespace charge
{

LibraryNotConfiguredError::LibraryNotConfiguredError(std::string const & lib)
    : Exception("library "s + lib + " is not configured"),
    lib_(lib)
{}

std::string const & LibraryNotConfiguredError::library() const
{
    return lib_;
}


CommandLineArgumentError::CommandLineArgumentError(std::string const & msg)
    : Exception(msg)
{}


UnsupportedFamilyError::UnsupportedFamilyError(std::string const & family)
    :
    Exception("unsupported compiler family "s + family),
    family_(family)
{}


std::string const & UnsupportedFamilyError::family() const
{
    return family_;
}


AbsolutePathError::AbsolutePathError(boost::filesystem::path const & p)
    :
    Exception(msg(p))
{}


std::string AbsolutePathError::msg(boost::filesystem::path const & p)
{
    std::ostringstream os;
    os << "path " << p << " is not absolute";
    return os.str();
}


} // charge
