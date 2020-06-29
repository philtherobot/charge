#include "stirrup/stirrup.hpp"
#include <cwchar>
#include <vector>

using namespace std;

namespace stirrup::str_construction
{

str from(string const & system_encoded_string)
{
    mbstate_t state;
    wchar_t dest_wide = 0;

    size_t result = 0;
    do
    {

        result = mbrtowc(&dest_wide, system_encoded_string.c_str(), system_encoded_string.size(), &state);

        auto const incomplete = static_cast<std::size_t>(-2);
        auto const error = static_cast<std::size_t>(-1);

        if (result == error)
        {
            throw runtime_error("error decoding system string");
        }

        if (result == incomplete)
        {
            throw runtime_error("incomplete multibyte character");
        }

    }
    while (result != 0);

    return str(system_encoded_string.c_str());


//    size_t const conversion_failure = static_cast<size_t>(-1);
//    mbstate_t conversion_state;
//
//    string destination;
//    destination.reserve(32);
//
//    wchar_t const * source = wide_text.c_str();
//    while (*source)
//    {
//        unsigned int const BIG_ENOUGH_FOR_ANY_MB_CHARACTER = 16;
//        char converted_character[BIG_ENOUGH_FOR_ANY_MB_CHARACTER];
//        size_t count_of_written_bytes = wcrtomb(converted_character, *source, &conversion_state);
//
//        if (count_of_written_bytes == conversion_failure)
//        {
//            converted_character[0] = '?';
//            count_of_written_bytes = 1;
//        }
//
//        destination.append(converted_character, count_of_written_bytes);
//
//        ++source;
//    }
//
//    return destination;
}

wstring wstring_from_system_string(string const & system_encoded_string)
{
    mbstate_t state;
    char const * p = system_encoded_string.c_str();

    auto result = mbsrtowcs(nullptr, &p, 0, &state);

    auto const error = static_cast<std::size_t>(-1);

    if (result == error)
    {
        throw runtime_error("error decoding system string");
    }

    vector<wchar_t> space(result+1);

    result = mbsrtowcs(space.data(), &p, 0, &state);

    if (result == error)
    {
        throw runtime_error("error decoding system string");
    }

    return wstring(space.data(), result);
}

}
