
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define AS_LABEL_TILE0 TILEREF(AS_LABEL_BASE_TILE, AS_LABEL_PALETTE)
#define AS_LABEL_TILE1 TILEREF(AS_LABEL_BASE_TILE + 1, AS_LABEL_PALETTE)

extern const u8 gAttackSpeedLabel[];


void AttackSpeedLabelStandard_Init(struct PlayerInterfaceProc* proc)
{
  /* Copies the AttackSpeed label to VRAM.
   */

  void* graphicsDest = (void*)(((int)((AS_LABEL_BASE_TILE * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);

  RegisterTileGraphics(gAttackSpeedLabel, graphicsDest, (2 * CHR_SIZE));

  return;
}


void AttackSpeedLabelStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draw a label for AttackSpeed.
   */

  gUiTmScratchA[TILEMAP_INDEX(AS_LABEL_X + 0, AS_LABEL_Y)] = AS_LABEL_TILE0;
  gUiTmScratchA[TILEMAP_INDEX(AS_LABEL_X + 1, AS_LABEL_Y)] = AS_LABEL_TILE1;

  return;
}
