
#include "command_line.hpp"

#include "Commandable.hpp"

#include <charge/exception.hpp>

#include <boost/program_options.hpp>

namespace charge
{

int run_command_line(StringList const & args, Commandable & cmds, std::ostream & user_console)
{
    namespace po = boost::program_options;

    po::options_description options("Allowed options");

    options.add_options()
        ("help,h", "get help on options")
        ("force,f", "force compilation even if up to date")
        ("update,u", "compile the script if it is out of date")
        ("execute,e", "execute script")
        ("script", po::value< StringList >(), "input script")
        ;

    po::positional_options_description p;
    p.add("script", -1);

    po::variables_map values;

    auto parser = po::command_line_parser(args).options(options).positional(p);

    po::store(parser.run(), values);

    po::notify(values);

    if (values.count("help"))
    {
        user_console << options;
        return 0;
    }

    if (!values.count("script"))
    {
        throw CommandLineArgumentError("missing script");
    }

    StringList input_args = values["script"].as<StringList>();

    std::string script(input_args[0]);

    StringList script_args;
    std::copy(input_args.begin() + 1, input_args.end(), std::back_inserter(script_args));

    int exit_code = 0;

    bool no_operation_required = true;

    if (values.count("force"))
    {
        no_operation_required = false;
        exit_code = cmds.compile();
    }

    if (values.count("update"))
    {
        no_operation_required = false;
        exit_code = cmds.update();
    }

    if (values.count("execute"))
    {
        if (no_operation_required)
        {
            throw CommandLineArgumentError("it is not allowed to execute without updating first");
        }
        exit_code = cmds.execute(script_args);
    }

    if (no_operation_required)
    {
        cmds.update();
        exit_code = cmds.execute(script_args);
    }

    return exit_code;
}

} // charge
