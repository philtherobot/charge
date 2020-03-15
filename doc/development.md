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
