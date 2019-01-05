
#include "cache.hpp"
#include "ChargeInterface.hpp"
#include "command_line.hpp"
#include "compiler.hpp"
#include "header_dependencies.hpp"
#include "imports.hpp"
#include "platform.hpp"
#include "process.hpp"
#include "tools.hpp"

#include <charge/charge.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/range/algorithm/max_element.hpp>
#include <boost/range/algorithm/transform.hpp>

#include <ctime>
#include <fstream>
#include <iterator>

namespace charge
{

namespace
{

std::time_t get_maybe_file_time(boost::filesystem::path const & path)
{
    using namespace boost::filesystem;

    if (exists(path))
    {
        return last_write_time(path);
    }

    return 0; // no file => first second of epoch
}


class Script : public ScriptInterface
{
public:
    explicit Script(boost::filesystem::path const & path);

    virtual int compile();
    virtual int update();
    virtual int execute(StringList const & args);

private:
    bool is_out_of_date();

    boost::filesystem::path path_;
    boost::filesystem::path cache_path_;
    boost::filesystem::path exec_path_;
};


Script::Script(boost::filesystem::path const & path)
    : path_(path)
{
    auto script_abspath{ boost::filesystem::absolute(path_) };

    auto hostn(hostname());
    auto home(home_path());

    cache_path_ = get_cache_path(hostn, home, script_abspath);

    create_cache(hostn, script_abspath, cache_path_);

    auto const exec_fn = platform::complete_executable_filename("executable");

    exec_path_ = cache_path_ / exec_fn;
}


int Script::compile()
{
    auto home(home_path());
    auto config_path{ home / ".charge" / "config" };
    auto config{ read_config(config_path) };

    std::ifstream script_stream(path_.string());

    auto libraries(find_imports(config, script_stream));

    Config compiler_config;

    if (config["compiler"])
    {
        compiler_config = config["compiler"];
    }
    else
    {
        ProgramDetector detector;
        compiler_config = configure(detector);
        config["compiler"] = compiler_config;
        write_config(config, config_path);
    }

    Compiler compiler(compiler_config);

    Compiler::Arguments compiler_args;
    compiler_args.source_ = path_;

    compiler_args.header_paths_ = libraries.header_paths_;
    compiler_args.static_libraries_ = libraries.static_;
    compiler_args.system_libraries_ = libraries.system_;
    compiler_args.lib_paths_ = libraries.lib_paths_;

    compiler_args.executable_output_fn_ = exec_path_;

    FileList new_deps = compiler.compile(compiler_args);

    auto new_deps_file_contents = encode_header_dependencies(new_deps);

    write_header_dependencies(cache_path_, new_deps_file_contents);

    return 0;
}


int Script::update()
{
    if (is_out_of_date())
    {
        return compile();
    }

    return 0;
}


int Script::execute(StringList const & args)
{
    return exec(exec_path_.string(), args);
}


bool Script::is_out_of_date()
{
    auto exec_time = get_maybe_file_time(exec_path_);

    auto deps_file_contents = read_header_dependencies(cache_path_);

    auto deps = decode_header_dependencies(deps_file_contents);

    std::vector<std::time_t> deps_time;

    boost::transform(
        deps,
        std::back_inserter(deps_time),
        get_maybe_file_time);

    deps_time.push_back(get_maybe_file_time(path_));

    auto youngest_dep_time{ boost::max_element(deps_time) };

    // We are guaranteed to find one because list is never empty.
    assert(youngest_dep_time != deps_time.end());

    return !exec_time || std::difftime(*youngest_dep_time, exec_time) > 0;
}


class Charge : public ChargeInterface
{
public:
    ScriptInterfacePointer script(boost::filesystem::path const & path);
};


Charge::ScriptInterfacePointer Charge::script(boost::filesystem::path const & path)
{
    return ScriptInterfacePointer(new Script(path));
}

} // anonymous



int charge(StringList const & args)
{
    Charge charge_impl;
    return run_command_line(args, charge_impl, std::cout);
}

} // charge

