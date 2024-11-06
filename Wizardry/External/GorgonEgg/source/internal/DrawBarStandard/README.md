
This file provides a way to draw stat bars to the window. It expects that the bar graphics are already in VRAM and are in the same order and layout as the vanilla bar tiles.

Modules using this should use
```xml
<Internal File="DrawBarStandard/DrawBarStandard.lyn.event"/>
```
in their module declaration files and should call `DrawBarStandard` to draw a bar.

For example:
```c

int filled = 10;
int max = 20;

// It's recommended that your module have definitions for these.
// See `modules/Bar/HPBarStandard/HPBarStandard.xml` for an example.

#define BAR_WIDTH 5
#define BAR_X 1
#define BAR_Y 1
#define BAR_BASE_TILE 0x0140
#define BAR_PALETTE 1

// See `modules/Bar/HPBarStandard/HPBarStandard.c` for an explanation of this.

#define BAR_LEVELS (((BAR_WIDTH - 2) * 8) + (5 * 2))

int ratio = Div(filled * BAR_LEVELS, max);

DrawBarStandard(
    gUiTmScratchA + TILEMAP_INDEX(BAR_X, BAR_Y),
    ratio,
    BAR_WIDTH,
    TILEREF(BAR_BASE_TILE, BAR_PALETTE)
  );

```
