
// This program tries to open the same file by specifying its name using three different encodings.


#include <iostream>
#include <fstream>
#include <windows.h>
#include "load_user_locale.cpp"

using namespace std;

void display_file_open_result(bool result, char const * const situation, int & exit_code)
{
    cout << situation << ": ";
    if (result)
    {
        cout << "successfully opened file\n";
    }
    else
    {
        cout << "failed to open file\n";
        exit_code = 1;
    }
};

void try_to_open_file(char const * const file_name, char const * const situation, int & exit_code)
{
    auto f = ifstream(file_name);
    display_file_open_result(bool(f), situation, exit_code);
}

int main()
{
    load_user_locale();

    wchar_t const * const ucs2_file_name = L"\u03B1\u03B2\u03C8\u03B4\u03B5";
    auto ucs2_file = wifstream(ucs2_file_name);
    if (!ucs2_file)
    {
        cout << "ERROR: test file (with five greek letters) is missing\n";
        return 1;
    }

    char windows_1253_file_name[128];
    WideCharToMultiByte(1253, 0, ucs2_file_name, -1, windows_1253_file_name, sizeof(windows_1253_file_name), 0, 0);

    char const * const expected_windows_1253_file_name = "\xE1\xE2\xF8\xE4\xE5";

    if( strcmp(windows_1253_file_name, expected_windows_1253_file_name) != 0)
    {
        cout << "ERROR: windows 1253 code page encoded file name is not as expected\n";
        return 1;
    }

    int exit_code = 0;

    try_to_open_file(windows_1253_file_name, "windows-1253", exit_code);

    char const * const utf8_file_name = "\xCE\xB1\xCE\xB2\xCF\x88\xCE\xB4\xCE\xB5";
    try_to_open_file(utf8_file_name, "UTF-8", exit_code);

    return exit_code;
}
