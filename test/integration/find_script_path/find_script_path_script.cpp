#include <cstdlib>
#include <iostream>

int main(int argc, char **argv)
{
    std::cout << std::getenv("CHARGE_SCRIPT") << '\n';
    return 0;
}
