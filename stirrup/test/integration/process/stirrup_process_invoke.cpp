#include <iostream>

#include "stirrup\stirrup.hpp"

using namespace std;
using namespace stirrup;

int main(int argc, char **argv) {
    try
    {
        cout << "stirrup_process_invoke: start" << endl;
        SystemProcess process;
        process.start(argv[1], StringList());
        cout << "stirrup_process_invoke: exit code " << process.wait_for_exit_code() << "\n";
        return 0;
    }
    catch(exception const &error)
    {
        cout << "Exception: " << error.what() << "\n";
        return 1;
    }
}
