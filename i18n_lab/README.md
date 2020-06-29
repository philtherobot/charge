
## command_line_arguments

Prints back in hexadecimal the first argument given to the program.

## getenv_encoding

Prints back an environment variable in hexadecimal.

## mbstowcs_locale_category

This program explores which category of `setlocale` affect `mbstowcs`.

## open_file

This program tries to open the same file by specifying its name using two different encodings:
- code page windows-1253
- UTF-8

The attempt is made by setting the locale code page from the `LC_ALL` environment variable.

To work correctly, this program requires the following file in the current directory: `αβψδε`. These are letters A, B, C, D and E typed on Windows with the greek keyboard.  See the program for the exact code points used.

## read_file

Reads the file named in the first argument and prints the content to stdout in hexadecimal.

To be useful, this program can be fed the following files:
- utf8_greek
- utf16be_greek
- windows-1253_greek


## wchar_win_console

This program tries to use `wcout`, part of the "all wstring" proposal.


## stdout_and_codepage

Does setting the code page of the locale affect the output of stdout?

Under Windows, yes it does.  Wow.  But `chcp` is also in the mix.  If you print "\xCE\xA6", two greek characters in code page 1253, while the C locale is active, then the output is still kind of classic line character art. 

## read_stdin

Waits for the user to input text on STDIN (until EOL) and prints the received text in hexadecimal.

Test how reading STDIN is handled.
