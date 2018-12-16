#ifndef GUARD_2cfa76ff0d7d45428f1b7c8c882d6aa3
#define GUARD_2cfa76ff0d7d45428f1b7c8c882d6aa3

#include "tools.hpp"

#include "charge/charge.hpp"

#include <boost/filesystem.hpp>
#include <yaml-cpp/yaml.h>

#include <stdexcept>
#include <string>
#include <vector>

namespace charge
{

YAML::Node configure();

class Compiler
{
public:
    explicit Compiler(YAML::Node const & config);

    struct Arguments
    {
        boost::filesystem::path source_;
        StringList header_paths_;
        StringList static_libraries_;
        StringList system_libraries_;
        boost::filesystem::path executable_output_fn_;
    };


    FileList compile(Arguments const & args);

	std::string msvc_command_line(Arguments const & args) const;
	FileList compile_with_msvc(Arguments const & args) const;

private:
    YAML::Node config_;
};


class UnsupportedFamilyError : public Exception
{
public:
    explicit UnsupportedFamilyError(std::string const & family);

    std::string const & family() const;

private:
    std::string family_;
};


}

#endif
