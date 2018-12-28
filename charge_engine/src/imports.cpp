
#include "imports.hpp"

#include <charge/exception.hpp>

#include <boost/algorithm/string/erase.hpp>

#include <regex>
#include <string>


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


std::string extract_import_trick(std::string const & chargetrick)
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
            libraries.emplace_back(extract_import_trick(line));
        }
    }

    return libraries;
}


StringList library_part(StringList const & libraries,
    Config const & config, std::string const & part)
{
    if (libraries.empty()) return StringList();

    auto libraries_node = config["libraries"];
    if (!libraries_node)
    {
        // TODO: figure out/add exception
        // specific to missing "libraries" section in config.
        throw LibraryNotConfiguredError(libraries.front());
    }

    StringList r;

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

StringList header_paths(StringList const & libraries, Config const & config)
{
    return library_part(libraries, config, "header_path");
}

StringList static_libraries(StringList const & libraries, Config const & config)
{
    return library_part(libraries, config, "static_library");
}

StringList system_libraries(StringList const & libraries, Config const & config)
{
    return library_part(libraries, config, "library");
}

StringList libpaths(StringList const & libraries, Config const & config)
{
    return library_part(libraries, config, "lib_path");
}

} // anonymous



Libraries find_imports(Config const & config, std::istream & is)
{
    auto libraries = read_imported_libraries(is);

    Libraries libs;

    libs.header_paths_ = header_paths(libraries, config);
    libs.static_ = static_libraries(libraries, config);
    libs.system_ = system_libraries(libraries, config);
    libs.lib_paths_ = libpaths(libraries, config);

    return libs;
}

} // charge


namespace std
{

std::ostream & operator << (std::ostream & os, charge::Libraries const & libs)
{
    os << "Libraries(\n";

    os << "  header paths: " << libs.header_paths_ << '\n';
    os << "  static: " << libs.static_ << '\n';
    os << "  system: " << libs.system_ << '\n';
    os << "  lib paths: " << libs.lib_paths_ << '\n';
    os << ")\n";
    return os;
}

} // std
