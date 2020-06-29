
// Does setting the code page of the locale affect the output of stdout?


#include <iostream>
#include "load_user_locale.cpp"

using namespace std;

void main()
{
    load_user_locale();

    // This is UTF-8 greek letter Phi, U+03A6
    // Printed in:
    // - Windows-65001: phi
    // - Windows-1252: capital circumflex-I and broken vertical line
    // - Windows-1253: greek Xi (hamburger) and broken vertical line
    // - Code page 437 (AKA extended ASCII):  double-line cross and superscript "a"

    cout << "\xCE\xA6\n";
    fputs("\xCE\xA6\n", stdout);
}
