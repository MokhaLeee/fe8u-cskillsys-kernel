# What is CHAX

It is origined from [StanHash/FE-CHAX](https://github.com/StanHash/FE-CHAX) with the main purpose to write EngineHacks in C rather than ASM. The early work mainly focus on how to achieve elf2ea (or say, [lyn utilly](https://feuniverse.us/t/ea-asm-tool-lyn-elf2ea-if-you-will/2986)). With the progress on [Decomp](https://github.com/FireEmblemUniverse/fireemblem8u) and other tools such as [ea-dep, a dependence generator](https://github.com/StanHash/ea-dep), now we may go further with the following goals:

1. All components are all generated at once via a `make` command, rather than we compile each *.c* or *.s* file before intergrating to ColorzCore, which requires a better sorting on makefile dependency.
2. Alignment on CHAX and Decomp, which means we could use the same header files and directly port source code from decomp output for our hack.
3. More C-Hacks and less ASM-Hacks.

# Memory

Since all source codes are all compiled at once, CHAX can offer a better Free-RAM-Space control method.
Free-RAM-Space, unused memory from vanilla is collected by wizardries and now can be refered by [StanH's DOC](https://github.com/StanHash/DOC/blob/master/FREE-RAM-SPACE.md). Here we mainly use space start at `0x02026E30` with size `0x2028`, which is the debug print buffer in vanilla (and unused). They are all defined in [./Configs/usr-defined.ref.s](../Configs/usr-defined.ref.s).

Suppose you want a 4 Byte RAM space (`u8 NewAlloc4Bytes[4]`), you need to do as following:

1. Get into [./Configs/usr-defined.ref.s](../Configs/usr-defined.ref.s).
2. Replace `FreeRamSpaceTail` to `NewAlloc4Bytes`.
3. Update `FreeRamSpaceTail` to the end of `NewAlloc4Bytes` (aka `SET_DATA FreeRamSpaceTail, NewAlloc4Bytes + 0x4`).
4. Declare such variable in your own C file, `extern u8 NewAlloc4Bytes[4];`

CHAX may auto detect on overflow error on game-init.

# Support

For now, unit support progress is located in BWL table, and the vanilla support table is used to save skills. If unit does not have BWL, (aka, unit character index > 0x45), we may directly use the pre-load supports, which means its support level may not develop in game playing.
