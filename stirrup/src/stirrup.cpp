#include "stirrup/stirrup.hpp"

#include <iostream>

int main(int argc, char ** argv)
{
    try
    {
        std::vector<std::string> args;
        for (int i = 0; i < argc; ++i)
        {
            args.push_back(argv[i]);
        }
        return stirrup::main(std::move(args));
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
