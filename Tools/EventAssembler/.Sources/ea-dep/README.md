# `ea-dep`

Simple file dependency listing utility for EA events. This is supposed to be a simpler (and faster) alternative to using a custom EA build or [pea](https://github.com/StanHash/pea-preprocessor).

This will analyse the given event file, and list all files that are included (or incbinned) by that file or the files it includes. The list is printed in a simple one-filename-per-line format.

## Usage

    ea-dep <input> [-I <path>]... [--add-missings] [--add-externals]

- `<input>`: This is the event file to analyse (for example: your `ROM Buildfile.event`).
- `-I <path>`: Adds `<path>` to the list of external include paths to check (this would typically only include the path to the EA distribution directory, containing `EAstdlib.event`).
  - `-I <path>` can be repeated to add multiple paths.
- `--add-missings`: if this option is given, missing files (included files that do not exist) will still be added to the list (useful when using the list to determine which files need to be generated).
- `--add-externals`: if this option is given, files included from external include paths will be added to the list.

The list is printed to stdout.

## Building

You need CMake, a C++14 compiler and RE2C.

```
mkdir build
cd build
cmake ..
cmake --build .
```
