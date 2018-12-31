#ifndef GUARD_2cfa76ff0d7d45428f1b7c8c882d6aa3
#define GUARD_2cfa76ff0d7d45428f1b7c8c882d6aa3

#include "config.hpp"
#include "tools.hpp"

#include <charge/charge.hpp>

#include <boost/filesystem/path.hpp>

#include <stdexcept>
#include <string>
#include <vector>

namespace charge
{


class Compiler
{
public:
    explicit Compiler(Config const & config);

    struct Arguments
    {
        boost::filesystem::path source_;
        StringList header_paths_;
        StringList static_libraries_;
        StringList system_libraries_;
        StringList lib_paths_;
        boost::filesystem::path executable_output_fn_;
    };


    FileList compile(Arguments const & args);

    std::string msvc_command_line(Arguments const & args) const;
    FileList compile_with_msvc(Arguments const & args) const;

private:
    Config config_;
};


class ProgramDetector;

// TODO: decide if "Config" or "CompilerConfig".
Config configure(ProgramDetector & program_detector);


} // charge

#endif
