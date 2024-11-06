
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define RES_LABEL_TILE0 TILEREF(RES_LABEL_BASE_TILE, RES_LABEL_PALETTE)
#define RES_LABEL_TILE1 TILEREF(RES_LABEL_BASE_TILE + 1, RES_LABEL_PALETTE)

extern const u8 gResistanceLabel[];


void ResistanceLabelStandard_Init(struct PlayerInterfaceProc* proc)
{
  /* Copies the resistance label to VRAM.
   */

  void* graphicsDest = (void*)(((int)((RES_LABEL_BASE_TILE * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);

  RegisterTileGraphics(gResistanceLabel, graphicsDest, (2 * CHR_SIZE));

  return;
}


void ResistanceLabelStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draw a label for Resistance.
   */

  gUiTmScratchA[TILEMAP_INDEX(RES_LABEL_X + 0, RES_LABEL_Y)] = RES_LABEL_TILE0;
  gUiTmScratchA[TILEMAP_INDEX(RES_LABEL_X + 1, RES_LABEL_Y)] = RES_LABEL_TILE1;

  return;
}
