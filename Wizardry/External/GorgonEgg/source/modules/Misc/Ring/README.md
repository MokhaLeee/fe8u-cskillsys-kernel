
Ring

---

This module draws a ring around the currently-selected unit. This ring displays the unit's HP (as a Diablo-style globe) and inventory item icons radially around the ring.

---

Definitions:

  * `RING_BASE_TILE`: The VRAM tile index of the first tile of the ring graphics
  * `RING_PALETTE`: The PalRAM index of the ring palette
  * `INVENTORY_ICON_BASE_TILE`: The VRAM tile index of the first tile of the item icons
  * `INVENTORY_ICON_PALETTE`: The PalRAM index of the item icon palette
  * `ITEM_ICON_PAGE`: If the icon rework is enabled, the value of this can be nonzero and reflects the page that weapon icons are part of

Special Definitions:

  * `UPDATE_ON_QUADRANT_CHANGE`: Set to FALSE in order to prevent the ring from being redrawn when selecting adjacent units in different quadrants
