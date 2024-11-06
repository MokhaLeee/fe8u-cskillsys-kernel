
HPOrStatus

---

This module alternates between drawing a unit's HP (the HP label, current HP, divider, and max HP) and their status. It switches between the two every 64 frames.

The position definitions for this module refer to the position of the HP label, with the other parts drawn relative to it. If you wanted the labels/numbers/status to be drawn in independent locations, you can instead add the following definitions to your config (and edit their values accordingly):

```xml
<Definition Name="HP_LABEL_X" Value="0"/>
<Definition Name="STATUS_X" Value="0"/>

<Definition Name="CURRENT_HP_X" Value="0"/>
<Definition Name="MAX_HP_X" Value="0"/>
```

The positions for the label and status are in tiles, the positions for the numbers are in pixels.

---

Definitions:

  * `HPORSTATUS_X`: The X position of the HP label in tiles
  * `HPORSTATUS_Y`: The Y position of the HP label in tiles
  * `NUMBER_BASE_TILE`: The VRAM tile index of the first number tile
  * `NUMBER_PALETTE`: The PalRAM index of the number palette
  * `HP_LABEL_BASE_TILE`: The VRAM tile index of the HP label tiles
  * `HP_LABEL_PALETTE`: The PalRAM index of the label palette
  * `HP_DIVIDER_BASE_TILE`: The VRAM tile index of the divider tile
  * `HP_DIVIDER_PALETTE`: The PalRAM index of the label palette
  * `STATUS_BASE_TILE`: The VRAM tile index of the status tiles
  * `STATUS_PALETTE`: The PalRAM index of the status text
  * `BG_NUMBER_BASE_TILE`: The VRAM tile index of the BG number tiles
  * `BG_NUMBER_PALETTE`: The PalRAM index of the BG numbers
