#ifndef CONFIG_MEMMAP_H
#define CONFIG_MEMMAP_H

/**
 * ------------------------------------------
 * Address  | Size  | Usage
 * ------------------------------------------
 * 080E8414 | 785A8 | Kernel (.section.text)
 * 08B2A604 | D59FC | Kernel (.section.data)
 * 08EFB2E0 | E4D20 | Font   (reserved)
 * 09000000 | ----  | DEMO
 * ------------------------------------------
 */
#define FreeSpaceKernelText 0x0E8414
#define FreeSpaceKernelTextSize 0x785A8

#define FreeSpaceKernel 0xB2A604
#define FreeSpaceKernelSize 0xD59FC

#define FreeSpaceFont 0xEFB2E0
#define FreeSpaceFontSize 0xE4D20

#define FreeSpaceDEMO 0x1000000
// #define FreeSpace FreeSpaceDEMO

/**
 * Kernel memmap
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
#define KernelMagicSize 0x10
#define KernelRelocSize 0x400
#define KernelDataSize  0xCD5E0
#define MsgTableSize    0x800C

#define FreeSpaceKernel_Magic    (FreeSpaceKernel + 0x0)
#define FreeSpaceKernel_Reloc    (FreeSpaceKernel + KernelMagicSize)
#define FreeSpaceKernel_Data     (FreeSpaceKernel + KernelMagicSize + KernelRelocSize)
#define FreeSpaceKernel_MsgTable (FreeSpaceKernel + KernelMagicSize + KernelRelocSize + KernelDataSize)

#define FreeSpaceKernel_MagicEnd    (FreeSpaceKernel + KernelMagicSize)
#define FreeSpaceKernel_RelocEnd    (FreeSpaceKernel + KernelMagicSize + KernelRelocSize)
#define FreeSpaceKernel_DataEnd     (FreeSpaceKernel + KernelMagicSize + KernelRelocSize + KernelDataSize)
#define FreeSpaceKernel_MsgTableEnd (FreeSpaceKernel + KernelMagicSize + KernelRelocSize + KernelDataSize + MsgTableSize)

#endif
