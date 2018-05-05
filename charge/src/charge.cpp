
#include "charge/charge.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>


int main_really(std::vector<std::string> const & args)
{
    if (args.size() < 1)
    {
        throw;
    }

    std::string script(args.front());

    charge::StringList script_args;
    std::copy(args.begin() + 1, args.end(), std::back_inserter(script_args));

    return charge::charge(script, script_args);
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
    catch (std::exception const & ex)
    {
        std::cerr << "exception: " << ex.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "unknown exception type\n";
    }
    return 1;
}
