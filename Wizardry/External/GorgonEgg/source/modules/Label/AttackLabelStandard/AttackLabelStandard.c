
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define ATK_LABEL_TILE0 TILEREF(ATK_LABEL_BASE_TILE, ATK_LABEL_PALETTE)
#define ATK_LABEL_TILE1 TILEREF(ATK_LABEL_BASE_TILE + 1, ATK_LABEL_PALETTE)

extern const u8 gAttackLabel[];


void AttackLabelStandard_Init(struct PlayerInterfaceProc* proc)
{
  /* Copies the Attack label to VRAM.
   */

  void* graphicsDest = (void*)(((int)((ATK_LABEL_BASE_TILE * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);

  RegisterTileGraphics(gAttackLabel, graphicsDest, (2 * CHR_SIZE));

  return;
}


void AttackLabelStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draw a label for Attack.
   */

  gUiTmScratchA[TILEMAP_INDEX(ATK_LABEL_X + 0, ATK_LABEL_Y)] = ATK_LABEL_TILE0;
  gUiTmScratchA[TILEMAP_INDEX(ATK_LABEL_X + 1, ATK_LABEL_Y)] = ATK_LABEL_TILE1;

  return;
}
