#ifndef GUARD_66b063825a434b2491df4322219f356a
#define GUARD_66b063825a434b2491df4322219f356a

#include <yaml-cpp/yaml.h>

#include <boost/filesystem/path.hpp>

namespace charge
{
using Config = YAML::Node;

Config read_config(boost::filesystem::path const & fn);

void write_config(Config const & config, boost::filesystem::path const & fn);

}

#endif
