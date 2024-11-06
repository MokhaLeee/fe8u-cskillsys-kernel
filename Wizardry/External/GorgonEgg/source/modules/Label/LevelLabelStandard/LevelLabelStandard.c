
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define LEVEL_LABEL_TILE0 TILEREF(LEVEL_LABEL_BASE_TILE, LEVEL_LABEL_PALETTE)
#define LEVEL_LABEL_TILE1 TILEREF(LEVEL_LABEL_BASE_TILE + 1, LEVEL_LABEL_PALETTE)

extern const u8 gLevelLabel[];


void LevelLabelStandard_Init(struct PlayerInterfaceProc* proc)
{
  /* Copies the level label to VRAM.
   */

  void* graphicsDest = (void*)(((int)((LEVEL_LABEL_BASE_TILE * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);

  RegisterTileGraphics(gLevelLabel, graphicsDest, (2 * CHR_SIZE));

  return;
}


void LevelLabelStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draw a label for Level.
   */

  gUiTmScratchA[TILEMAP_INDEX(LEVEL_LABEL_X + 0, LEVEL_LABEL_Y)] = LEVEL_LABEL_TILE0;
  gUiTmScratchA[TILEMAP_INDEX(LEVEL_LABEL_X + 1, LEVEL_LABEL_Y)] = LEVEL_LABEL_TILE1;

  return;
}
