#ifndef GUARD_443ec49651504cce9c4942eab05fa9ac
#define GUARD_443ec49651504cce9c4942eab05fa9ac


#include <boost/filesystem.hpp>
#include <boost/optional.hpp>
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string>
#include <vector>


namespace charge
{


using StringList = std::vector<std::string>;


int charge(boost::filesystem::path const & script, StringList const & args);


class Dependencies
{
public:
    struct
    {
        StringList headers_;
        StringList static_;
        StringList system_;
    } libraries_;
};


Dependencies find_dependencies(YAML::Node const & config, std::istream & is);


YAML::Node load_config(boost::filesystem::path const & fn);


void compile(boost::filesystem::path const & script);


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


class CommandLineArgumentError : public Exception
{
public:
	explicit CommandLineArgumentError(std::string const & msg);
};


} // charge

namespace std
{
std::ostream & operator << (std::ostream & os, charge::StringList const & ss);
std::ostream & operator << (std::ostream & os, charge::Dependencies const & deps);
}

#endif
