
HPBarStandard

---

This module draws a bar that shows the unit's current HP/max HP. This bar's appearance matches the bar of the vanilla window.

It uses a palette and tiles that are already in PalRAM/VRAM when the window is built.

---

Definitions:

  * `HPBAR_X`: The X coordinate of the bar relative to the window in tiles
  * `HPBAR_Y`: The Y coordinate of the bar relative to the window in tiles
  * `HPBAR_WIDTH`: The width of the bar in tiles
  * `HPBAR_BASE_TILE`: The VRAM tile index of the first tile of the bar graphics
  * `HPBAR_PALETTE`: The PalRAM index of the bar's palette
