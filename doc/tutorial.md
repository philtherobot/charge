
# Tutorial

## Using a third party library

If your Charge script depends on a library, the library must be configured in Charge so Charge can issue the correct compilation command.

In your script, add a `chargetrick` to include the library during the script compilation:

```
// chargetrick import boost_filesystem
``` 

The word `boost_filesystem` is a key that makes a reference to a section inside the `libraries` section of the Charge configuration file:

```c++
libraries:
  boost_filesystem:
    header_path: /usr/lib/boost_1_69_0/boost
    static_library: /usr/lib/boost_1_69_0/lib/libboost_filesystem.a
```

Now the `chargetrick import` works and Charge will correctly link the script with the library.
