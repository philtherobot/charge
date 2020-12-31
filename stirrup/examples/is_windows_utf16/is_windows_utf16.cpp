#include <windows.h>
#include <iostream>

int main()
{
    char const * emoji = reinterpret_cast<char const *>(u8"\U0001F600");
    int emoji_size = strlen(emoji);

    std::cout << "emoji size = " << emoji_size << '\n';

    WCHAR result[1024] = {};
    int result_size = 1024;

    int e = MultiByteToWideChar(
        CP_UTF8,
        0,
        emoji,
        emoji_size,
        result,
        result_size
    );

    std::cout << "returned " << e << '\n';

    // e is 2 and there are two WCHAR in result: 0xD83D and 0xDE00.
    // Those are correct UTF-16 for U0001F600.
    // The answer to "does windows do utf-16?" is yes.  WCHAR
    // is actually UTF-16, not UCS-2.
}
