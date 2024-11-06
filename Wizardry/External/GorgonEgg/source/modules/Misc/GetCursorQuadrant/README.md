
GetCursorQuadrant

---

This module overrides the vanilla cursor quadrant check, which is used to determine when UI elements should get out of the cursor's way when moving it around. This change allows for windows to be larger than a quarter of the screen in size without the cursor overlapping them.

By default, the 'quadrant' that the cursor is considered to be in is determined by the size of the window (using the `GE_WIDTH` and `GE_HEIGHT` definitions, which ideally should be defined by whatever module determines the shape/size of the window). If you wish to manually override the thresholds needed to be considered on the top/bottom or left/right sides of the screen, this module provides the following definitions:

```xml
<Definition Name="X_THRESHOLD" Value="0"/>
<Definition Name="Y_THRESHOLD" Value="0"/>

```

The values for these thresholds are in pixels.
