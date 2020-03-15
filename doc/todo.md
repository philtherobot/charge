# Things to do

## Complete the reference for command line options

Medium size.
Medium priority.

The top-level README has a description of the options, but the reference, for completeness, also needs to cover them.  I have left a "To be completed" note in `reference.md`.

## gcc support

Large size.
High priority.

This is high priority because the documentation mentions gcc support.  I got ahead of myself, and the actual implementation does not support the doc.  An option is to remove gcc from the documentation.  Which in turns means to forget all pretense of being multi-platform.  I simply made the mistake of documenting things I had not implemented.

Add support for gcc.  We need auto-detection.  Command-line writing.  Documentation.

## clang support

Large size.
Low priority.

Add support for clang.  We need auto-detection.  Command-line writing.  Documentation.

## Start from zero tutorial

Medium size.

The tutorial needs to start with an "Hello World" example.  Use to editor to write a simple program.  What is the command-line to launch it.

## Document compiler defines for libraries in settings

Medium size.
High priority.

Charge supports the `defines` setting in a library definition in the configuration.  This needs to be documented.  I think only the reference needs to talk about `defines`.

## Programs in the the project share a similar main function

Medium size.
Medium priority.

There are three programs (`charge`, `run_exec` and `run_process`) that implement the same pattern of main with `try/catch` calling `main_really`.  Make this a reusable component.

## ShellProcess and exec

Small size.
High priority.

In the Windows implementation, exec is not implemented from ShellProcess.  There is duplication of somewhat similar nasty OS-specific code.

## Config should get its own interface

Large size.
Medium priority.

Right now, `YAML::Node` is exposed directly, so we have no firewall between it and our code.  The `Config` object is a `typedef` to `YAML::Node`.

Make a configuration interface class.  Make an implementation of that interface over a YAML file.  Add a depencency injection container that provides access to the configuration object.

## Unit test initialization file has confusing name

Small size.
Low priority.

`charge_engine/test/test_initialization.cpp` is not a test, it is the implementation file that bootstraps the Boost Unit Test.  Named as it is, it looks like the tests for a component named "initialization".


## Script depends on its libraries

Medium size.
Low priority.

If a static library (linked through `chargetrick import` with `static_library`) is younger than the compiled executable, relink (or recompile if we must) the script.  


## Compilation should be dependent on Charge's version

Medium size.
Low priority.

New version might have different options for the compiler, so an executable in the cache might be out of date when charge is upgraded.

We need to version the cache (which version of Charge was used to generate it).


## Finding the script filepath via CHARGE_SCRIPT

Small size.
High priority.

This is high priority because the documentation mentions this in more than a single place and now its either change the doc once to remove this and then again to put it back.

In order for running scripts to find where the source script is located, setup an environment variable named `CHARGE_SCRIPT` in the running script.

## Test that all three STDIO work normally

Medium size.
High priority.

Need to test that piping to and from a script work properly:

```
charge pgm.cpp < files > output
```

Do not forget stderr.

### Notes

This could be done at the level of the `charge` executable itself, for a full integration test.  This can also be done at the level of `charge::exec`.  I think the latter is best because then we are concentrating all the nasty shell and BAT scripts-based tests to that component.  Afterwards, we will assume the rest of the `charge` process is not interfering with the pipes.


## Complete the ShellProcess tests for POSIX

Large size.
High priority.

The shell scripts to test `charge::exec` are done (state remains to be verified) but the `ShellProcess` tests are not completed.  See `charge_engine/test/exec_posix/shell_process`.


## make_absolute_path

Medium size.
Low priority.

`make_absolute_path` has a reference to "C:\".  At least the function is limited to tests, but this is still an issue. What if the host does not have a "C" drive?


## Enable stricter warnings

Medium size.
High priority.

Compilation of Charge should be done at an higher level of warnings.  Change the CMake project.


## Scripts are compiled at a strict warning level

Medium size.
Medium priority.

Compilation of *scripts* should be done at an higher level of warnings.

## Disable MSVC "secure no warnings" for scripts

Small size.
Medium priority.

If a script uses a C++ API that is deemed unsafe by MSVC and the warning level is raised to three, the compiler will issue a "not secure" warning.  For example:

```
securenowarning.cpp(6): warning C4996: 'localtime': This function or variable may be unsafe. Consider using localtime_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
C:\Program Files (x86)\Windows Kits\10\include\10.0.17763.0\ucrt\time.h(506): note: see declaration of 'localtime'
```

Because scripts are meant to be portable, we want to avoid such differences between platforms.  Charge wants to remove as many of these things as possible.  Charge can be seen as a frontend that uniformizes compilers.

With this in mind, what is the correct course of action?  I think we want to offer a modern and safe compilation, so a elevated warning level will be used.  I recommend it myself for MSVC as there are important warnings at level four.  So it seems like we will encounter the "unsafe function" warning.  So that seems like a good idea.

## Check configuration error reporting 

Small size.
High priority.

I had a small "to do" about "YAML exception messages are useless".  I have since forgotten what this was about.  Review what happens when YAML throws exceptions.  More importantly, review what happens when the configuration file is badly formatted.  What kind of error does the user get?  Something useful?

## Decide if implementation details should go in a special namespace

Small size.
High priority.

I need to make a decision about where implementation details go.

Should we have a "details" namespace where things such as "ReadableStream", which is not part of the public API of Charge but rather an implementation detail, are placed?

Should we simply rely on "public" versus "private" headers?  I think not because the references made in the code do not show if an identifier is part of the public API or an implementation detail.

