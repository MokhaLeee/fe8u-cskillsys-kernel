
# Setting up

You need to install [the latest .NET SDK](https://docs.microsoft.com/en-us/dotnet/core/install/linux) to build ColorzCore.

In addition to the .NET SDK, you will need to install the following packages (here I am using Debian/Ubuntu/Mint names, adapt for your own distro):

- `build-essential`, `cmake`, `re2c` for compress, lyn and ea-dep.
- `ghc`, `cabal-install`, `libghc-vector-dev`, `libghc-juicypixels-dev` for the EA Formatting Suite (ParseFile, Png2Dmp and PortraitFormatter).

On Debian/Ubuntu/Mint, you can install all of that using a single command:

```bash
sudo apt install \
  build-essential cmake re2c \
  ghc cabal-install libghc-vector-dev libghc-juicypixels-dev
```

# Building

Simply run the `build.sh` script included at the root of this repository.

# Included tools

- an Event Assembler distribution including:
  - ColorzCore, the current preffered Core for EA Buildfiles.
  - Stan's exprimental updated standard library.
  - The EA Formatting Suite (which includes ParseFile, Png2Dmp and PortraitFormatter).
  - a compress tool for fast lz77 compression.
  - lyn.
- ea-dep, an utility that allows you to list the dependencies of a full buildfile.

# Expected directory structure

- `.Sources` (dir)
  - ... (source files for all of the components of this distribution)
- `ColorzCore`
- `EAstdlib.event`
- `EA Standard Library` (dir)
  - ...
- `Language Raws` (dir)
  - ...
- `Extensions` (dir)
  - ...
- `Tools` (dir)
  - `ParseFile`
  - `Png2Dmp`
  - `PortraitFormatter`
  - `compress`
  - `lyn`
- `ea-dep`
