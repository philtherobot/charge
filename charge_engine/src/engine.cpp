

#include "cache.hpp"
#include "compiler.hpp"
#include "header_dependencies.hpp"
#include "imports.hpp"
#include "platform_config.hpp"
#include "process.hpp"
#include "tools.hpp"

#include <charge/charge.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/range/algorithm/max_element.hpp>
#include <boost/range/algorithm/transform.hpp>

#include <ctime>
#include <iterator>

using namespace std::string_literals;

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

    return 0; // no file => first epoch second
}

} // anonymous



int charge(boost::filesystem::path const & script, StringList const & args)
{
    //TODO: check if script exists.

    auto script_abspath{ boost::filesystem::absolute(script) };

    auto hostn(hostname());
    auto home(home_path());

    auto cache_path = get_cache_path(hostn, home, script_abspath);

    bool is_new_cache = create_cache(hostn, script_abspath, cache_path);

#if defined(CHARGE_WINDOWS)
    auto const exec_fn{ "executable.exe" };
#else
    auto const exec_fn{ "executable" };
#endif

    auto exec_path = cache_path / exec_fn;

    auto exec_time = get_maybe_file_time(exec_path);

    auto deps_file_contents = read_header_dependencies(cache_path);

    auto deps = decode_header_dependencies(deps_file_contents);

    std::vector<std::time_t> deps_time;

    boost::transform(
        deps,
        std::back_inserter(deps_time),
        get_maybe_file_time);

    deps_time.push_back(get_maybe_file_time(script));

    auto youngest_dep_time{ boost::max_element(deps_time) };

    // We are guaranteed to find one because list is never empty.
    assert(youngest_dep_time != deps_time.end());

    if (!exec_time || std::difftime(*youngest_dep_time, exec_time) > 0)
    {
        // Compile

        auto config_path{ home / ".charge" / "config" };
        auto config{ read_config(config_path) };

        std::ifstream script_stream(script.string());

        auto libraries(find_imports(config, script_stream));

        Config compiler_config;

        if (config["compiler"])
        {
            compiler_config = config["compiler"];
        }
        else
        {
            compiler_config = configure();
            config["compiler"] = compiler_config;
            write_config(config, config_path);
        }

        Compiler compiler(compiler_config);

        Compiler::Arguments compiler_args;
        compiler_args.source_ = script;

        compiler_args.header_paths_ = libraries.header_paths_;
        compiler_args.static_libraries_ = libraries.static_;
        compiler_args.system_libraries_ = libraries.system_;
        compiler_args.lib_paths_ = libraries.lib_paths_;

        compiler_args.executable_output_fn_ = exec_path;

        FileList new_deps = compiler.compile(compiler_args);

        auto new_deps_file_contents = encode_header_dependencies(new_deps);

        write_header_dependencies(cache_path, new_deps_file_contents);
    }


    return exec(exec_path.string(), args);
}

} // charge



namespace std
{

std::ostream & operator << (std::ostream & os, charge::FileList const & fl)
{
    os << '(';
    if (fl.size())
    {
        auto set_it = fl.begin();
        os << *set_it;
        while (++set_it != fl.end())
        {
            os << ',' << *set_it;
        }
    }
    os << ')';
    return os;
}

} //std
