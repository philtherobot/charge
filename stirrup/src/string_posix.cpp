#include "stirrup/string.hpp"

#include "stirrup/error.hpp"
#include "stirrup/locale.hpp"

#include <cuchar>

namespace stirrup {

using std::u32string;
using std::vector;
using std::locale;

u32string decode_string(vector<char> const & encoded_string, locale const & locale)
{
    locale_block set_locale_from_argument(locale);

    u32string result;
    result.reserve(encoded_string.size());

    mbstate_t state{};

    char const * read_position = reinterpret_cast<char const *>(encoded_string.data());
    size_t left_count = encoded_string.size();

    while (left_count > 0)
    {
        char32_t c{};
        size_t read_count = std::mbrtoc32(&c, read_position, left_count, &state);

        switch (read_count)
        {
            case 0: return result;
            case size_t(-3):
            {
                result.push_back(c);
                break;
            }
            case size_t(-2): throw logic_error(U"incomplete multi-bytes character");
            case size_t(-1): throw runtime_error(U"error converting from encoded string");

            default:
            {
                result.push_back(c);
                read_position += read_count;
                left_count -= read_count;
                break;
            }
        }
    }

    return result;
}

vector<char> encode_string(u32string const & unicode_string, locale const & locale)
{
    locale_block set_locale_from_argument(locale);

    vector<char> result;
    result.reserve(unicode_string.size());

    std::mbstate_t state{};

    for (char32_t const c: unicode_string)
    {
        auto const insertion_index = result.size();
        size_t const max_mb_length = MB_CUR_MAX;
        result.resize(result.size() + max_mb_length);

        char * insertion = reinterpret_cast<char *>(result.data() + insertion_index);

        auto const nb_bytes_output = std::c32rtomb(insertion, c, &state);
        result.resize(insertion_index + nb_bytes_output);
    }

    return result;
}


}

