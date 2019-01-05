#include "compiler.hpp"

#include "InclusionNotesPredicate.hpp"
#include "platform.hpp"
#include "process.hpp"
#include "StreamFilter.hpp"
#include "tools.hpp"

#include <charge/exception.hpp>

#include <functional>
#include <iostream>

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/predicate.hpp>


using namespace std::string_literals;


namespace charge
{


namespace
{


boost::optional<Config> detect_msvc(ProgramDetector & detector)
{
    auto result = detector.look_for_program("cl");
    if (!result) return boost::optional<Config>();

    if ((*result).exit_code_ != 0) return boost::optional<Config>();

    char const * const msvc_marker =
        "Microsoft (R) C/C++ Optimizing Compiler Version";

    bool is_marker_found = boost::starts_with((*result).captured_, msvc_marker);

    if (is_marker_found)
    {
        Config conf;
        conf["command"] = "cl";
        conf["family"] = "msvc";

        return conf;
    }

    return boost::optional<Config>();
}


boost::optional<Config> detect_gcc(ProgramDetector & detector)
{
    auto result = detector.look_for_program("g++ --version");
    if (!result) return boost::optional<Config>();

    if ((*result).exit_code_ != 0) return boost::optional<Config>();

    // TODO: check for some signature in the output.

    Config conf;
    conf["command"] = "g++";
    conf["family"] = "gcc";

    return conf;
}


} // anonymous


Config configure(ProgramDetector & program_detector)
{
    auto to_function =
        [](std::string const & compiler_family)
    {
        if (compiler_family == "msvc") return &detect_msvc;
        if (compiler_family == "gcc") return &detect_gcc;
        throw UnsupportedFamilyError(compiler_family);
    };


    for (auto compiler_family : platform::compiler_detection_order())
    {
        auto detection_function = to_function(compiler_family);
        auto result = detection_function(program_detector);
        if (result) return *result;
    }

    throw CompilerDetectionError("unable to find a compiler");
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

    for (;;)
    {
        auto res = proc.child_stdout_->read();
        if (res.empty()) break;
        std::cout << res;
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


    for (;;)
    {
        auto res = filter.read();
        if (res.empty()) break;
        std::cout << res;
    }

    if (proc.exit_code())
    {
        throw CompilationFailureError("non-zero compiler exit code");
    }

    return inclusion_predicate.inclusions_;
}


} // charge

