# Charge

Charge compiles *and runs* a single-file C++ program.  The resulting binary is cached.  Charge performs dependency checking so future invocations are fast (Charge skips compilation when the dependencies are unchanged).

Workflow:

1. Code with a text editor.
2. Save your code to file.
3. In the shell: `charge pgm.cpp`.

Command-line arguments are passed to the executable so your C++ script can receive them:

```
$ charge add.cpp 1 2
1 + 2 is 3

$
```


## What is it for?

Charge is *not* for industrial strength programming.  Charge is limited to small scripts: as soon as a script grows to medium size, you will need to use a classical build system and deployment.

Charge can be used to:

- Program in your favorite language (C++!) more often.
- Quickly test an hypothesis.  If you do not need to actually execute the script, it might now be even quicker and more powerful to use [Compiler Explorer](https://godbolt.org/).
- Code up a one-use script that will be more comfortable to write in C++ versus the other choices (Bash, Python, etc).
- Get away from shell quoting, word splitting and quote removal issues.
- Write small utility scripts and scripts you use daily.  You may accumulate over time a small library of useful tools that make this easier.


## Requirements

Charge runs on Windows and POSIX platforms such as Linux.  It will automatically detect and use one of these compilers if they are installed:

- Microsoft Visual Studio
- GNU C++
- clang


## Installation

Get the sources and build them.  See [building](doc/building.md).  Place the `charge` executable in your path.


## Configuration

Charge finds its configuration in the `.charge` directory located in the user's home directory.  There, it reads a [YAML](http://yaml.org) file named `config`.

```yaml
compiler:
  family: msvc
  command: cl

libraries:
  boost_filesystem:
    header_path: C:\Users\philt\dev\boost_1_65_1
    lib_path: C:\Users\philt\dev\boost_1_65_1\lib64-msvc-14.1
```

See the [reference](doc/reference.md) for details.


## Using libraries

It is highly likely that the scripts you want to create will need to include and/or link to one or more additional libraries.  You can specify libraries to link with and header paths to add with "charge tricks".  Here is an example script:

```c++
// chargetrick import boost_filesystem

#include <boost/filesystem.hpp>

#include <iostream>

int main(int argc, char ** args)
{
    if (argc < 1) return 1;
    boost::filesystem::path p = args[1];

    std::cout << p << '\n';
    std::cout << boost::filesystem::file_size(p) << '\n';

    return 0;
}
```

The C++ comment `// chargetrick import boost_filesystem` is seen by Charge before the script is launched.  If the script needs recompilation, Charge will look in its configuration in the `libraries` top level key for `boost_filesystem`.  There it expects to find the information to properly configure the compiler to compile the script.


## Options


### Force compilation `-f`

Charge will not perform an "up to date" check and will compile the script unconditionally prior to executing it normally.


### Update `-u`

Charge will ensure the cached executable is up to date with the sources like usual but will not launch the script.  This option is useful to ensure the next launch will be quick.


## Environment

Charge sets up an environment variable named `CHARGE_SCRIPT`.  This variable is accessible from the script when it executes.  `CHARGE_SCRIPT` contains the filepath to the script.


## How does it work?

Charge compiles the script using the configured compiler.  The intermediary file and executable file are stored in a cache located in the `.charge/cache` directory in the user's home directory.  The cache is keyed by the hostname and the absolute filepath of the source file.  Because the hostname is part of the cache key, the cache will work correctly if your home directory is shared across hosts.

Charge gets the header dependency information from the configured compiler.

The "is up to date" check is performed by comparing file modification times.


## Limitations and working around them

Charge can only build scripts made of a single *compilation unit*: one source file as input.  What if you have a library of functions you want to share across many scripts?  You will have to place those functions, with their definitions, in a separate file and use an `#include` directive to bring in the functions in each of your scripts.

Your script will receive the arguments the user gave on the command-line when the script was invoked.  But the very first one, argument zero, which is usually the script's filename, will not be correct.  Argument zero will be the cached executable filename.  A workaround is to read the `CHARGE_SCRIPT` environment variable that has been setup for the executing program.  The variable contains the filepath of the script given to the `charge` command.

Charge uses the file modification times to verify if the script needs to be recompiled.  Because file times have limited precision (which depends on a variety of factors), if a compilation-modification-dependency check cycle is done very quickly, the dependency check could fail and declare that the file was not changed.  This can occur in scenarios of scripts being launch by another script very quickly.  For example, if a main script generates a sub script based on some input and launches it, the source file will be compiled and its cached executable will have a file modification time equal to the time of compilation.  In this scenario, the main script, upon return of the sub script, rewrites the sub script to something different.  If the main script accomplished this quickly enough that the new sub script has the same file modification time as the cached executable, then charge's dependency will fail by declaring that the cache is up to date.  To workaround this:
- Make the main script wait for at least your file system's time precision.
- Change the source file's name at every generation, or every X generation (like "f1" to "f10" then cycle back).
- Since the main script knows it changed the source file, it could launch it with the "force compilation" option (-f).

Charge cannot reliably handle multiple invocations of the same script at the same time that would result in a compilation.  The mutiple executing `charge` invocations will all try to compile the cached executable and write to the same output file at the same time which will cause a compilation failure.  Possible workaround are:
- Do nothing: maybe you can handle/ignore a failure like this from time to time.  
- Use the update option (-u) to ensure the cached executable is up to date before launching the multiprocessing system.


## Future

### Important

- Find or write a small companion library that makes it easy to do things that are already easy in shell scripting: launching subprocesses, piping to/from them, manipulating environment variables, etc.

### Wishes

- Add an option to compile the executable and place it in the current directory.
- Try to spoof argument zero.  Or offer a library that the script can use to recover the script's filename.
