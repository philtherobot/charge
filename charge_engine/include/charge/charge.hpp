#ifndef GUARD_443ec49651504cce9c4942eab05fa9ac
#define GUARD_443ec49651504cce9c4942eab05fa9ac

#include <charge/exception.hpp>
#include <charge/types.hpp>

#include <boost/filesystem/path.hpp>


namespace charge
{

int charge(boost::filesystem::path const & script, StringList const & args);

} // charge

#endif
