
// Prints back an environment variable in hexadecimal.


#include <iostream>
#include <cstdlib>
#include "dump_to_hexadecimal.cpp"
#include "load_user_locale.cpp"

using namespace std;

int main()
{
    load_user_locale();

    char const * const variable_name = "STIRRUP_GETENV_TEST";
    cout << "reading environment variable " << variable_name << "\n";
    auto const value = getenv(variable_name);
    if (!value)
    {
        cout << "ERROR: value was not found\n";
        return 1;
    }

    cout << dump_to_hexadecimal(value);
    return 0;
}
