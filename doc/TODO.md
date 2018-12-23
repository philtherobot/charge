# TODO

## Configure

Implement part where we detect the available compilers.


## Read MSVC dependency info


## Entry points

There are three programs, all of them implement the same pattern of main with try/catch calling main_really.  Make this a reusable component.


## Should we use "typedef" or "using X =" ?


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


## Cache info file

write info file with hostname and script path


## Load MSVC environment before launching compiler

It would be best to load, at least I think, to load vcvarsall.bat before
launching the compiler.  This way, the user does not have to do this
him/herself.  We detect the compiler, which really finds vcvarsall rather than
cl.exe, and we launch this instead.

This is compilated and possibly slower.  vcvarsall is not exactly fast.  We
need a cmd batch script to launch both vcvarsall and then cl.


## Compilation should be dependant on charge's version

New version might have different options for the compiler, so an executable in the
cache might be out of date when charge is upgraded.

We need to version the cache (which version or charge was used to generate it).


## Config should get its own inteface

Right now, YAML is exposed directly, so we have no firewall between it and our code.

Make a simple configuration class that is implemented with YAML.


## namespace and details strategy

Should we have a "details" namespace where things such as "ReadbleStream", which is not part of the public API of charge but rather an implementation detail, are placed?

Should we simply rely on "public" versus "private" headers?  I think not, the references made in the code do not show if an identifer is part of the public API or an implementation detail.


## Small things

- Enable stricter warnings
- Enable _CRT_SECURE_NO_WARNINGS for MSVC
- Check for TABs
- YAML exception messages are useless
- Process stdout capture must assume text output can "cook" it
