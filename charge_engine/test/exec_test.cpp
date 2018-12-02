
#include "../src/process.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main_really(std::vector<std::string> const & args)
{
	if (args.size() <= 1)
	{
		std::cout << "at least one argument required\n";
		return 1;
	}

	charge::StringList exec_args;

	std::copy(args.begin()+2, args.end(), std::back_inserter(exec_args));

	int code = charge::exec(args[1], exec_args);

	std::cout << "exit code = " << code << "\n";

	return code;
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
