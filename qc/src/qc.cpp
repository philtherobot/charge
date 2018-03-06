
#include "quickc/future.hpp"
#include "quickc/quickc.hpp"

#include <iostream>
#include <memory>


namespace ffl = quickc::fu_filesystem;

int main_really(int argc, char ** argv)
{
    std::unique_ptr<quickc::InputStream> is;

    if( argc >= 2 )
    {
        is.reset( new quickc::InputStream(ffl::path(argv[1])) );
    }
    else
    {
        is.reset( new quickc::InputStream(std::cin));
    }

    auto configpath( ffl::path(getenv("HOME")) /= ".qcconfig" );

    auto config( quickc::load_config( configpath ) );

    /*

    // First, check if changed

    fu_filesystem::path sandbox(".");

    auto exec_fn( exec_filename(sandobx, fn) );

    // create/open quickc cache file for script fn
    Cache cache( load_cache(sandbox, fn) );

    bool up_to_date = false;
    if cache contains source dependencies info

        auto time( exec_fn );
        up_to_date = all time of source_deps <= time


    if ! up_to_date

        Compiler comp;

        // Rebuild
        auto library_deps(find_dependencies(*input))

            grep the qctrick import, find them in the config
            this is options for the compiler

        comp.compile(fn, library_deps, exec_fn, sandbox.cache.deps);


    // Up to date
    execute exec_fn, with command-line arguments
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
