#pragma once

#include <locale>

namespace stirrup
{

class locale_block
{
public:
    explicit locale_block(std::locale const & new_locale);
    ~locale_block();

private:
    std::locale initial_locale;
};

}
