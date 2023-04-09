#include "stirrup/stirrup.hpp"

#include <algorithm>
#include <iterator>

using namespace stirrup;
using namespace std;

int main(int argc, char * argv[])
{
    try
    {
        print(U"{}", U"transcode_bananas\n");

        print(U"{}", U"First argument is a file\n");
        print(U"{}", U"Second argument is the file's text encoding, ex \".1252\"\n");
        print(U"{}", U"Third argument is the output text encoding, ex \".UTF-8\"\n");
        print(U"{}", U"Optional fourth argument is the output file\n");
        print(U"{}", U"\nIt will replace the letter \"A\" with bananas.\n"
                     "So your output encoding needs to support emojis.\n"
                     "If the output file is not specified, then the console\n"
                     "must have the correct encoding.  See \"chcp 65001\"\n\n"
        );

        if (argc < 4)
        {
            print(U"Wrong number of arguments: {}\n", argc);
            return 1;
        }

        for (int arg_index = 0; arg_index < argc; ++arg_index)
        {
            print(U"#{}: {}\n", arg_index, convert_from_ascii(argv[arg_index]));
        }

        auto const input_file_name = convert_from_ascii(argv[1]);
        auto const input_encoding = argv[2];
        auto const output_encoding = argv[3];
        auto const output_file_name = argc > 4 ? convert_from_ascii(argv[4]) : U"";

        auto input_file = open_file(input_file_name);
        input_file.set_locale(locale(input_encoding));

        auto input_content = input_file.read(1024);

        size_t banana_counter = 0;

        ranges::replace_if(
            input_content,
            [&](char32_t c) -> bool
            {
                if (c == U'a')
                {
                    ++banana_counter;
                    return true;
                }
                return false;
            },
            U'\U0001F34C' // banana
        );

        if (output_file_name.empty())
        {
            stdout_file.set_locale(locale(output_encoding));
            print(U"{}", input_content);
        }
        else
        {
            auto output_file = create_new_file(output_file_name);
            output_file.set_locale(locale(output_encoding));
            output_file.write(input_content);
        }

        print(U"counted {} bananas\n", banana_counter);
    }
    catch (stirrup::exception const & ex)
    {
        print(U"exception: {}\n", ex.message());
        return 1;
    }
    catch (std::exception const & ex)
    {
        print(U"exception: {}\n", convert_from_ascii(ex.what()));
        return 1;
    }
}
