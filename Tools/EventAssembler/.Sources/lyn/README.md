# `lyn`

Aims to bring the functionalities of a proper linker to EA.

(My goal will be to progressively port different EA-outputting tools to object-outputting tools, as well as allowing `lyn` to output directly to binary, so that using EA will eventually become a non-requirement.)

What `lyn` does is it takes any number of [ELF object files](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format) plus a bunch of options, and outputs labeled EA code to stdout.

See the [FEU Thread](http://feuniverse.us/t/ea-asm-tool-lyn-elf2ea-if-you-will/2986?u=stanh) for a detailed walkthrough and other goodies.

## Usage

```
lyn [-[no]link] [-[no]longcalls] [-[no]temp] [-[no]hook] [-raw] <elf...>
```

(parameters, including elf file references, can be arranged in any order)

- `-[no]link` specifies whether anticipated linking is to occur (elves linking between themselves) (default is on)
- `-[no]longcalls` specifies whether lyn should transform any relative jump/reference to an absolute one (useful when linking code using `bl`s to symbols out of range) (default is off)
- `-[no]temp` specifies whether lyn should keep & print *all* local symbols (when off, only the ones needed for relocation are kept) (default is off)
- `-[no]hook` specifies whether automatic routine replacement hooks should be inserted (this happens when an object-relative symbol and an absolute symbol in two different elves have the same name, then lyn will output a "hook" to where the absolute symbol points to that will jump to the object-relative location) (default is on)

## Building

You need [CMake](https://cmake.org/) and a working C++11 compiler.

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

## Planned features

- Bugfixes (always)
- A partial rewrite that would imply:
  - Decoupling the object interface with the event-gerenating part (and then put the object bit into a reusable library (to make writing other object-generating tools))
  - Decoupling the object interface with the elf-reading bit, allowing me to define other ways of reading objects.
- Rewrite the way relocations are handled. The current handler was written back when I didn't have a very good understanding of the way relocations work.
- Allowing handling not only only read-only allocated sections (`.text`, `.rodata`, etc) but also writable allocated sections (`.data`, `.bss`, etc). Idk how that'd work with EA tho...
- Cleanup, clarify and expand on the "auto hook" feature. Allow "special sections" that would be a way to encode data to output at a specific location in the target binary.
- Allowing other output formats:
  - Object files (in which case `lyn`'s role would be to merge/simplify objects rather than "translating" them)
  - I'm thinking [`json-bpatch`](https://github.com/zahlman/json_bpatch) since zahl himself mentionned being interested.
  - **Direct output to target binary, and allowing some kind of control over how to do that.**

**If you have any questions, feel free to ask in the [FEU Thread](http://feuniverse.us/t/ea-asm-tool-lyn-elf2ea-if-you-will/2986?u=stanh) or on the [FEU Discord](http://feuniverse.us/t/feu-discord-server/1480?u=stanh).**
