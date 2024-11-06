
DefenseOrResistance

---

This module alternates between drawing a label and number for a unit's defense and resistance. It switches which stat it's drawing every 64 frames.

The position definitions for this module refer to the position of the stat label, with the stat itself drawn to the right of the label. If you wanted the labels/numbers to be drawn in independent locations, you can instead add the following definitions into your config (and edit their values accordingly):

```xml
<Definition Name="DEF_LABEL_X" Value="0"/>
<Definition Name="DEF_LABEL_Y" Value="0"/>

<Definition Name="RES_LABEL_X" Value="0"/>
<Definition Name="RES_LABEL_Y" Value="0"/>

<Definition Name="DEF_X" Value="0"/>
<Definition Name="DEF_Y" Value="0"/>

<Definition Name="RES_X" Value="0"/>
<Definition Name="RES_Y" Value="0"/>
```

The positions for the labels are in tiles, the positions for the numbers are in pixels.

---

Definitions:

  * `DEFORRES_X`: The X position of the stat label in tiles
  * `DEFORRES_Y`: The Y position of the stat label in tiles
  * `NUMBER_BASE_TILE`: The VRAM tile index of the first number tile
  * `NUMBER_PALETTE`: The PalRAM index of the number palette
  * `DEF_LABEL_BASE_TILE`: The VRAM tile index of the defense label tiles
  * `DEF_LABEL_PALETTE`: The PalRAM index of the label palette
  * `RES_LABEL_BASE_TILE`: The VRAM tile index of the resistance label tiles
  * `RES_LABEL_PALETTE`: The PalRAM index of the label palette
