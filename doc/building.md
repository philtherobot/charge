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

### Testing the process control functions

To complete all the tests, the BAT script `charge_engine/test/exec_win/runtests.bat` must be executed.  This test scripts requires that:
- The target `run_exec` is compiled.
- The target `run_process` is compiled.
- cscript.exe be available on PATH.
- The environment variable `BUILD_ROOT` be set to either `cmake-build-debug` or `cmake-build-release`.
 
