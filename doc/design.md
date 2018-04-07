# Design notes

## Compiling script read from stdin

Compiling a script passed through stdin to charge
is not supported.  The reason is that processing a
script requires the script to be read twice: the 
first time to read the "chargetricks" (needed before
the compilation) and then compile the script with
the compiler.  A stream such as stdin cannot be
rewinded and thus read twice.  So, in all cases,
charge needs to have the script in a file.

Additionally, because charge caches the compilation
result, it needs some reference to check if the cached
executable is out of date.  This reference is primarily
the source script file's modification date and time.
Without a source script file, there is no cache expiry
reference.

