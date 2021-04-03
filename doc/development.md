# Development Notes

## Wishlist


### Shebang support

Large size.
Low priority.

We want to support "shebang" on the first line of a script.  

```
#!/bin/charge

#include <iostream>

int main() {
  std::cout << "Hello world!\n";
  return 0;
}
```

#### Notes

Requires Charge to remove the "#!" from the file before sending to the compiler.  This is best done by either rewriting the file to a new temporary file or sending the source to the compiler through a pipe.  Doing so breaks the error messages for the user: either she sees "stdin" or "tmp00023.tmp" as the source filename.

It will be hard and/or imply compromises, but shebang support would be great.


### Load MSVC environment before launching compiler

In order for Charge to detect and invoke the MSVC toolchain, it must run inside a "Developer Command Prompt" or at least a command prompt where `vcvarsall.bat` was loaded.

Is there a way to allow Charge to operate correctly without requiring `vcvarsall.bat` at all?

#### Notes

We could launch a sub command line box that loads `vcvarsall.bat` before launching the compiler (in the same environment).  This way, the user does not have to do this him/herself.  Instead of detecting the `cl.exe`, we really search for `vcvarsall`.

This is complicated and certainly slower.  `vcvarsall` is not exactly fast.  We need a cmd batch script to launch both `vcvarsall` and `cl` in the same environment.

There might be a faster way to initialize the MSVC toolchain.  But that seems potentially dependant on the MSVC version.


### Purge command

Add a way to clean the compilation cache.  Maybe have a X days argument to the option to remove all cached executables that have not been executed in the last X days.  Note that this option implies that the last execution time must be refreshed whenever the script is launched.


### Compile command

The executable is compiled and the resulting executable placed in the current directory.  In this mode, Charge works more like a compiler.  I do not know if the executable is cached when this command is used.

## Logbook

I am doing the first port to Linux.  It's problems from all sides:
- I am down to trying to get the string tests working
- The locale tests, requiring a few character codepages installed, on Linux, are problematic because by default, not many codepages are installed.
- Visual Studio Code is not responding
- The first and simplest of the string tests, utf-8 to u32string, is failing, mbrtoc32 returns -1.

Explanation for mbrtoc32 failing: it fails when it gets to the codepoint that is 3 bytes, a 3 bytes UTF-8 codepoint.  The test passes is you call `setlocale(LC_ALL, "en_US.utf8");` before.  So this explains it: `mbrtoc32` requires to be in the correct locale, UTF-8, to decode UTF-8.  Makes sense actually.  Why does this work under Windows?  Does MSVC assume UTF-8 encoding on input?

Here's my decision: `transcode_from_utf8` and `transcode_to_utf8` were the first functions to be implemented.  They are not used anywhere in a situation where we know we are going to and from UTF-8.  The current design calls for using the current thread or program locale to know what to encode to and decode from.  Resolution: these functions are getting deleted.

Most of string encoding/decoding works.  Next up is CMake detecting installed locales.

