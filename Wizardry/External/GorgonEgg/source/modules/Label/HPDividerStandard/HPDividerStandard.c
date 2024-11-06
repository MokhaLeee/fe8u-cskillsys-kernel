
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define HP_DIVIDER_TILE TILEREF(HP_DIVIDER_BASE_TILE, HP_DIVIDER_PALETTE)


void HPDividerStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws the `/` between current and max HP.
   */

  // Vanilla already has the correct tile in VRAM, so
  // we just draw the tilemap.

  gUiTmScratchA[TILEMAP_INDEX(HP_DIVIDER_X + 0, HP_DIVIDER_Y)] = HP_DIVIDER_TILE;

  return;
}
