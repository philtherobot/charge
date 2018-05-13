
#include "charge/charge.hpp"

#include "cache.hpp"
#include "compiler.hpp"
#include "config.hpp"
#include "process.hpp"
#include "tools.hpp"

#include <boost/algorithm/string/erase.hpp>
#include <boost/range/algorithm/max_element.hpp>
#include <boost/range/algorithm/transform.hpp>

#include <ctime>
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


FileList decode_dependencies(std::string const & deps_text)
{
	std::istringstream is(deps_text);
	FileList retval;
	std::string str;
	while (std::getline(is, str))
	{
		retval.push_back(boost::filesystem::path{ str });
	}

	return retval;
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


std::time_t get_maybe_file_time(boost::filesystem::path const & path)
{
	using namespace boost::filesystem;

	if (exists(path))
	{
		return last_write_time(path);
	}

	return 0; // no file => first epoch second
}


int charge(boost::filesystem::path const & script, StringList const & args)
{
	//TODO: check if script exists.

	auto script_abspath{ boost::filesystem::absolute(script) };

	auto hostn{ hostname() };
	auto home{ home_path() };

	auto cache_path = get_cache_path(hostn, home, script_abspath);

	bool is_new_cache = create_cache(hostn, script_abspath, cache_path);

#if defined(CHARGE_WINDOWS)
	auto const exec_fn{ "executable.exe" };
#else
	auto const exec_fn{ "executable" };
#endif

	auto exec_path = cache_path / exec_fn;

    auto exec_time = get_maybe_file_time(exec_path);

	auto deps_file_contents = get_dependencies(cache_path);

	auto deps = decode_dependencies(deps_file_contents);

	std::vector<std::time_t> deps_time;

    boost::transform(
		deps,
		std::back_inserter(deps_time), 
		get_maybe_file_time);

	deps_time.push_back( get_maybe_file_time(script) );

	auto youngest_dep_time{ boost::max_element(deps_time) };

	// We are guaranteed to find one because list is never empty.
	assert(youngest_dep_time != deps_time.end());

	if ( !exec_time || std::difftime(*youngest_dep_time, exec_time) > 0 )
    {
        // Compile

		/*
		auto config_path{ home / ".charge" / "config" };
		auto config{ load_config(config_path) };
		
		auto library_deps( find_dependencies(config, script) );



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
		*/
		std::cout << "compile\n";
    }

	std::cout << "execute\n";

	Process p;

    /*
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
