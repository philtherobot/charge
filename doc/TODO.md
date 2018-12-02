# TODO

## Should we use "typedef" or "using X =" ?


## Declarations of the innards for tests

Some declarations are placed in the public headers of engine.  This is done in order to be able to write tests against those pieces.  Right now, the public interface and those implementation pieces are mixed together.  The users are lost.   Find a better way.


## ShellProcess and exec

In the Windows implementation, exec is not implemented from ShellProcess.  There is duplication of somewhat similar nasty OS-specific code.


## Integration tests

ShellProcess and exec are tested by hand.  There should be scripts (batches and shell) to automate those routines.


## Small things

- Enable stricter warnings
- Enable _CRT_SECURE_NO_WARNINGS for MSVC
- Check for TABs
