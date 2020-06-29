#include <iostream>
#include <fstream>
#include "dump_to_hexadecimal.cpp"
#include "load_user_locale.cpp"

using namespace std;

int main(int argc, char **argv)
{
    load_user_locale();

    if (argc != 2)
    {
        cout << "ERROR: this program expects an argument\n";
        return 1;
    }

    auto input_file = ifstream(argv[1]);
    char input_buffer[128] = {};

    input_file.read(input_buffer, sizeof(input_buffer));

    cout << dump_to_hexadecimal(input_buffer, input_file.gcount());

    return 0;
}
