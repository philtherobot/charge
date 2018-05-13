#include "compiler.hpp"

#include "process.hpp"

#include <iostream>


using namespace std::string_literals;


namespace charge
{

namespace
{

std::string quote(std::string const & s)
{
    return "\""s + s + "\""s;
}


FileList msvc_compiler(Compiler::Arguments const & args)
{
    // "cl /nologo /TP /MT /showIncludes /Fe:<exe> /Fo:<exe path\source.obj> source.cpp"

    auto s = args.source_.stem(); // "script.cpp" -> "script"
    s += ".obj";
    auto p = args.executable_output_fn_.parent_path() / s;

    auto cmd = "cl /nologo /TP /MT /showIncludes "s;
    cmd += quote("/Fe:" + args.executable_output_fn_.string()) + " ";
    cmd += quote("/Fo:" + p.string()) + " ";
    cmd += quote(args.source_.string());

    Process proc;

    proc.start(cmd);

    while (auto res = proc.child_stdout_->read())
    {
        std::cout << *res;
    }

    return FileList();
}

} // anonymous


YAML::Node configure()
{
    // TODO
    YAML::Node conf;

    conf["compiler"] = "/usr/bin/g++";
    conf["version"] = "7.2.0";
    conf["family"] = "g++";

    return conf;
}


Compiler::Compiler(YAML::Node const & conf)
    : config_(conf)
{}


FileList Compiler::compile(Arguments const & args)
{
    auto family = config_["family"].as<std::string>();

    if (family == "msvc")
    {
        return msvc_compiler(args);
    }
    else
    {
        throw UnsupportedFamilyError(family);
    }

    return FileList();
}


UnsupportedFamilyError::UnsupportedFamilyError(std::string const & family)
    :
    Exception("unsupported compiler family "s + family),
    family_(family)
{}

std::string const & UnsupportedFamilyError::family() const
{
    return family_;
}

}
