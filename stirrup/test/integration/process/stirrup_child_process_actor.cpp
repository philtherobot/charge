#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char ** argv)
{
    cout << "stirrup_child_process_actor\n";

    int exit_code = 0;

    for (int i = 1; i < argc; ++i)
    {
        cout << i << ": " << argv[i] << "\n";

        if (strcmp(argv[i], "exit_with_error") == 0)
        {
            exit_code = 22;
        }
    }

    return exit_code;
}
