
// chargetrick import boost_filesystem

#include <boost/filesystem.hpp>

#include <iostream>

int main(int argc, char ** args)
{
	if (argc < 1) return 1;
	boost::filesystem::path p = args[1];

	std::cout << p << '\n';
	std::cout << boost::filesystem::file_size(p) << '\n';

	return 0;
}
