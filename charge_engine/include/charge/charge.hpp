#ifndef GUARD_443ec49651504cce9c4942eab05fa9ac
#define GUARD_443ec49651504cce9c4942eab05fa9ac


#include "charge/future.hpp"

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace charge
{

typedef std::vector<std::string> StringList;


class InputStream
{
public:
    explicit InputStream(std::istream & is);

    explicit InputStream(fu_filesystem::path const & fn);

    std::istream & reset();

    fu_optional::optional<fu_filesystem::path> filename() const 
    { return filename_; }

private:

    std::string content_;
    fu_optional::optional<fu_filesystem::path> filename_;

    std::istringstream is_;
};

class Dependencies
{
public:
    struct
    {
        StringList headers_;
        StringList static_;
        StringList system_;
    } libraries_;

    StringList included_;
};

Dependencies find_dependencies(YAML::Node const & config, InputStream & is);

YAML::Node load_config(fu_filesystem::path const & fn);

class Exception : public std::runtime_error
{
public:
    using runtime_error::runtime_error;
};

class LibraryNotConfiguredError : public Exception
{
public:
    explicit LibraryNotConfiguredError(std::string const & lib);

    std::string const & library() const;

private:
    std::string lib_;
};

} // charge

namespace std
{
std::ostream & operator << (std::ostream & os, charge::StringList const & ss);
std::ostream & operator << (std::ostream & os, charge::Dependencies const & deps);
}

#endif
