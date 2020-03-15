
# Charge Reference

## Configuration

The configuration file is located at `$HOME/.charge` (`%USERPROFILE%\.charge` on Windows).

The configuration file is a [YAML](https://yaml.org/) file.

### Top level keys

`compiler`: compiler configuration

`libraires`: libraries configured that charge scripts can compile with.


#### Compiler configuration

The top level key `compiler` contains these keys:

- `family`: a string with a value of one of:
  - `msvc`
  - `gcc`

- `command`: the shell string for the compiler exectuble.  Example: `/usr/bin/g++4.9`.


#### Libraries

The top level key `libraries` follows this structure:
- keys for each of the configured libraries, as set by the user.
- within a library, a set of options required to compile against the library.

Use a `header_path` key to add a directory to search for included headers.

Use a `lib_path` key to add a directory to search for library files (such as `*.lib `).

Use a `static_library` key to link with a static library.  The value is the file path of the library.

## Tricks

### Import library

The `import` trick makes Charge compile the script with the configured library (see Configuration).

## Command-line options

To Be Completed.
