#include <iostream>
#include <windows.h>

std::ostream & operator<<(std::ostream & os, char8_t const * char8_array)
{
    os << reinterpret_cast<char const *>(char8_array);
    return os;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    std::cout << u8"Hello, \u503c.\n";
    return 0;
}


