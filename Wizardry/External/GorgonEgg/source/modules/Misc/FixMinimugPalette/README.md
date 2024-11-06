
FixMinimugPalette

---

This module fixes a vanilla quirk when drawing a unit's minimug to the window. Normally, when moving between units, it takes a frame for the new unit's minimug to be drawn and overwrite the old unit's. The new palette has no such delay, however, and the old minimug will be displayed with the new palette until the graphics are copied. This module changes the palette copying step to take an additional frame in order to sync up with the graphics.

This is based on [Huichelaar's fix](https://github.com/Huichelaar/FE8U_MinimugPalette).

---

Special Definitions:

  * `gMinimugTempBuffer`: This defines space used as a temporary buffer to hold the palette

Special ASM Definitions:

  * `DecompressMinimugGraphics`: This is the ROM address of the minimug graphics decompression function
