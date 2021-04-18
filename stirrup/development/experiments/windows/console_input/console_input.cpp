#include <iostream>
#include <string>
#include <windows.h>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::strlen;

int main()
{
    //SetConsoleCP(CP_UTF8);  Not required since we read WCHARs (no conversion needed)
    SetConsoleOutputCP(CP_UTF8);

    HANDLE std_input = GetStdHandle(STD_INPUT_HANDLE);

    DWORD const file_type = GetFileType(std_input);
    if( file_type == FILE_TYPE_CHAR)
    {

        cout << "Enter some text: ";
        cout.flush();

        char user_input_array[1024];
        //fgets(user_input_array, sizeof(user_input_array), stdin);

        //ReadFile(std_input, user_input_array, 6, nullptr, nullptr);
        //GetFileType,   FILE_TYPE_CHAR
        //ReadConsole(h, user_input_array, );
//GetConsoleMode

        WCHAR wa[1024] = {};
        unsigned int p{};
        for(;;)
        {
            DWORD nb_read{};
            WCHAR c;
            auto read_successful = ReadConsoleW(std_input, &c, 1, &nb_read, nullptr);
            cout << "out.  success = " << read_successful << " nb_read " << nb_read << "\n";
            if (!read_successful)
            {
                cout << "error " << GetLastError() << '\n';
                std::exit(1);
            }

            wa[p++] = c;
            if ( c == 10 )
            {
                break;
            }
        }
        auto const user_input_size = 6;
        //auto const user_input_size = strlen(user_input_array);

        cout << "Text is " << p << " WCHAR(s) long.\n";

        for(unsigned int i=0; i < p; ++i)
        {
            auto const car = wa[i];
            cout << '[' << i << "] " << unsigned int(car) << '\n';
        }

        //cout << "Text is " << user_input_size << " byte(s) long.\n";

//    for(int i=0; i < user_input_size; ++i)
//    {
//        auto const car = user_input_array[i];
//        cout << '[' << i << "] " << car << ' ' << unsigned int(car) << '\n';
//    }
    }
    else
    {
        char buf[1024];
        DWORD nb_read{};
        bool success = ReadFile(std_input, buf, sizeof(buf), &nb_read, nullptr);
        if( !success)
        {
            cout << "failed to read\n";
            exit(1);
        }
        cout << "ReadFile nb_read = " << nb_read << '\n';

        for(unsigned int i=0; i < nb_read; ++i)
        {
            auto const car = buf[i];
            cout << '[' << i << "] " << unsigned int(unsigned char(car)) << '\n';
        }
    }

    return 0;
}
