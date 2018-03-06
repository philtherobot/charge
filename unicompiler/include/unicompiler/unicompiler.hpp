#ifndef GUARD_0a6115d8e7704b4c9daaca10e823b268
#define GUARD_0a6115d8e7704b4c9daaca10e823b268

#include "unicompiler/future.hpp"

#include <string>
#include <vector>


namespace unicompiler
{
	
typedef std::vector<std::string> StringList;
typedef std::vector<fu_filesystem::path> PathList;

class Dependencies
{
public:
    StringList defines_;
    StringList static_libraries_;
    StringList system_libraries_;
};

class Compiler
{
public:
	bool compile(fu_filesystem::path const & fn, 
        fu_filesystem const & exe_output_fn,
        Dependencies const & deps, 
        PathList * source_dependencies_out = 0
    ) const;
};

} // unicompiler

#endif
