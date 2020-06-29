#include <iostream>
#include <io.h>
#include <fcntl.h>
//#include <windows.h>
using namespace std;

void main()
{
    // Calling _setmode breaks stdout and the program outputs two bytes per char!
    // But without calling it, output stops at the Unicode character.
    _setmode(_fileno(stdout), _O_U16TEXT);
    wcout << L"unicode -\u03A6- greek letter phi U+03A6\n";
    wcout << L"unicode -\u040A- Cyrillic Capital Letter Nje U+040A\n";
}
