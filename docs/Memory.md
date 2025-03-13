# ROM space

ROM space distribution is configured in [config-memmap.h](../include/Configs/config-memmap.h)

| Address   | Size    | Usage
| -------   | ------- | -----
| 0x00E8414 | 0x785A8 | Kernel text section (**in-BL range**)
| 0x0B2A604 | 0xD59FC | Kernel data section
| 0x0EFB2E0 | 0xE4D20 | Font
| 0x1000000 | ---     | ***reserved for DEMO***

To improve program performance, the kernel use **in-BL range** space (start from `0xE8414`) for text section, which is quite different from the custom modifications.

For other data and contents, we mainly use free-space starting from `0xB2A604`.

In order to collaborate with FEBuilderGBA and make DEMO based on kernel, we also need to define some important data at these fixed locations:

### a). Magic pattern

[A serial of characters](../main.event#L11) is set at the head of kernel free-space (`0xB2A604`), which we can use as an identifier for [FEBuilder patch](../Patches/PATCH_SkillInfo.txt#L4).

### b). Pointer list

There is a pointer list after the magic pattern, starting at `0xB2A614` with size = `0x400`. Both wizard C-hacks and FEBuilder patches can find the data via this list, so that you can expand the data via FEB.

### c). Text table

TextTable is repointed at the end of data section, [0xBFBBF4](../include/Configs/config-memmap.h#35) (the very beginning of main space) with [size = `0x1200 * sizeof(uintptr_t)`](../Repoint/RepointText/RepointText.event#L9) free space allocated.

> [!NOTE]
> For now, we use the vanilla msg data location to put kernel .text section, so that the kernel can run at in-BL range space. In exchange, the vanilla TextTable and text content have been reduced and redirected to the .data section of the kernel. Since we've only cut the story text from the original game, this won't affect custom game development.

## Font space

Free space at `0x0EFB2E0` is used to insert font data for further multi-language support, which is also a reserved space.

# RAM space

RAM space distribution is configured in [config-memmap.s](../include/Configs/config-memmap.s)

| Address    | Size    | Usage
| -------    | ------  | -----
| 0x02026E30 | 0x2028  | Kernel
| 0x0203F000 | 0x1000  | ***reserved for DEMO***

Since the entire source code is compiled at once, CHAX can offer a better Free-RAM-Space control method.
Free-RAM-Space (unused memory) from vanilla that has been previously identified by other wizards, can now can be referenced in [StanH's DOC](https://github.com/StanHash/DOC/blob/master/FREE-RAM-SPACE.md). Here we mainly use space starting at `0x02026E30` with size `0x2028`, which is the debug print buffer in vanilla (and unused).

In the kernel, free-RAM space is allocated from the bottom to the top:

```assembly
0x02026E30, FreeRamSpaceTop

<--- gKernelUsedFreeRamSpaceTop
<------------
<!used kernel space>
0x02028E58, FreeRamSpaceBottom
```

Developers should ensure that used free-RAM space does not overflow, which means asseration `(gKernelUsedFreeRamSpaceTop > FreeRamSpaceTop)` should be valid. We have also added detection for RAM space overflows, CHAX will auto detect an overflow error on [game-init](../Wizardry/Common/GameInitHook/source/GameInit.c#L14).

## Example

Here is an example to allocate RAM space in the kernel:

Suppose you want a 4 Byte RAM space (`u8 NewAlloc4Bytes[4]`)

1. Get into[config-memmap.s](../include/Configs/config-memmap.s)
2. Insert new allocation

```
_kernel_malloc NewAlloc4Bytes, 4
```
> [!WARNING]
> Make sure that the allocated space is 32 bits aligned

3. Declare the variable in your own C file, `extern u8 NewAlloc4Bytes[4];`

# Other kernel built-in RAM space usage
```
part     function name       start           end             max size    real size

[a]      ARM_MapFloodCoreRe  0x03003CAC      0x03003F94      0x2E8       0x2E8
[a]      ARM_MapTask         0x03003F94      0x03003FF0      0x05C       0x05C
[a]      ARM_SkillTester     0x03003FF0      0x03004150      0x138       0x160

[b]      ARM_UnitList        0x0300428C      0x0300438C      0x100       0xEC
[b]      ARM_SkillList       0x0300438C      0x0300448C      0x100       0xCC
[b]      __free__            ---             0x03004960      0x4D4       ---
```
