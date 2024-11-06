
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define AVO_LABEL_TILE0 TILEREF(AVO_LABEL_BASE_TILE, AVO_LABEL_PALETTE)
#define AVO_LABEL_TILE1 TILEREF(AVO_LABEL_BASE_TILE + 1, AVO_LABEL_PALETTE)

extern const u8 gAvoidLabel[];


void AvoidLabelStandard_Init(struct PlayerInterfaceProc* proc)
{
  /* Copies the Avoid label to VRAM.
   */

  void* graphicsDest = (void*)(((int)((AVO_LABEL_BASE_TILE * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);

  RegisterTileGraphics(gAvoidLabel, graphicsDest, (2 * CHR_SIZE));

  return;
}


void AvoidLabelStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draw a label for Avoid.
   */

  gUiTmScratchA[TILEMAP_INDEX(AVO_LABEL_X + 0, AVO_LABEL_Y)] = AVO_LABEL_TILE0;
  gUiTmScratchA[TILEMAP_INDEX(AVO_LABEL_X + 1, AVO_LABEL_Y)] = AVO_LABEL_TILE1;

  return;
}
