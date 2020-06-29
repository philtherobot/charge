#include <iostream>
#include <cstdlib>


void load_user_locale()
{
    using namespace std;

    auto const lc_all_value = getenv("LC_ALL");
    if (lc_all_value != nullptr && lc_all_value[0] != 0)
    {
        auto const new_locale = setlocale(LC_ALL, lc_all_value);
        if (new_locale)
        {
            cout << "User locale loaded \"" << new_locale << "\"\n";
        }
        else
        {
            cout << "ERROR: failed to set user locale \"" << lc_all_value << "\"\n";
        }
        return;
    }

    auto const new_locale = setlocale(LC_ALL, "");
    if (new_locale)
    {
        cout << "System locale loaded \"" << new_locale << "\"\n";
    }
    else
    {
        cout << "ERROR: failed to set system locale\n";
    }
}
