
TilemapZane

---

This module copies a tilemap made by Zane. It uses tiles that are in VRAM at build time.

It also handles copying the tilemap's palette. The window's palette is based on the selected unit's faction, as it is in vanilla.

When editing the width and height, be sure to also change them in the .tmx file.

---

Definitions:

  * `GE_WIDTH`: The width of the tilemap in tiles
  * `GE_HEIGHT`: The height of the tilemap in tiles
  * `GE_TILEMAP_PALETTE`: The PalRAM index of the palette

Special Definitions:

  * `gPal_PlayerInterface_Blue`: The C declaration for the blue palette
  * `gPal_PlayerInterface_Red`: The C declaration for the red palette
  * `gPal_PlayerInterface_Green`: The C declaration for the green palette

Special ASM Definitions:

  * `gPal_PlayerInterface_Blue`: The ROM location of the red palette
  * `gPal_PlayerInterface_Red`: The ROM location of the blue palette
  * `gPal_PlayerInterface_Green`: The ROM location of the green palette
