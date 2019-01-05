
#include "command_line.hpp"

#include "ChargeInterface.hpp"

#include <charge/exception.hpp>

#include <boost/program_options.hpp>

namespace charge
{

namespace
{

StringList butfirst(StringList const & a)
{
    if (a.empty()) { return StringList();  }

    StringList r = a;
    r.erase(r.begin());
    return r;
}

} // anonymous


int run_command_line(StringList const & args, ChargeInterface & charge_impl, std::ostream & user_console)
{
    namespace po = boost::program_options;

    po::options_description options("Allowed options");

    options.add_options()
        ("help,h", "get help on options")
        ("force,f", "force compilation even if up to date")
        ("noexecute,n", "do not execute script")
        ("script", po::value< StringList >(), "input script")
        ;

    /*
    We are cheating: the "script" option (which is positional) is
    made to hold more the just the script.  It holds all the
    positional arguments.
    */

    // TODO: remove "script" from the help text.

    po::positional_options_description p;
    p.add("script", -1);

    po::variables_map values;

    auto parser = po::command_line_parser(butfirst(args)).options(options).positional(p);

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

    std::string script_filepath(input_args[0]);

    auto script_object = charge_impl.script(script_filepath);

    StringList script_args;
    std::copy(input_args.begin() + 1, input_args.end(), std::back_inserter(script_args));

    int exit_code = 0;

    if (values.count("force"))
    {
        exit_code = script_object->compile();
    }
    else
    {
        exit_code = script_object->update();
    }

    if (!values.count("noexecute"))
    {
        exit_code = script_object->execute(script_args);
    }

    return exit_code;
}

} // charge
