
#include "../src/process.hpp"

#include <boost/algorithm/string/join.hpp>

#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main_really(std::vector<std::string> const & args)
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
