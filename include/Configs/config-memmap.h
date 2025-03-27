#ifndef CONFIG_MEMMAP_H
#define CONFIG_MEMMAP_H

/**
 * ------------------------------------------
 * Address  | Size  | Usage
 * ------------------------------------------
 * 080E8414 | 785A8 | Kernel .text
 * 089875AC | 0F000 | Kernel .rodata (secure)
 * 08B2A604 | D59FC | user configurable .rodata (none-secure)
 * 08EFB2E0 | E4D20 | Font .rodata
 * 09000000 | ----  | _unused_
 * ------------------------------------------
 */
#define KernelSpace_Text 0x0E8414
#define KernelSpace_TextSize 0x785A8

#define KernelSpace_Data 0x9875AC
#define KernelSpace_DataSize 0xF000

#define FreeSpaceCommon 0xB2A604
#define FreeSpaceCommonSize 0xD59FC

#define FreeSpaceFont 0xEFB2E0
#define FreeSpaceFontSize 0xE4D20

#define FreeSpaceDEMO 0x1000000
// #define FreeSpace FreeSpaceDEMO

/**
 * Secure rodata memmap
 *
 * ------------------------------
 * Name      | Address | Size
 * ------------------------------
 * Magic     | 9875AC  | 00010
 * Reloc     | 9875BC  | 00400
 * Main      | 9879BC  | 0EBF0
 * ------------------------------
 */
#define KernelMagicSize 0x10
#define KernelRelocSize 0x400
#define KernelKDataSize 0xEBF0

#define KernelSpace_Magic (KernelSpace_Data + 0x0)
#define KernelSpace_Reloc (KernelSpace_Data + KernelMagicSize)
#define KernelSpace_KData (KernelSpace_Data + KernelMagicSize + KernelRelocSize)

#define KernelSpace_MagicEnd (KernelSpace_Data + KernelMagicSize)
#define KernelSpace_RelocEnd (KernelSpace_Data + KernelMagicSize + KernelRelocSize)
#define KernelSpace_KDataEnd (KernelSpace_Data + KernelMagicSize + KernelRelocSize + KernelKDataSize)

/**
 * None-secure rodata memmap
 *
 * ------------------------------
 * Name      | Address | Size
 * ------------------------------
 * Magic     | B2A604  | 00010
 * Reloc     | B2A614  | 00400
 * Main      | B2AA14  | CD5E0
 * MSG table | BF7FF4  | 0800C
 * ------------------------------
 */
#define UsrMagicSize 0x10
#define UsrRelocSize 0x400
#define UsrDataSize  0xCD5E0
#define MsgTableSize 0x800C

#define FreeSpace_Magic    (FreeSpaceCommon + 0x0)
#define FreeSpace_Reloc    (FreeSpaceCommon + UsrMagicSize)
#define FreeSpace_UsrData  (FreeSpaceCommon + UsrMagicSize + UsrRelocSize)
#define FreeSpace_MsgTable (FreeSpaceCommon + UsrMagicSize + UsrRelocSize + UsrDataSize)

#define FreeSpace_MagicEnd    (FreeSpaceCommon + UsrMagicSize)
#define FreeSpace_RelocEnd    (FreeSpaceCommon + UsrMagicSize + UsrRelocSize)
#define FreeSpace_UsrDataEnd  (FreeSpaceCommon + UsrMagicSize + UsrRelocSize + UsrDataSize)
#define FreeSpace_MsgTableEnd (FreeSpaceCommon + UsrMagicSize + UsrRelocSize + UsrDataSize + MsgTableSize)

#endif
