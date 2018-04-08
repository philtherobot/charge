#include "unicompiler/unicompiler.hpp"

namespace unicompiler
{

YAML::Node configure()
{
    YAML::Node conf;

    conf["compiler"] = "/usr/bin/g++";
    conf["version"] = "7.2.0";

    return conf;
}   


Compiler::Compiler(YAML::Node const & conf)
{}


FileList Compiler::compile(boost::filesystem::path const & source,
    StringList const & header_paths,
    StringList const & static_paths,
    StringList const & system_paths,
    boost::filesystem::path const & executable_path)
{
    return FileList();
}


}