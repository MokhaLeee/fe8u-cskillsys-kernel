
CritOrDodge

---

This module alternates between drawing a label and number for a unit's crit and dodge (crit avoid). It switches which stat it's drawing every 64 frames.

The position definitions for this module refer to the position of the stat label, with the stat itself drawn to the right of the label. If you wanted the labels/numbers to be drawn in independent locations, you can instead add the following definitions into your config (and edit their values accordingly):

```xml
<Definition Name="CRT_LABEL_X" Value="0"/>
<Definition Name="CRT_LABEL_Y" Value="0"/>

<Definition Name="DOD_LABEL_X" Value="0"/>
<Definition Name="DOD_LABEL_Y" Value="0"/>

<Definition Name="CRT_X" Value="0"/>
<Definition Name="CRT_Y" Value="0"/>

<Definition Name="DOD_X" Value="0"/>
<Definition Name="DOD_Y" Value="0"/>
```

The positions for the labels are in tiles, the positions for the numbers are in pixels.

---

Definitions:

  * `CRTORDOD_X`: The X position of the stat label in tiles
  * `CRTORDOD_Y`: The Y position of the stat label in tiles
  * `NUMBER_BASE_TILE`: The VRAM tile index of the first number tile
  * `NUMBER_PALETTE`: The PalRAM index of the number palette
  * `CRT_LABEL_BASE_TILE`: The VRAM tile index of the crit label tiles
  * `CRT_LABEL_PALETTE`: The PalRAM index of the label palette
  * `DOD_LABEL_BASE_TILE`: The VRAM tile index of the dodge label tiles
  * `DOD_LABEL_PALETTE`: The PalRAM index of the label palette
