#include "stirrup/standard_streams.hpp"

#include <algorithm>
#include <iostream> // todo-php: remove when all done!
#include <iomanip>
#include <string>

using namespace stirrup;
using std::cout;
using std::string;
using std::u32string;

int main()
{
    u32string greeting(U"console_stream_test\n");

    sout.write(greeting);

    auto console_input = sinp.read(15);

    cout << console_input.size() << '\n';
    u32string reversed;
    std::copy(rbegin(console_input), rend(console_input), back_inserter(reversed));
    for(char32_t const c: reversed)
    {
        cout << std::hex << int(c) << ' ';
    }
    cout << '\n';

    // todo-php: console_stream_test needs to be completed
    // it should print what it is with instructions
    // to copy the chinese character it printed
    // and paste it as console input.
    // The program will also advise to run in Windows Terminal.
    // The input will be read through stirrup::sinp.
    // The input will be printed back, reversed to stirrup::sout.

    return 0;
}
