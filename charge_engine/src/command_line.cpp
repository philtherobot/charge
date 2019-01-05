
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


void help(boost::filesystem::path const & pgm_path, std::ostream & user_console)
{
    auto bn = pgm_path.stem().string();

    user_console << "Usage: " << bn << " [OPTION]... [script] [script args]...\n";

    user_console <<
        "\n"
        "  -h [--help]         get help on options\n"
        "  -f [--force]        force compilation even if up to date\n"
        "  -n [--noexecute]    do not execute script\n";
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

    po::positional_options_description p;
    p.add("script", -1);

    po::variables_map values;

    auto parser = po::command_line_parser(butfirst(args)).options(options).positional(p);

    po::store(parser.run(), values);

    po::notify(values);

    if (values.count("help"))
    {
        help(args[0], user_console);
        return 0;
    }

    if (!values.count("script"))
    {
        throw CommandLineArgumentError("missing script");
    }

    StringList input_args = values["script"].as<StringList>();

    std::string script_filepath(input_args[0]);

    auto script_object = charge_impl.script(script_filepath);

    StringList script_args = butfirst(input_args);

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
