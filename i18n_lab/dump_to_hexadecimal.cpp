#include <string>
#include <iomanip>
#include <cctype>
#include <sstream>

bool is_printable(char character)
{
    if (character < 1)
    {
        return false;
    }

    return std::isprint(character);
}

std::string dump_to_hexadecimal(void const * data, std::size_t length)
{
    int const number_of_columns = 16;
    int const width_of_hex = 2;
    int const width_of_offset = 4;
    int const spaces_to_hex_printout = 2;
    int const spaces_to_human_printout = 2;

    using namespace std;

    ostringstream line;

    size_t offset = 0;

    for (; offset < length; offset += 16)
    {
        for (size_t i = 0; i < 16 && i + offset < length; ++i)
        {
            unsigned char const * data_as_bytes = reinterpret_cast<unsigned char const *>(data);
            unsigned char byte = data_as_bytes[i + offset];
            line << setfill('0') << setw(width_of_hex) << hex << unsigned int(byte) << ' ';
        }

        line << '\n';
    }

    return line.str();
}

std::string dump_to_hexadecimal(char const * zero_terminated_array)
{
    return dump_to_hexadecimal(
        reinterpret_cast<void const *>(zero_terminated_array),
        std::strlen(zero_terminated_array));
}
