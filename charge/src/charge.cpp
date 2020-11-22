
#include <charge/charge.hpp>

#include <exception>
#include <iostream>

int main_really(charge::StringList const & args)
{
    return charge::charge(args);
}

int main(int argc, char ** argv)
{
    try
    {
        charge::StringList args;

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
