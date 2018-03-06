
#include "unicompiler/unicompiler.hpp"

#include <boost/assign/std/vector.hpp>

#include <algorithm>
#include <string>
#include <vector>

using namespace std::string_literals;

namespace unicompiler
{
	
namespace
{

int execute(string const & cmd, vector<string> const & args)
{
    auto t = args;
    t.insert(t.begin(), cmd);

    string verbo = "executing: " + join( t, " " );
    talk(1, verbo, "\n");

    redi::pstream process(cmd, t);
    process.close();

    auto st = process.rdbuf()->status();

    talk(1, "exit status: ", st, "\n");

    return st;
}


} //anonymous


bool Compiler::compile(fu_filesystem::path const & fn, 
    fu_filesystem const & exe_output_fn,
    Dependencies const & deps, 
    PathList * source_dependencies_out
) const
{

    syslibs.insert("stdc++");

    using namespace boost::assign;

    std::vector<std::string> args;

    args += "-Wall", "-Wextra", "-Wpedantic";
    args += "-pthread";
    args += "--std=c++14";

    std::for_each( hdrs.begin(), hdrs.end(), 
        [&args] (string const & h) { args += "-I" + h; }
    );

    args += "-o" + exe_output_fn.string();

    args += fn.string();

    args.insert(args.end(), 
        deps.static_libraries_.begin(), 
        deps.static_libraries_.end()
    );

    for_each( syslibs.begin(), syslibs.end(), 
        [&args] (string const & s) { args += "-l" + s; }
    );

    return execute("g++", args);

}

} // unicompiler
