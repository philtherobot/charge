
#include "charge/charge.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>


int main_really(std::vector<std::string> const & args)
{
    //auto configpath( boost::filesystem::path(std::getenv("HOME")) /= ".charge" );

    //auto config( charge::load_config( configpath ) );

	charge::compile("script.cpp");

    /*


    // First, check if changed

    filesystem::path cache_path("~/.charge/cache");

    Cache cache( cache_path );

    // create/open charge cache file for script fn
    CacheEntry entry( fn );

    bool up_to_date = false;
    if entry is not new (or null)

        auto time( exec_fn );

        // note: source_dependencies includes
        // the input source file.
        // It is the list of included files
        up_to_date = true;
        for(auto dep: entry.source_dependencies())
            up_to_date &= time dep <= time


    if ! up_to_date

        YAML::Node compiler_config; // from charge config
        if( no compiler config in charge config )
        {
            compiler_config = Compiler::configure();
        }
        else
        {
            // take config from charge config
        }

        Compiler comp(compiler_config);

        // Rebuild
        auto library_deps(find_dependencies(*input))

        entry.source_dependencies() =
            comp.compile(fn, library_deps, entry.executable_filename());



    // Up to date
    execute entry.executable_filename(), with command-line arguments
    */


    return 0;
}

int main(int argc, char ** argv)
{
    try
    {
		std::vector<std::string> args;
		for (int i = 0; i < argc; ++i)
		{
			args.push_back(argv[i]);
		}
		return main_really(std::move(args));
	}
    catch(std::exception const & ex)
    {
        std::cerr << "exception: " << ex.what() << '\n';
    }
    catch(...)
    {
        std::cerr << "unknown exception type\n";
    }
    return 1;
}
