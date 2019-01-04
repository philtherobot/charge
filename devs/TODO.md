# TODO


## Entry points

There are three programs, all of them implement the same pattern of main with try/catch calling main_really.  Make this a reusable component.


## ShellProcess and exec

In the Windows implementation, exec is not implemented from ShellProcess.  There is duplication of somewhat similar nasty OS-specific code.


## Integration tests

ShellProcess and exec are tested by hand.  There should be scripts (batches and shell) to automate those routines.

Done for Windows.

TODO for other platforms.


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


## Config should get its own interface

Right now, YAML is exposed directly, so we have no firewall between it and our code.

Make a simple configuration class that is implemented with YAML.


## namespace and details strategy

Should we have a "details" namespace where things such as "ReadbleStream", which is not part of the public API of charge but rather an implementation detail, are placed?

Should we simply rely on "public" versus "private" headers?  I think not, the references made in the code do not show if an identifer is part of the public API or an implementation detail.


## Macro definition for libraries

Some libraries require some macros to be defined to compile properly.  

Add a "macro" setting for library configuration.


## Compiled script depends on its libraries

If a library (linked through "chargetrick import") is younger than the compiled executable, relink (or recompile if we must) the script.


## Shebang support

Requires charge to remove the "#!" from the file before sending to the compiler.  This is best done by either rewriting the file to a new temporary file or sending the source to the compiler through a pipe.  Doing so breaks the error messages for the user: either she sees "stdin" or "tmp00023.tmp" as the source filename.

It will be hard, but shebang support would be great.


## Chargetrick as a YAML document

We should think about making the chargetrick a YAML document.  Since we will probably allow the script's chargetrick to override things from the configuration's YAML, it will be much easier for users to specify things in the same syntax both in the configuration and the script.


## Finding the script filepath

In order for runnin scripts to find where the source script is located, setup an environment variable named `CHARGE_SCRIPT` in the running script.


## Small things

- Enable stricter warnings
- Enable _CRT_SECURE_NO_WARNINGS for MSVC
- YAML exception messages are useless
- In README, fix YAML URL
- Implement way to link against a library directly.
- "Force compilation" option
- "Update" option
- "Purge" option