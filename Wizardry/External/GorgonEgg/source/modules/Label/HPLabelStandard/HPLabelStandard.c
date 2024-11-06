
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define HP_LABEL_TILE0 TILEREF(HP_LABEL_BASE_TILE, HP_LABEL_PALETTE)
#define HP_LABEL_TILE1 TILEREF(HP_LABEL_BASE_TILE + 1, HP_LABEL_PALETTE)


void HPLabelStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draw a label for HP.
   */

  // Vanilla already has the tiles in VRAM,
  // so we just have to draw the tilemap.

  gUiTmScratchA[TILEMAP_INDEX(HP_LABEL_X + 0, HP_LABEL_Y)] = HP_LABEL_TILE0;
  gUiTmScratchA[TILEMAP_INDEX(HP_LABEL_X + 1, HP_LABEL_Y)] = HP_LABEL_TILE1;

  return;
}
