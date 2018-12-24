
#ifndef GUARD_135e65fec96c429eac6e09ed5d344f3e
#define GUARD_135e65fec96c429eac6e09ed5d344f3e

namespace charge
{

using FileList = std::vector<boost::filesystem::path>;

FileList decode_header_dependencies(std::string const & deps_text);
std::string encode_header_dependencies(FileList const & deps);

} // charge

namespace std
{
	std::ostream & operator << (std::ostream & os, charge::FileList const & ss);
}

#endif
