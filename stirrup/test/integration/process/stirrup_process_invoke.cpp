#include <iostream>

#include "stirrup\stirrup.hpp"

using namespace std;
using namespace stirrup;

int main(int argc, char **argv) {
    try
    {
        StringList arguments;
        for(int i=1; i < argc; ++i)
        {
            arguments.push_back(argv[i]);
        }

        if(arguments.size() < 2)
        {
            cout << "Minimum of two arguments required.\n";
            return 1;
        }

        auto const captureOption = arguments[0];

        bool capture_output = true;

        if(captureOption == "output_capture_on")
        {
            capture_output = true;
        }
        else if(captureOption == "output_capture_off")
        {
            capture_output = false;
        }
        else
        {
            cout << "Invalid capture option.\n";
            return 1;
        }

        cout << "stirrup_process_invoke: start" << endl;

        auto const child_process = arguments[1];

        StringList child_process_arguments;
        copy(arguments.begin() + 2, arguments.end(), back_inserter(child_process_arguments));

        SystemProcess process;
        process.start(child_process, child_process_arguments);
        cout << "stirrup_process_invoke: exit code " << process.wait_for_exit_code() << "\n";
        return 0;
    }
    catch(exception const &error)
    {
        cout << "Exception: " << error.what() << "\n";
        return 1;
    }
}
