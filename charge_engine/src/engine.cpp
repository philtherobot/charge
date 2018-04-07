
#include "charge/charge.hpp"

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
    while( get_line(is, line) )
    {
        if( std::regex_match(line, re) )
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

    for(auto lib: libraries)
    {
        auto node = libraries_node[lib];
        if( !node )
        {
            throw LibraryNotConfiguredError(lib);
        }

        auto s = node[part];
        if( s ) 
        {
            auto v = s.as<std::string>();
            r.push_back( v );
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
: Exception( "library "s + lib + " is not configured"), 
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
    catch(YAML::BadFile const &)
    {
        return YAML::Node();
    }
}


Dependencies find_dependencies(YAML::Node const & config, std::istream & is)
{
    auto libraries = read_imported_libraries(is);

    Dependencies deps;

    deps.libraries_.headers_ =          headers(libraries, config);
    deps.libraries_.static_  = static_libraries(libraries, config);
    deps.libraries_.system_  = system_libraries(libraries, config);

    return deps;
}

} // charge



namespace std 
{

std::ostream & operator << ( std::ostream & os, charge::StringList const & ss)
{
    os << '(';
    if( ss.size() )
    {
        auto set_it = ss.begin();
        os << *set_it;
        while( ++ set_it != ss.end() )
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
    os << "    static: "  << deps.libraries_.static_  << '\n';
    os << "    system: "  << deps.libraries_.system_  << '\n';
    os << "  )\n";
    os << ")\n";
    return os;
}

} //std
