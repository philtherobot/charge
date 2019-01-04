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
