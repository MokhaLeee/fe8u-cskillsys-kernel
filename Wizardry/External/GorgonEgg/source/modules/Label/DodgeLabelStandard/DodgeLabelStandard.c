
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define DOD_LABEL_TILE0 TILEREF(DOD_LABEL_BASE_TILE, DOD_LABEL_PALETTE)
#define DOD_LABEL_TILE1 TILEREF(DOD_LABEL_BASE_TILE + 1, DOD_LABEL_PALETTE)

extern const u8 gDodgeLabel[];


void DodgeLabelStandard_Init(struct PlayerInterfaceProc* proc)
{
  /* Copies the Dodge label to VRAM.
   */

  void* graphicsDest = (void*)(((int)((DOD_LABEL_BASE_TILE * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);

  RegisterTileGraphics(gDodgeLabel, graphicsDest, (2 * CHR_SIZE));

  return;
}


void DodgeLabelStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draw a label for Dodge.
   */

  gUiTmScratchA[TILEMAP_INDEX(DOD_LABEL_X + 0, DOD_LABEL_Y)] = DOD_LABEL_TILE0;
  gUiTmScratchA[TILEMAP_INDEX(DOD_LABEL_X + 1, DOD_LABEL_Y)] = DOD_LABEL_TILE1;

  return;
}
