
#include <charge/charge.hpp>

#include <stirrup/stirrup.hpp>

#include <exception>
#include <iostream>

int stirrup::main(std::vector<std::string> && args)
{
    return charge::charge(args);
}
