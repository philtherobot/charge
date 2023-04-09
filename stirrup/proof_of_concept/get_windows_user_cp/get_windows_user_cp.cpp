#include <windows.h>

#include <iostream>

using namespace std;

int main()
{
    // The result of these calls is affected by a setting in Windows.
    // On Windows 10: Control Panel, Clock and Region, Region, Administrative, Change system locale.
    cout << "GetACP = " << GetACP() << '\n';
    cout << "GetOEMCP = " << GetOEMCP() << '\n';
}
