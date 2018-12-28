
#include "header_dependencies.hpp"

#include <sstream>
#include <string>

#include <boost/range/algorithm/for_each.hpp>

namespace charge
{

FileList decode_header_dependencies(std::string const & deps_text)
{
    std::istringstream is(deps_text);
    FileList retval;
    std::string str;
    while (std::getline(is, str))
    {
        retval.push_back(boost::filesystem::path(str));
    }

    return retval;
}


std::string encode_header_dependencies(FileList const & deps)
{
    std::ostringstream os;

    boost::for_each(deps,
        [&os](auto const & dep) {os << dep.string() << '\n'; }
    );

    return os.str();
}

} // charge
