# TODO

## Entry points

There are three programs, all of them implement the same pattern of main with try/catch calling main_really.  Make this a reusable component.


## Should we use "typedef" or "using X =" ?


## StringList and FileList

Belongs to tools.hpp.  Move them.


## Declarations of the innards for tests

Some declarations are placed in the public headers of engine.  This is done in order to be able to write tests against those pieces.  Right now, the public interface and those implementation pieces are mixed together.  The users are lost.   Find a better way.


## ShellProcess and exec

In the Windows implementation, exec is not implemented from ShellProcess.  There is duplication of somewhat similar nasty OS-specific code.


## Integration tests

ShellProcess and exec are tested by hand.  There should be scripts (batches and shell) to automate those routines.

Done for Windows.


## Documentation

See the Quicksee doc.  Also the grind doc.


## POSIX platform support


## Small things

- Enable stricter warnings
- Enable _CRT_SECURE_NO_WARNINGS for MSVC
- Check for TABs

