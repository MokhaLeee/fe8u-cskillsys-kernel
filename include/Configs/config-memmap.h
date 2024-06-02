#ifndef CONFIG_MEMMAP_H
#define CONFIG_MEMMAP_H

/**
 * ---------------------------
 * Address  | Size  | Usage
 * ---------------------------
 * 08B2A604 | D5DFC  | Kernel
 * 08EFB2E0 | E4D20  | Data 
 * 09000000 | ----   | DEMO
 * ---------------------------
 */
#define FreeSpaceKernel 0xB2A604
#define FreeSpaceKernelSize 0xD5DFC

#define FreeSpaceFont 0xEFB2E0
#define FreeSpaceFontSize 0xE4D20

#define FreeSpaceDEMO 0x1000000
// #define FreeSpace FreeSpaceDEMO

/**
 * Kernel memmap
 *
 * ------------------------
 * Name    | Offset | Size
 * ------------------------
 * Magic   | 0000   | 0010
 * Reloc   | 0010   | 0400
 * Main    | ----   | ----
 * ------------------------
 */
#define FreeSpaceKernel_Magic (FreeSpaceKernel + 0x0)
#define FreeSpaceKernel_Reloc (FreeSpaceKernel_Magic + 0x10)
#define FreeSpaceRelocSize 0x400

#define FreeSpaceKernelMain (FreeSpaceKernel_Reloc + FreeSpaceRelocSize)

#endif
