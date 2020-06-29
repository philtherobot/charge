
// This program explores which category of `setlocale` affect `mbstowcs`.


#include <iostream>
#include <cstdlib>
#include "dump_to_hexadecimal.cpp"

using namespace std;

int main()
{
    cout << "Setting locale: " << setlocale(LC_CTYPE, "english_United States.1253") << '\n';

    wchar_t destination[128] = {};

    // Greek Capital Letter Phi U+03A6, a 'O' with a vertical bar
    char const * const utf8_string = "UTF-8 \xCE\xA6";

    auto const converted_characters_count = mbstowcs(destination, utf8_string, sizeof(destination));

    wchar_t const * const expected = L"UTF-8 \u03A6";

    cout << "converted string result:\n";

    cout << dump_to_hexadecimal(destination, sizeof(destination[0]) * converted_characters_count);

    if (wcscmp(expected, destination) != 0)
    {
        cout << "ERROR: converted string is not as expected\n";
        return 1;
    }

    return 0;
}
