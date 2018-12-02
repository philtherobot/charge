# Design notes

## Compiling script read from stdin

Compiling a script passed through stdin to charge is not supported.  The reason is that processing a script requires the script to be read twice: the first time to read the "chargetricks" (needed before the compilation) and then compile the script with the compiler.  A stream such as stdin cannot be rewinded and thus read twice.  So, in all cases, charge needs to have the script in a file.

Additionally, because charge caches the compilation result, it needs some reference to check if the cached executable is out of date.  This reference is primarily the source script file's modification date and time.  Without a source script file, there is no cache expiry reference.

Both of those reasons can be eliminated by:
- Writing a program passed in via stdin to a temporary file and then processing it twice.
- The user understandans that the programm will be compiled at every invokation.


## The engine interface design

Charge is an engine library and a command-line tool that invokes the engine.

The engine API is used to create an Engine object.  Optionally set a configuration on the object.  By default, the engine will read the user's configuration ($HOME/.charge/config).

The default task for the charge tool is to update and execute the user's program.

Other tasks might be to:
- only update and copy the executable from the cache
- only compile
- prune the cache

Options:
- force update (to get around modification time problems)


## The engine implementation

First thing is to load the config: most commands need it.

If the compiler is not configured, this is the next step.  Under Windows, we try MSVC.  Launching the program, checking the output to determine version and supported options (useful to check the c++ standard support).  Then gcc, then clang.  Under Unix systems, gcc first, then clang, then MSVC.  The configuration is updated and saved.

The script is read, checking for Charge tricks.

Dependency analysis is also performed: included files are added to the list of files to check.  This is done recursively.  The youngest file's modification time comes out.

This script's cache directory is calculated.  The executable's time is compared: up to date or not.

Do we use the compiler to get the include dependencies or do we parse the input ourselves?

If we are not using the compiler, then we can perform the configuration step only when we need to compile.

We compile with another component, the Compiler.


## The compiler interface

At first, we may not have tricks or other options to pass to the compiler that is specific to a script.  The engine will want to be abstracted from differences between compilers.  When specific options are added, they should be added in an abstracted way.

The engine wants to abstract away:
- automatically setting warning options (-Wall -Wpedantic -Wextra)
- adding include directories
- adding static libraries
- adding dynamic libraries

This framework can be extended to abstract away more options:
- optimization level
- exception support
- others?

If warning levels are also made as options to the caller, then the compiler part of Charge becomes a useful self-sustaining project.  It might be used to create a type of agnostic compiler, a "unicompiler".  After configuration, a user may configure uniformly all compilers supported by the unicompiler.


## Parts "done" in Charge

- The parsing for "chargetricks".
- Making dependencies lists
- Launching sub-processes on Windows

The big problem for me is testing.  The sub-process testing is rather manual.  I need shell scripts for both platforms to make the tests and integrate to CMake.

C++ tests are done with Boost Test.
