# GBA emulator DebugKit

- Include *DebugKit.event* in your EA buildfile.
- Include dependence, *../GameInitHook/GameInitHook.event* in your EA buildfile.
- Include header *debug-kit.h* in your *C* source code.
- Now you can use function `Printf`, `Debugf`, `Assert` etc to communicate with emulaters' log system (*mgba* and *No$GBA*).
