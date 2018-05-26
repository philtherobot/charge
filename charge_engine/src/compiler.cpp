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

	std::string cmd;

    if (family == "msvc")
    {
		return compile_with_msvc(args);
    }
    else
    {
        throw UnsupportedFamilyError(family);
    }

	ShellProcess proc;

	proc.start(cmd);

	while (auto res = proc.child_stdout_->read())
	{
		std::cout << *res;
	}

	return FileList();
}


std::string Compiler::msvc_command_line(Arguments const & args) const
{
	// "cl /nologo /TP /MT /showIncludes /Fe:<exe> /Fo:<exe path\source.obj> source.cpp"

	auto s = args.executable_output_fn_.stem(); // "fn.exe" -> "fn"
	s += ".obj";
	auto p = args.executable_output_fn_.parent_path() / s;

	auto cmd = "cl /nologo /TP /MT /showIncludes "s;
	cmd += quote("/Fe:" + args.executable_output_fn_.string()) + " ";
	cmd += quote("/Fo:" + p.string()) + " ";
	cmd += quote(args.source_.string());

	return cmd;
}


FileList Compiler::compile_with_msvc(Arguments const & args) const
{
	auto cmd = msvc_command_line(args);

	ShellProcess proc;

	proc.start(cmd);

	while (auto res = proc.child_stdout_->read())
	{
		std::cout << *res;
		// TODO: extract included dependencies
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
