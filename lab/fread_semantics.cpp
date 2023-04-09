//
// Created by philippe.payant on 2021-12-30.
//

#include <cstdio>
#include <iostream>

using namespace std;

void read()
{
    cout << "reading..." << endl;
    char b[9];
    size_t c = fread(b, 1, 8, stdin);
    b[c] = 0;
    cout << "returned " << c << " -" << b << "-" <<endl;
    cout << "feof = " << feof(stdin) << " ferror = " << ferror(stdin) << endl;
    if( feof(stdin) ) exit(0);
}

int main()
{
    for(;;) read();
}