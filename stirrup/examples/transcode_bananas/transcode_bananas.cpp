#include "stirrup/stirrup.hpp"

#include <algorithm>
#include <iterator>

using namespace stirrup;
using namespace std;

u32string decode_ansi_string(char const * string)
{
    vector<char> ansi_string;
    copy(string, string + strlen(string), back_inserter(ansi_string));
    return decode_string(ansi_string, locale(".1252"));
}

int main(int argc, char *argv[])
{
    try
    {
        /*
         * transcode_bananas input_file 437 output_file UTF-8
         */

        print(U"transcode_bananas\n");

        if(argc != 5)
        {
            print(U"Wrong number of arguments: {}\n", argc);
            return 1;
        }

        for(int arg_index = 0; arg_index < argc; ++arg_index)
        {
            print(U"#{}: {}\n", arg_index, decode_ansi_string(argv[arg_index]));
        }

        auto const input_file_name = decode_ansi_string(argv[1]);
        auto const input_encoding = argv[2];
        auto const output_file_name = decode_ansi_string(argv[3]);
        auto const output_encoding = argv[4];

        auto input_file = open_file(input_file_name);
        input_file.set_locale(locale(input_encoding));

        auto output_file = create_new_file(output_file_name);
        output_file.set_locale(locale(output_encoding));

        auto const input_content = input_file.read(1024);
        output_file.write(input_content);
    }
    catch(stirrup::exception const & ex)
    {
        print(U"exception: {}\n", ex.message());
        return 1;
    }
    catch(std::exception const & ex)
    {
        print(U"exception: {}\n", decode_ansi_string(ex.what()));
        return 1;
    }
}
