
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

// This is the total number of possible states, using the vanilla
// HP bar graphics. The vanilla endcap tiles only allow for 5 pixels
// of bar each, so that's the reason why we subtract two tiles
// and then add (5 * 2). The 8 is the number of pixels per tile.
#define HPBAR_LEVELS (((HPBAR_WIDTH - 2) * 8) + (5 * 2))

void DrawBarStandard(u16* tilemapPosition, s16 filled, int tiles, int tileBase);


void HPBarStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws an HP bar for the unit the way vanilla does.
   */
  struct Unit* unit = udp->unit;

  int hpRatio = Div(
      HPBAR_LEVELS * GetUnitCurrentHp(unit),
      GetUnitMaxHp(unit)
    );

  DrawBarStandard(
      gUiTmScratchA + TILEMAP_INDEX(HPBAR_X, HPBAR_Y),
      hpRatio,
      HPBAR_WIDTH,
      TILEREF(HPBAR_BASE_TILE, HPBAR_PALETTE)
    );

  return;
}
