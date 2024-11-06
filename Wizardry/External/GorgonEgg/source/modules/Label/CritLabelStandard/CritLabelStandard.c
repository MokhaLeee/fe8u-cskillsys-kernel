
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define CRT_LABEL_TILE0 TILEREF(CRT_LABEL_BASE_TILE, CRT_LABEL_PALETTE)
#define CRT_LABEL_TILE1 TILEREF(CRT_LABEL_BASE_TILE + 1, CRT_LABEL_PALETTE)

extern const u8 gCritLabel[];


void CritLabelStandard_Init(struct PlayerInterfaceProc* proc)
{
  /* Copies the Crit label to VRAM.
   */

  void* graphicsDest = (void*)(((int)((CRT_LABEL_BASE_TILE * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);

  RegisterTileGraphics(gCritLabel, graphicsDest, (2 * CHR_SIZE));

  return;
}


void CritLabelStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draw a label for Crit.
   */

  gUiTmScratchA[TILEMAP_INDEX(CRT_LABEL_X + 0, CRT_LABEL_Y)] = CRT_LABEL_TILE0;
  gUiTmScratchA[TILEMAP_INDEX(CRT_LABEL_X + 1, CRT_LABEL_Y)] = CRT_LABEL_TILE1;

  return;
}
