#include "compiler.hpp"

#include "InclusionNotesPredicate.hpp"
#include "process.hpp"
#include "StreamFilter.hpp"
#include "tools.hpp"

#include <charge/exception.hpp>

#include <functional>
#include <iostream>

#include <boost/algorithm/string/join.hpp>


using namespace std::string_literals;


namespace charge
{


Config configure()
{
    // TODO
    Config conf;

    conf["compiler"] = "/usr/bin/g++";
    conf["version"] = "7.2.0";
    conf["family"] = "g++";

    return conf;
}


Compiler::Compiler(Config const & conf)
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
    auto s = args.executable_output_fn_.stem(); // "fn.exe" -> "fn"
    s += ".obj";
    auto p = args.executable_output_fn_.parent_path() / s;

    auto cl_cmd = config_["command"].as<std::string>();

    StringList options{
        quote_if_needed(cl_cmd),
        "/nologo",
        "/TP",
        "/MD",
        "/showIncludes",
        "/EHsc",
        quote_if_needed("/Fe:" + args.executable_output_fn_.string()),
        quote_if_needed("/Fo:" + p.string())
    };


    for (auto header_path : args.header_paths_)
    {
        options.push_back(quote_if_needed("/I"s + header_path));
    }


    StringList linker_options;

    for (auto lib_path : args.lib_paths_)
    {
        linker_options.push_back(quote_if_needed("/LIBPATH:"s + lib_path));
    }


    StringList words;

    words.insert(words.end(), options.begin(), options.end());
    words.insert(words.end(), quote_if_needed(args.source_.string()));

    if (!linker_options.empty())
    {
        words.insert(words.end(), "/link"s);
        words.insert(words.end(), linker_options.begin(), linker_options.end());
    }

    return boost::algorithm::join(words, " ");
}


FileList Compiler::compile_with_msvc(Arguments const & args) const
{
    auto cmd = msvc_command_line(args);

    ShellProcess proc;

    proc.start(cmd);


    InclusionNotesPredicate inclusion_predicate;

    auto inclusion_filter = make_stream_filter(*proc.child_stdout_, std::ref(inclusion_predicate));

    auto source_fn = args.source_.filename().string() + "\n"s;

    auto source_filename_predicate =
        [source_fn](std::string const & line)
        {
            return line != source_fn;
        };

    auto filter = make_stream_filter(inclusion_filter, source_filename_predicate);


    while (auto res = filter.read())
    {
        std::cout << *res;
    }

    return inclusion_predicate.inclusions_;
}


} // charge

