# Charge design notes

## Compiling script read from stdin

Because Charge caches the compilation result, it needs some reference to check if the cached executable is out of date.  This reference is primarily the source script file's modification date and time.  Without a source script file, there is no cache expiry reference.

We do not support compiling stdin and force the user to do what Charge would have to do to support stdin: write the stream to a temporary file before compiling.
