
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define HIT_LABEL_TILE0 TILEREF(HIT_LABEL_BASE_TILE, HIT_LABEL_PALETTE)
#define HIT_LABEL_TILE1 TILEREF(HIT_LABEL_BASE_TILE + 1, HIT_LABEL_PALETTE)

extern const u8 gHitLabel[];


void HitLabelStandard_Init(struct PlayerInterfaceProc* proc)
{
  /* Copies the Hit label to VRAM.
   */

  void* graphicsDest = (void*)(((int)((HIT_LABEL_BASE_TILE * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);

  RegisterTileGraphics(gHitLabel, graphicsDest, (2 * CHR_SIZE));

  return;
}


void HitLabelStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draw a label for Hit.
   */

  gUiTmScratchA[TILEMAP_INDEX(HIT_LABEL_X + 0, HIT_LABEL_Y)] = HIT_LABEL_TILE0;
  gUiTmScratchA[TILEMAP_INDEX(HIT_LABEL_X + 1, HIT_LABEL_Y)] = HIT_LABEL_TILE1;

  return;
}
