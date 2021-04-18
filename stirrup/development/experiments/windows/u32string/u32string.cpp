#include <string>
#include <windows.h>
#include <cuchar>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <clocale>
#include <fmt/format.h>
#include <optional>

using std::u32string;
using std::vector;
using std::uint8_t;
using std::mbstate_t;

vector<char> convert(u32string const & s)
{
    vector<char> utf8;
    utf8.reserve(s.size());

    std::mbstate_t state{};

    for (char32_t const c: s)
    {
        auto const insertion_index = utf8.size();
        utf8.resize(utf8.size() + 4);
        //   utf8.resize(utf8.size() + MB_CUR_MAX);

        char * insertion = utf8.data() + insertion_index;

        // This works for us because MSVC has a bug that works in our favor.
        // We want conversion to UTF-8.  c32rtomb should convert to the locale's
        // encoding, but it does not: it always does UTF-8.
        // I need to fix this as this is not portable.
        auto const nb_bytes_output = c32rtomb(insertion, c, &state);
        utf8.resize(insertion_index + nb_bytes_output);
    }

    return utf8;
}

// we will need to be able to process incomplete UTF-8 sequences (read has cutoff a
// multi-byte sequence, next bytes are coming later).
u32string convert(vector<char> const & utf8)
{
    u32string u32s;
    u32s.reserve(utf8.size());

    mbstate_t state{};

    char const * read_position = utf8.data();
    std::size_t left_count = utf8.size();

    while (left_count > 0)
    {
        char32_t c{};
        std::size_t read_count = mbrtoc32(&c, read_position, left_count, &state);
        switch (read_count)
        {
            case 0: return u32s;
            case -3:
            {
                u32s.push_back(c);
                break;
            }
            case -2: break; // we do not support this (cutoff multi-bute seq)
            case -1:
            {
                std::puts("error converting from UTF-8");
                exit(2);
            }

            default:
            {
                u32s.push_back(c);
                read_position += read_count;
                left_count -= read_count;
                break;
            }
        }
    }

    return u32s;
}

void check(bool expression)
{
    if (expression) return;

    std::fputs("error!\n", stdout);
    std::exit(1);
}

char uint_to_char(unsigned int i)
{
    return char(i);
}

void test_convert()
{
//    check(convert(U"") == vector<char>());
    //check(convert(U"s") == vector{'s'});
    check(convert(U"\u503c") == vector{uint_to_char(0xe5), uint_to_char(0x80), uint_to_char(0xbc)});

    check(convert(vector<char>()) == U"");
    check(convert(vector<char>{'a'}) == U"a");
    check(convert(vector{uint_to_char(0xe5), uint_to_char(0x80), uint_to_char(0xbc)}) == U"\u503c");
}

void print(u32string const & s)
{
    auto const utf8 = convert(s);
    std::fwrite(utf8.data(), sizeof(char), utf8.size(), stdout);
}

namespace stirrup
{

class text_output
{
public:
    virtual ~text_output() = default;
    virtual void write(u32string const & text) = 0;
};

class text_input
{
public:
    virtual ~text_input() = default;
    virtual u32string read(std::size_t /*=max<size_t>*/) = 0;  // read all, n or one
};

std::optional<u32string> read_line(text_input & input); // reads to EOL, strips the EOL

void vprint(std::u32string_view format,
    fmt::basic_format_args<fmt::buffer_context<fmt::type_identity_t<char32_t>>> args
)
{
    ::print(fmt::vformat(format, args));
}

template <typename S, typename ... Args>
void print(S const & format, Args && ... args)
{
    vprint(format, fmt::make_args_checked<Args...>(format, args...));
}

}

vector<char> read_std_in_handle(FILE * handle)
{
    vector<char> utf8;

    for (;;)
    {
        char buffer[1024];
        std::size_t requested_count = sizeof(buffer);
        std::size_t nb_read = std::fread(buffer, sizeof(char), requested_count, stdin);

        std::size_t insertion_index = utf8.size();
        utf8.resize(insertion_index + nb_read);
        std::copy(buffer, buffer + nb_read, utf8.begin() + insertion_index);

        if (nb_read != requested_count) break; // error or EOF
    }

    return utf8;
}

u32string input()
{
    using stirrup::print;

    print(U"Enter some text: ");

    HANDLE std_input = GetStdHandle(STD_INPUT_HANDLE);

    u32string res;

    CONSOLE_READCONSOLE_CONTROL control{};
    control.nLength = sizeof(control);
    control.dwCtrlWakeupMask = 1 << 4;   // Ctrl-D, also known as EOF

    unsigned int p{};
    for (;;)
    {
        DWORD nb_read{};
        WCHAR c[1024] = {};
        auto read_successful = ReadConsoleW(std_input, c, sizeof(c) / sizeof(c[0]), &nb_read, &control);
        print(U"out.  success = {}, nb_read = {}\n", read_successful, nb_read);
        if (!read_successful)
        {
            print(U"error {}\n", GetLastError());
            std::exit(1);
        }

        std::size_t insertion = res.size();
        res.resize(res.size() + nb_read);
        std::copy(c, c + nb_read, res.begin() + insertion);

        if (c[nb_read - 1] == 10)
        {
            break;
        }

        if (c[nb_read - 1] == 4)  // Ctrl-D, EOF
        {
            print(U"EOF detected\n");
            break;
        }
    }

    print(U"Text is {} WCHAR(s) long.\n", res.size());

    for (char32_t c: res)
    {
        print(U"[{}]\n", uint32_t(c));
    }

    return res;
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    //std::setlocale(LC_ALL, "en_US.UTF-8");
    test_convert();

    u32string s(U"32 bits code points, \u503c.\n");
    print(s);

    print(U"Emoticons? Code points above 2^32? Yes: \U0001F600\n");

    // demonstrate print to file
    // demonstrate formatting
    // demonstrate reading console & stdin & file

    //fmt::print("some {} replacement\n", "string");
    //fmt::print(U"some {} replacement\n", U"string");  // do not compile
    fmt::print(L"no replacement\n");  // works
    u32string formatted = fmt::format(U"some {} replacement\n", U"string \u503c");
    print(formatted); // works

    stirrup::print(U"test this {} replacement\n", U"second"); // works

    u32string s_format_string(U"trickier, no ? {}\n");
    stirrup::print(s_format_string, true); // works

    u32string s_arg(U"an argument");
    stirrup::print(U"does it work with {}?\n", s_arg); // works

    if(false)
    {
        HANDLE std_input = GetStdHandle(STD_INPUT_HANDLE);

        DWORD const file_type = GetFileType(std_input);
        if (file_type == FILE_TYPE_CHAR)
        {
            input();
        }
        else
        {
            auto bytes = read_std_in_handle(stdin);
            stirrup::print(U"read {} bytes\n", bytes.size());
            u32string converted = convert(bytes);
            stirrup::print(U"here is what I read \"{}\"\n", converted);
        }
    }

//    print(U"type int then space...\n");
//    int d{};
//    std::scanf("%d", &d);
//    stirrup::print(U"I received {} as an int\n", d);

    return 0;
}
