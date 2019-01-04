
# Charge Reference

## Configuration


### Top level keys

`compiler`: compiler configuration

`libraires`: libraries configured that charge scripts can compile with.


#### Compiler configuration

The top level key `compiler` contains these keys:

- `family`: a string with a value of one of:
  - `msvc`
  - `gcc`
  - `clang`

- `command`: a string of the shell command of the compiler.


#### Libraries

The top level key `libraries` follow this structure:
- keys for each of the configured libraries, as set by the user.
- within a library, a set of options required of compile against the library.

Use a `header_path` key to add a directory to search for included headers in.

Use a `lib_path` key to add a directory to search for library files (such as `\*.lib `) in.


## Tricks

### Import library

The `import` trick makes Charge compile the script with use the configured library (see Configuration).

