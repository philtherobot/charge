#include "stirrup/standard_streams.hpp"

#include <algorithm>
#include <iostream> // todo-php: remove when all done!
#include <string>

using namespace stirrup;
using std::cout;
using std::string;
using std::vector;

int main()
{
    string greeting("console_stream_test\n");
    vector<char> ascii_greeting;
    std::copy(begin(greeting), end(greeting), back_inserter(ascii_greeting));

    binary_stdout.write(ascii_greeting);

    auto console_input = binary_stdin.read(15);

    cout << console_input.size() << '\n';
    vector<char> reversed;
    std::copy(rbegin(console_input), rend(console_input), back_inserter(reversed));
    for(char const c: reversed)
    {
        cout << c;
    }
    cout << '\n';

    //stirrup::stdout.write(U"console_stream_test\n");
    // todo-php: console_stream_test needs to be completed
    // it should print what it is with instructions
    // to copy the chinese character it printed
    // and paste it as console input.
    // The program will also advise to run in Windows Terminal.
    // The input will be read through stirrup::stdin (a text_stream).
    // The input will be printed back, reversed to stirrup::stdout.

    return 0;
}
