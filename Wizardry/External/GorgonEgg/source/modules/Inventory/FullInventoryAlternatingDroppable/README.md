
FullInventoryAlternatingDroppable

---

This module draws icons for the unit's inventory. It draws a single icon and alternates which item in the unit's inventory that is being drawn every 64 frames. If the unit has the droppable item status flag set, their last item will be drawn with a droppable item icon.

---

Definitions:

  * `INVENTORY_ICON_X`: The X coordinate of the icon in pixels
  * `INVENTORY_ICON_Y`: The Y coordinate of the icon in pixels
  * `INVENTORY_ICON_BASE_TILE`: This is the starting index of where the icons will be drawn in VRAM
  * `INVENTORY_ICON_PALETTE`: This is the index of the palette slot to use
  * `DROPPABLE_ICON_BASE_TILE`: This is the tile index of where the droppable item icon will be drawn in VRAM
  * `DROPPABLE_ICON_PALETTE`: This is the index of the palette slot to use for the droppale item icon
  * `ITEM_ICON_PAGE`: If the icon rework is enabled, the value of this can be nonzero and reflects the page that weapon icons are part of
