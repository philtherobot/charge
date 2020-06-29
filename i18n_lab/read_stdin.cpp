
// Waits for the user to input text on STDIN (until EOL) and prints the received text in hexadecimal.


#include <iostream>

#include "dump_to_hexadecimal.cpp"
#include "load_user_locale.cpp"

using namespace std;

int main() {
    load_user_locale();

    char input_buffer[128] = {};
    auto const result = std::fgets(input_buffer, sizeof(input_buffer), stdin);
    if (!result && !feof(stdin))
    {
        cout << "ERROR: failed to read STDIN\n";
        return 1;
    }

    std::cout << dump_to_hexadecimal(input_buffer, 128);
    return 0;
}
