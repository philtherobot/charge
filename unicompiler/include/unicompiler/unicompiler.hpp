#ifndef GUARD_2cfa76ff0d7d45428f1b7c8c882d6aa3
#define GUARD_2cfa76ff0d7d45428f1b7c8c882d6aa3

#include <boost/filesystem.hpp>
#include <yaml-cpp/yaml.h>

#include <vector>

namespace unicompiler
{

typedef std::vector<std::string> StringList;
typedef std::vector<boost::filesystem::path> FileList;

YAML::Node configure();

class Compiler
{
public:
    explicit Compiler(YAML::Node const & config);

    FileList compile(boost::filesystem::path const & source,
        StringList const & headers_path,
        StringList const & static_libraries,
        StringList const & system_libraries,
        boost::filesystem::path const & executable_output_fn);
};


}

#endif

