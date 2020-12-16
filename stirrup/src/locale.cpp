#include "stirrup/locale.hpp"

namespace stirrup
{

locale_block::locale_block(std::locale const & new_locale)
{
    std::locale::global(new_locale);
}

locale_block::~locale_block()
{
    std::locale::global(initial_locale);
}

}
