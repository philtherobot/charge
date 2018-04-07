
#include "charge/charge.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>


int main_really(int argc, char ** argv)
{
    std::ifstream is;

    if( argc >= 2 )
    {
        is.open( argv[1] );
    }
    else
    {
        throw charge::Exception("missing source script argument");
    }

    auto configpath( boost::filesystem::path(std::getenv("HOME")) /= ".charge" );

    auto config( charge::load_config( configpath ) );

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
        return main_really(argc, argv);
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
