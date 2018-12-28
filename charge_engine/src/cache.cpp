
#include "cache.hpp"

#include "tools.hpp"

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>

#include <sstream>

namespace charge
{


boost::filesystem::path get_cache_path(
    std::string const & hostname,
    boost::filesystem::path const & home,
    boost::filesystem::path const & script_abspath)
{
    check_absolute(script_abspath);

    auto cache_root = home / ".charge" / "cache";

    std::string uniq{ hostname + script_abspath.string() };

    std::ostringstream os;
    os << std::hash<std::string>{}(uniq);

    return cache_root / os.str();
}


bool create_cache(
    std::string const & /*hostname*/,
    boost::filesystem::path const & script_abspath,
    boost::filesystem::path const & cache_abspath)
{
    check_absolute(script_abspath);
    check_absolute(cache_abspath);

    // TODO: write info file with hostname and script path

    return boost::filesystem::create_directories(cache_abspath);
}


std::string read_header_dependencies(boost::filesystem::path const & cache_abspath)
{
    check_absolute(cache_abspath);

    auto depsfile{ cache_abspath / "deps" };

    if (!boost::filesystem::exists(depsfile))
    {
        return std::string();
    }

    boost::filesystem::ifstream is(depsfile);

    // Dependencies file is text and does not contain
    // char "zero", so getline reads the whole file.
    std::string str;
    std::getline(is, str, char(0));
    return str;
}


void write_header_dependencies(boost::filesystem::path const & cache_abspath,
    std::string const & deps_text)
{
    check_absolute(cache_abspath);

    auto depsfile{ cache_abspath / "deps" };

    boost::filesystem::ofstream os(depsfile);

    os << deps_text;
}

} // charge
