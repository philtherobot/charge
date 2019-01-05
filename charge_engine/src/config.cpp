
#include "config.hpp"

#include <fstream>


namespace charge
{

Config read_config(boost::filesystem::path const & fn)
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


void write_config(Config const & config, boost::filesystem::path const & fn)
{
    std::ofstream config_stream(fn.string());
    config_stream << config << '\n';
}

} // charge
