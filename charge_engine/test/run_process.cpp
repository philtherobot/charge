
#include "../src/process.hpp"

#include <stirrup/stirrup.hpp>

#include <boost/algorithm/string/join.hpp>

#include <iostream>
#include <string>
#include <utility>
#include <vector>

int stirrup::main(std::vector<std::string> && args)
{
    charge::ShellProcess p;

    auto cmd = boost::algorithm::join(std::make_pair(args.begin() + 1, args.end()), " ");

    p.start(cmd);

    for (;;)
    {
        auto res = p.child_stdout_->read();
        if (res.empty()) break;
        std::cout << res;
    }

    std::cout << "exit code = " << p.exit_code() << "\n";

    return p.exit_code();
}

