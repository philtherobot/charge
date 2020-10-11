#include <iostream>
#include <optional>

#include "stirrup\stirrup.hpp"

using namespace std;
using namespace stirrup;

namespace
{

struct Options
{
    string child_process;
    StringList child_process_arguments;
    bool capture_output{false};
};

optional<Options> get_options(StringList const & arguments)
{
    if (arguments.size() < 2)
    {
        cout << "Minimum of two arguments required.\n";
        return {};
    }

    auto const capture_option = arguments[0];

    bool capture_output = true;

    if (capture_option == "output_capture_on")
    {
        capture_output = true;
    }
    else if (capture_option == "output_capture_off")
    {
        capture_output = false;
    }
    else
    {
        cout << "Invalid capture option.\n";
        return {};
    }

    auto const child_process = arguments[1];

    StringList child_process_arguments;
    copy(arguments.begin() + 2, arguments.end(), back_inserter(child_process_arguments));

    return
        Options
            {
                child_process,
                child_process_arguments,
                capture_output
            };
}

int program(StringList const & arguments)
{
    auto const options = get_options(arguments);
    if (!options) return 1;

    cout << "stirrup_process_invoke: start" << endl;

    SystemProcess process;

    if (options->capture_output)
    {
        auto output_stream = process.start_capture_output(options->child_process, options->child_process_arguments);
        cout << "stirrup_process_invoke: exit code " << process.wait_for_exit_code() << "\n";
        cout << "stirrup_process_invoke: captured\n";
        cout << output_stream->read();
    }
    else
    {
        process.start(options->child_process, options->child_process_arguments);
        cout << "stirrup_process_invoke: exit code " << process.wait_for_exit_code() << "\n";
    }
    return 0;
}

}

int main(int argc, char ** argv)
{
    try
    {
        StringList arguments;
        for (int i = 1; i < argc; ++i)
        {
            arguments.push_back(argv[i]);
        }

        return program(arguments);
    }
    catch (exception const & error)
    {
        cout << "Exception: " << error.what() << "\n";
        return 1;
    }
}
