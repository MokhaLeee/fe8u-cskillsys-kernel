
HitOrAvoid

---

This module alternates between drawing a label and number for a unit's hit and avoid. It switches which stat it's drawing every 64 frames.

The position definitions for this module refer to the position of the stat label, with the stat itself drawn to the right of the label. If you wanted the labels/numbers to be drawn in independent locations, you can instead add the following definitions into your config (and edit their values accordingly):

```xml
<Definition Name="HIT_LABEL_X" Value="0"/>
<Definition Name="HIT_LABEL_Y" Value="0"/>

<Definition Name="AVO_LABEL_X" Value="0"/>
<Definition Name="AVO_LABEL_Y" Value="0"/>

<Definition Name="HIT_X" Value="0"/>
<Definition Name="HIT_Y" Value="0"/>

<Definition Name="AVO_X" Value="0"/>
<Definition Name="AVO_Y" Value="0"/>
```

The positions for the labels are in tiles, the positions for the numbers are in pixels.

---

Definitions:

  * `HITORAVO_X`: The X position of the stat label in tiles
  * `HITORAVO_Y`: The Y position of the stat label in tiles
  * `NUMBER_BASE_TILE`: The VRAM tile index of the first number tile
  * `NUMBER_PALETTE`: The PalRAM index of the number palette
  * `HIT_LABEL_BASE_TILE`: The VRAM tile index of the hit label tiles
  * `HIT_LABEL_PALETTE`: The PalRAM index of the label palette
  * `AVO_LABEL_BASE_TILE`: The VRAM tile index of the avoid label tiles
  * `AVO_LABEL_PALETTE`: The PalRAM index of the label palette
