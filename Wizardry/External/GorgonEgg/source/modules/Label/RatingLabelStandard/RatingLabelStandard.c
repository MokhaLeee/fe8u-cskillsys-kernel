
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define RTG_LABEL_TILE0 TILEREF(RTG_LABEL_BASE_TILE, RTG_LABEL_PALETTE)
#define RTG_LABEL_TILE1 TILEREF(RTG_LABEL_BASE_TILE + 1, RTG_LABEL_PALETTE)

extern const u8 gRatingLabel[];


void RatingLabelStandard_Init(struct PlayerInterfaceProc* proc)
{
  /* Copies the Rating label to VRAM.
   */

  void* graphicsDest = (void*)(((int)((RTG_LABEL_BASE_TILE * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);

  RegisterTileGraphics(gRatingLabel, graphicsDest, (2 * CHR_SIZE));

  return;
}


void RatingLabelStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draw a label for Rating.
   */

  gUiTmScratchA[TILEMAP_INDEX(RTG_LABEL_X + 0, RTG_LABEL_Y)] = RTG_LABEL_TILE0;
  gUiTmScratchA[TILEMAP_INDEX(RTG_LABEL_X + 1, RTG_LABEL_Y)] = RTG_LABEL_TILE1;

  return;
}
