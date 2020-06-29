# International Support

> Note: this was written on April 2020.  This is important because what is and is not supported may change in the future.

Stirrup is the library upon which Charge is built.  Stirrup is also intended to be a set of tools for Charge script writers to make their work easier.

I want to support development of software that works well in any locale scenario.  In this day and age, we should expect our programs to work seamlessly with more than the ASCII or the extended ASCII character sets.  At a minimum, code pages must be supported. 
 
## Goal

I want Charge and other small programs that I will write with Stirrup to support the many languages found in the world.  Exactly how Stirrup will support this depends on the following research.  

Things that must be possible:
- getting correct command line arguments
- getting correct environment variables
- opening and creating files with names using the user's code page characters
- standard I/O on the console works for the user's code page and keyboard

Stretch goal:
- All of the above, but for *any* Unicode character on top of respecting the user's code page.  This may come for free from the above if it supports UTF-8.

A first possibility is that Stirrup offers multi-platform support for code pages.  Stirrup programs would handle the user's encoding correctly.

## Questions

### What is the default locale in a C++ program?

The standard library guarantees:

> During program startup, the equivalent of std::setlocale(LC_ALL, "C"); is executed before any user code is run. 


### How do various platforms write their locale strings?

A locale string is one of two things: a locale name or a language and country/region pair.  The encoding part is another property of the complete locale string.  The encoding part is system-defined.

"ja_JP.UTF-8"


### Which locale category affects mbstowcs?

At http://www.cplusplus.com/reference/cstdlib/mbstowcs/:

> The behavior of this function depends on the LC_CTYPE category of the selected C locale.

This was proven to be true.

### Is getenv affected by locale code page like ifstream?

No, not at all.


### Is mbstowcs affected by locale code page like ifstream?

The doc says no.  If that's true, then how will we convert the user's code page characters to something else?  Maybe that's not important, but it would be cool to know.

Turns out that `mbstowcs` is affected!  And it is specifically affected by `LC_CTYPE`.  If you set it right (`setlocale` return non-null, ie. success), then it works.  The windows-1253 code page and UTF-8 worked.

## The `wstring` option

C++17, the latest official C++ standard with available compilers at the moment, has `wstring` as the best type for international strings in Stirrup.  C++20 will introduce `u8string` which is certainly appealing but I have no compiler that is C++20, nor do I want to force it on users.

But using `wstring` has issues.

### Outputting to the Windows console

Lets try:

```c++
    wcout << L"unicode -\u02A4-\n";
```

The string is not fully printed.  The output stops at the first dash.

```c++
    cout << "unicode -\u02A4-\n";
```

This prints "unicode -?-".  This is OK, the Unicode character does not have a matching character in the console's code page.  The cause for the question mark is certainly the compiler trying an failing to output a `char` for `\u02A4`.


```c++
    SetConsoleOutputCP(CP_UTF8);
    wcout << L"unicode -\u02A4-\n";
```

Also not fully printed, stops at dash.

```c++
    _setmode(_fileno(stdout), _O_U16TEXT); 
    wcout << L"unicode -\u02A4-\n";
```

This works, but `cout` is now broken.  In fact, anything that forces us to use `wcout` is not a good idea.  The reason is that in a given program, only one of `cout` or `wcout` can be used.  Because we want Stirrup to play nice with many over tools and that those tools will surely use `cout` if they want to print any thing, we will run into problems.

See the C++ standard:

> Mixing operations on corresponding wide- and narrow-character streams follows the same semantics as mixing such operations on FILEs, as specified in Amendment 1 of the ISO C standard.

And the C standard says that they cannot be mixed.

But the worst part of using `wcout` is that the output is now pairs of bytes for every character: wide chars in the output!

### Conclusion on `wstring`

Using `wstring` forces us down `wcout` which has several issues.  This is not what users or I want.

The only way to use `wstring` everywhere would be to encode and decode at all the outside world boundaries.  That's pretty cool because then, to use `wstring` all the time:
- we have to use wmain (we need to hijack main in any solution for Stirrup)
- users can call wgetenv on any platform
- file name will work as the OS receives the name in wide chars
- only I/O needs conversion

## The UTF-8 option

This option is about using UTF-8 to encode all the strings in the program.  At the boundaries with the outside world, there would be encoding and decoding to and from UTF-8 contained in our strings.  This option splits in two possibilities:
- use a plain `std::string` that holds UTF-8
- use a similar but not implicitly convertible string type with `char*` and `string` (but we could be implicitly convertible with `wchar_t` and `wstring`)


### Why do we need conversion at the boundary of the outside world?

We need conversion at the boundaries because we really want to respect the user's code page.

If we simply output UTF-8:

```c++
    std::cout << "Greek Capital Letter Phi U+03A6, a 'O' with a vertical bar\n";
    std::cout << "UTF-8 \\xCE\\xA6 = \xCE\xA6\n";
```

This prints `UTF-8 \xCE\xA6 = ╬ª` on my Windows console.  But if I switch it to the newly supported UTF-8 code page, 65001, it works!  The output is UTF-8 and the console prints it correctly.  The console font needs to be Lucinda according the pages I read.

```
    chcp 65001
```

Because we want to honor the user's code page (and encoding), we would need to take our UTF-8 and transform it to the user's code page.

### Conclusion on UTF-8

Using UTF-8 everywhere inside the program imposes a completely new set of input/output functions.  Merely doing `cout << "This is phi: \xCE\xA6"` is not enough.  The same is true for all string exchanges with the outside world.

Using UTF-8 everywhere forces us to wrap *all* the outside world because, contrary to the `wstring` proposal, no API in stdlib will do UTF-8.  

### Advantages of using a UTF-8 specific type of string

You get type safety: you will not be mixing things.

The code is aware of the UTF-8 encoding so it can provide better answers.  For example, when the code is aware of UTF-8, it can answer correctly to "how many letters are there in this string?".

You can get automatic conversion to and from the outside world.  Reading a file into a UTF-8 type string can trigger the proper conversion for you.

## "Do nothing" option

In this option, we simply read in bytes and output bytes.  The user is expected to have set the system locale (but most importantly, the locale code page) to the proper setting.  The console is also configured to match as the program will output on STDOUT in the correct code page.

To implement this:
- use wmain to setlocale to user's locale and convert arguments
- %TBD: does setlocale code page fix getenv?

File name handling is correctly handled.
STDOUT is automatically converted.


## Experiments

### `command_line_arguments.cpp`

Dumps the first command line argument as hex.  

Tests:
- In a plain system console:
  - set a Greek keyboard and console and code page 1253
  - set UTF-8 encoding, true multi-bytes encoding

Under Windows, we can see that `main` does not receive all the characters.

## setlocale under Windows

The system locale under Windows 10 is controlled from the Control Panel.  Go to Control Panel/Clock and Region/Region.  The field "Format" controls the locale, specifically the way things are formatted (numbers, money, date and time).  The code page for `setlocale` is set on the Administrative tab.  It is global for all users and requires a reboot to take effect.

So, to help our users, we should add support for `LC_ALL` and `LANG`, just like in the POSIX world.

## ifstream file open affected by locale code page

In program `open_file`, we can see that setting the code page through `setlocale` will allow `ifstream` to correctly open a file with a name using characters from the code page.

## Output to the console

The `chcp` command can be use to make `cmd.exe` correctly interpret the bytes outputted to STDOUT.

So a Windows user, and I think also POSIX user, needs to configure the program to read and write bytes from the system using a specific encoding *and also* configure the console on how to interpret the output of the program.

## getenv

### Windows

`chcp` has no impact.

I did not investigate the effect of the global MS-DOS programs encoding found in Control Panel as this is not something I want Stirrup to rely on.

The locale code page has no effect on getenv.

So we are going to need to use wgetenv under Windows.


## Command line arguments

### Windows

`chcp` has no effect on how arguments are encoded.  It is pointless to try `setlocale` as the strings have already been prepared at this point.

We are going to have to use `wmain` under Windows: this will not be much trouble anyway as we have additional reasons to replace the regular program entry point (exception handling). 

Although, we did notice that the global system encoding affects it.  The command prompt sends the argument properly encoded to the program. 


## Console output

### Windows

The program can output any type of encoded bytes and the user can get them properly interpreted by the console by using `chcp` to set the correct code page.

But the locale *and* encoding have an effect.

- chcp 437
  - "C" locale
    - ╬ª
  - System (empty string) locale, "English_United States.1252"
    - I▌
  - "English_United States.1252" locale
    - I▌
  - "English_United States.1253" locale
    - ?▌
  - "English_United States.437" locale
    - ╬ª
  - "en"
    - I▌
  - "el"
    - ?▌
  - "en.1252"
    - Φ
  - "en.1253"
    - Φ
  - "en.65001"
    - Φ
  - ".65001", "English_United States.utf8"
    - Φ
  - ".437"
    - ╬ª
    
If we pipe the output of the program to a file, the file always contains \xCE \xA6.  It seems that the program's locale only affects how the console will interpret its output.

- chcp 1253
  - "C" locale
    - Ξ¦
  - System (empty string) locale, "English_United States.1252"
    - I¦
  - "English_United States.1252" locale
    - I¦
  - "English_United States.1253" locale
    - Ξ¦
  - "en"
    - I¦
  - "el"
    - Ξ¦
  - "en.1252"
    - Φ
  - "en.1253"
    - Φ
  - "en.65001"
    - Φ
  - ".65001", "English_United States.utf8"
    - Φ
  - "el.1253"
    - Φ
  - ".1253"
    - Ξ¦
    
- chcp 65001
  - "C" locale
    - Φ
  - System (empty string) locale, "English_United States.1252"
    - Î¦
  - "English_United States.1252" locale
    - Î¦
  - "English_United States.1253" locale
    - Ξ¦
  - "English_United States.65001" locale
    - Φ
  - "en"
    - Î¦
  - "el"
    - Ξ¦
  - "en.1252"
    - Φ
  - "en.1253"
    - Φ
  - "en.65001"
    - Φ
  - ".65001", "English_United States.utf8"
    - Φ

- chcp 1252
  - "C" locale
    - Î¦
  - System (empty string) locale, "English_United States.1252"
    - Î¦
  - "English_United States.1252" locale
    - Î¦
  - "English_United States.1253" locale
    - ?¦
  - "en"
    - Î¦
  - "el"
    - ?¦
  - "en.1252"
    - F
  - "en.1253"
    - F
  - "en.65001"
    - F
  - ".65001", "English_United States.utf8"
    - F
  - ".1252"
    - Î¦
    
  
Some observations:
- Using the empty locale (system locale) or its fully qualified equivalent makes no difference.
- When using the "C" locale, `chcp` works as expected.
- Using a locale (versus language) and code page causes `chcp` to be ignored and the console to interpret output as UTF-8.
- Using a locale (versus language) *without* a code page (mostly) causes the console to interpret the output as targeting a code page related to the locale.  "el" locale prints "Ξ¦" in any (expect CP 437) `chcp` setting.  

The simplest setup that gives the expected results is:
- set chcp to the code page
- set the locale with "dot" followed by the code page

Example:
```
chcp 1253
set LC_ALL=.1253
```

Another locale string possible is "language" plus "country/region" "dot" a code page: "English_Canada.1252".

Example:
```
chcp 1253
set LC_ALL=English_Canada.1253
```


## Console input

### Windows

The console's code page (`chcp`) has an effect.

In `read_stdin`, when we paste greek letters ("a" to "j" on the keyboard, "αβψδεφγηιξ"):
- "C" locale
  - chcp 437
    - e0 e1 3f eb ee ed 3f 3f 3f 3f 0a
  - chcp 1252
    - 61 df 3f 64 65 66 3f 3f 3f 3f 0a
  - chcp 1253
    - e1 e2 f8 e4 e5 f6 e3 e7 e9 ee 0a
  - chcp 65001
    - 00 00 00 00 00 00 00 00 00 00 0a

By setting the console code page correctly to 1253, the correct characters are mapped.

Except that this does not work for UTF-8.

- ".65001" locale
  - chcp 437
    - e0 e1 3f eb ee ed 3f 3f 3f 3f 0a
  - chcp 1252
    - 61 df 3f 64 65 66 3f 3f 3f 3f 0a
  - chcp 1253
    - e1 e2 f8 e4 e5 f6 e3 e7 e9 ee 0a
  - chcp 65001
    - 00 00 00 00 00 00 00 00 00 00 0a
  
I found out that UTF-8 support for Windows, in the Console system is buggy.  In 2018, somebody pinpointed an issue with WideCharToMultiByte in the Windows code.  The same person also noted that the static library of MSVC is buggy.  

The conclusion to this is that either I abandon the idea having UTF-8 working on Windows or, at a minimum, I add an API in Stirrup to read from stdin.

## Open file

### Windows

`chcp` has no effect, unsurprisingly.

The locale has an effect, but it only works for UTF-8.  If the locale code page is set to 65001 and we pass an UTF-8 file name to `ifstream`, then it opens.  The same does not work for a windows-1253 encoded string running with a 1253 locale.

Our conclusion here is that we will need to offer an alternate API to open files, even for simple code page-encoded file names.


## Read file

### Windows

Nothing affects file reading (`chcp` or the locale), even when opened in text mode.

Actually encoding and decoding the file content looks like a whole new subject onto itself.  As long as the author has written the files in to correct code page, and we got all our strings also in the correct code page, then the bytes we have in RAM are correct.  If we write them back to file without conversion, the output files should then have the proper code page too.
