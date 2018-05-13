
#include "charge/charge.hpp"

#include "compiler.hpp"
#include "tools.hpp"

#include <boost/algorithm/string/erase.hpp>

#include <regex>

using namespace std::string_literals;

namespace charge
{

namespace
{

std::istream & get_line(std::istream & is, std::string & out)
{
    std::getline(is, out);
    boost::erase_all(out, "\r"); // To handle CRLF-style EOL
    return is;
}

std::string extract_trick_library(std::string const & chargetrick)
{
    std::istringstream is(chargetrick);
    std::string s;
    is >> s; // the comment
    is >> s; // chargetrick
    is >> s; // import
    is >> s; // library - keep this one
    return s;
}

StringList read_imported_libraries(std::istream & is)
{
    StringList libraries;

    std::regex re("//\\s+chargetrick\\s+import\\s.*");
    std::string line;
    while (get_line(is, line))
    {
        if (std::regex_match(line, re))
        {
            libraries.push_back(extract_trick_library(line));
        }
    }

    return libraries;
}

StringList library_part(StringList const & libraries,
    YAML::Node const & config, std::string const & part)
{
    StringList r;

    auto libraries_node = config["libraries"];

    for (auto lib : libraries)
    {
        auto node = libraries_node[lib];
        if (!node)
        {
            throw LibraryNotConfiguredError(lib);
        }

        auto s = node[part];
        if (s)
        {
            auto v = s.as<std::string>();
            r.push_back(v);
        }
    }

    return r;
}

StringList headers(StringList const & libraries, YAML::Node const & config)
{
    return library_part(libraries, config, "header");
}

StringList static_libraries(StringList const & libraries, YAML::Node const & config)
{
    return library_part(libraries, config, "static_library");
}

StringList system_libraries(StringList const & libraries, YAML::Node const & config)
{
    return library_part(libraries, config, "library");
}

} // anonymous



LibraryNotConfiguredError::LibraryNotConfiguredError(std::string const & lib)
    : Exception("library "s + lib + " is not configured"),
    lib_(lib)
{}

std::string const & LibraryNotConfiguredError::library() const
{
    return lib_;
}


YAML::Node load_config(boost::filesystem::path const & fn)
{
    try
    {
        return YAML::LoadFile(fn.string());
    }
    catch (YAML::BadFile const &)
    {
        return YAML::Node();
    }
}


Dependencies find_dependencies(YAML::Node const & config, std::istream & is)
{
    auto libraries = read_imported_libraries(is);

    Dependencies deps;

    deps.libraries_.headers_ = headers(libraries, config);
    deps.libraries_.static_ = static_libraries(libraries, config);
    deps.libraries_.system_ = system_libraries(libraries, config);

    return deps;
}


void compile(boost::filesystem::path const & script)
{
    YAML::Node conf;
    conf["family"] = "msvc";
    Compiler comp(conf);

    Compiler::Arguments args;
    args.source_ = script;

    boost::filesystem::path cache_dir("C:\\Users\\philt\\OneDrive\\Desktop\\tmp");

    auto basename = script.stem();
    basename += ".exe";

    args.executable_output_fn_ = cache_dir / basename;

    comp.compile(args);

}


/*
bool is_up_to_date(bool is_new_entry, exec_time, deps_time[])
{
    if (is_new_entry) return false;

    for (auto time : deps_times)
    {
        if (time > exec_time) return false;
    }

    return true;
}
*/

int charge(boost::filesystem::path const & script, StringList const & args)
{
	/*
	boost::filepath::path cache_path = get_cache_path(script);

    bool is_new_cache = create_cache(cache_path);

    boost::filesystem::path exec_path = get_executable_path(cache_path, script);

    // Return script time if executable does not exist.
    time exec_time = get_executable_time(exec_path, script);

    std::string deps_file_contents = get_dependencies(cache_path);

    FileList deps = decode_dependencies(deps_file_contents);

    std::vector<time> deps_time;
    std::transform(deps, deps_time, get_file_time();

    if (!is_up_to_date(is_new_cache, exec_time, deps_time))
    {
        // Compile

        auto library_deps( find_dependencies(config, script) );

        auto configpath( boost::filesystem::path(std::getenv("HOME")) /= ".charge" );
        auto config( charge::load_config( configpath ) );

        YAML::Node compiler_config;

        if (!config["compiler"])
        {
            compiler_config = configure();
            config["compiler"] = compiler_config;
            charge::write_config(configpath, config);
        }
        else
        {
            compiler_config = config["compiler"];
        }

        Compiler compiler(compiler_config);

        Compiler::Arguments compiler_args;
        compiler_args.source_ = script;
        compiler_args.header_paths_ = library_deps.libraries_.headers_;
        compiler_args.static_libraries_ = library_deps.libraries_.static_;
        compiler_args.system_libraries_ = library_deps.libraries_.system_;
        compiler_args.executable_output_fn_ = exec_path;

        FileList new_deps = compiler.compile(compiler_args);

        std::string new_deps_file_contents = encode_dependencies(new_deps);

        write_dependencies(cache_path, new_deps_file_contents);
    }

    Process p;

    p.start(write_command(exec_path, script_args));

    return p.exit_code();
	*/
	return 0;
}

} // charge



namespace std
{

std::ostream & operator << (std::ostream & os, charge::StringList const & ss)
{
    os << '(';
    if (ss.size())
    {
        auto set_it = ss.begin();
        os << *set_it;
        while (++set_it != ss.end())
        {
            os << ',' << *set_it;
        }
    }
    os << ')';
    return os;
}

std::ostream & operator << (std::ostream & os, charge::Dependencies const & deps)
{
    os << "Dependencies(\n";
    os << "  libraries(\n";
    os << "    headers: " << deps.libraries_.headers_ << '\n';
    os << "    static: " << deps.libraries_.static_ << '\n';
    os << "    system: " << deps.libraries_.system_ << '\n';
    os << "  )\n";
    os << ")\n";
    return os;
}

} //std
