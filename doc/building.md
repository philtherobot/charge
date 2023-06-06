# Building Charge

You need Conan and CMake.

```
mkdir build
cd build
conan install ../charge
cmake ../charge
```


## For Windows

Builds on the Windows platform commonly make use of the build type (more than other platforms).  As such, greater care must be taken.

By default, Conan will offer "release" packages and CMake will prepare makefiles for "debug", so building might be difficult out of the box.

The easiest solution is to tell CMake to make a project for the "release" build type.

```
mkdir build
cd build
conan install ..\charge
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=RELEASE ..\charge
```

The required packages are hosted on conan-center, so you need that remote to be set.

### My personal setup

I prepared a Conan profile like so (actually two, one release, one debug):

```
[settings]
os=Windows
os_build=Windows
arch=x86_64
arch_build=x86_64
compiler=Visual Studio
compiler.version=17
build_type=Release

[options]
[build_requires]
[env]
```

Then:

```
conan install .. -pr release-vc17
```

### Testing the process control functions

To complete all the tests, the BAT script located in the CMake build directory at `test_exec_win/runtests.bat` must be executed.  This test script requires that:
- The target `run_exec` is compiled.
- The target `run_process` is compiled.
- cscript.exe be available on PATH.
