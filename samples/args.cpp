
#include <cstdlib>
#include <iostream>

int main(int argc, char **argv)
{
    for (int arg_index = 0; arg_index < argc; ++arg_index)
    {
        std::cout << arg_index << ": \"" << argv[arg_index] << "\"\n";
    }

    if (argc > 1)
    {
        return std::atoi(argv[1]);
    }

    return 0;
}
