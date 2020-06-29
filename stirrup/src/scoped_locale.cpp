#include "stirrup/stirrup.hpp"

#include <clocale>

using std::setlocale;

namespace stirrup
{
scoped_locale::scoped_locale(int category, char const * locale)
    : category_(category), previous_locale_(setlocale(category_, nullptr))
{
    setlocale(category_, locale);
}

scoped_locale::~scoped_locale()
{
    setlocale(category_, previous_locale_.c_str());
}

}
