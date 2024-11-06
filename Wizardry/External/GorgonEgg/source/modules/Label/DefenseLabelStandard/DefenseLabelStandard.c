
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define DEF_LABEL_TILE0 TILEREF(DEF_LABEL_BASE_TILE, DEF_LABEL_PALETTE)
#define DEF_LABEL_TILE1 TILEREF(DEF_LABEL_BASE_TILE + 1, DEF_LABEL_PALETTE)

extern const u8 gDefenseLabel[];


void DefenseLabelStandard_Init(struct PlayerInterfaceProc* proc)
{
  /* Copies the Defense label to VRAM.
   */

  void* graphicsDest = (void*)(((int)((DEF_LABEL_BASE_TILE * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);

  RegisterTileGraphics(gDefenseLabel, graphicsDest, (2 * CHR_SIZE));

  return;
}


void DefenseLabelStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draw a label for Defense.
   */

  gUiTmScratchA[TILEMAP_INDEX(DEF_LABEL_X + 0, DEF_LABEL_Y)] = DEF_LABEL_TILE0;
  gUiTmScratchA[TILEMAP_INDEX(DEF_LABEL_X + 1, DEF_LABEL_Y)] = DEF_LABEL_TILE1;

  return;
}
