
#include "../src/process.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <stirrup/stirrup.hpp>

int stirrup::main(std::vector<std::string> && args)
{
    if (args.size() <= 1)
    {
        std::cout << "at least one argument required\n";
        return 1;
    }

    charge::StringList exec_args;

    std::copy(args.begin() + 2, args.end(), std::back_inserter(exec_args));

    int code = charge::exec(args[1], exec_args);

    std::cout << "exit code = " << code << "\n";

    return code;
}
