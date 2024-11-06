
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define DEF_LABEL_TILE0 TILEREF(DEF_LABEL_BASE_TILE, DEF_LABEL_PALETTE)
#define DEF_LABEL_TILE1 TILEREF(DEF_LABEL_BASE_TILE + 1, DEF_LABEL_PALETTE)

#define RES_LABEL_TILE0 TILEREF(RES_LABEL_BASE_TILE, RES_LABEL_PALETTE)
#define RES_LABEL_TILE1 TILEREF(RES_LABEL_BASE_TILE + 1, RES_LABEL_PALETTE)

extern const u8 gDefenseLabel[];
extern const u8 gResistanceLabel[];

// These definitions are formatted this way to
// allow users to simply specify `DEFORRES_X` and `DEFORRES_Y`
// instead of defining the positions of components, but still
// allowing definitions if desired.

#ifdef DEFORRES_X

  #ifndef DEF_LABEL_X
    #define DEF_LABEL_X (DEFORRES_X + 0)
    #endif // DEF_LABEL_X

  #ifndef RES_LABEL_X
    #define RES_LABEL_X (DEFORRES_X + 0)
    #endif // RES_LABEL_X

  #ifndef DEF_X
    #define DEF_X ((DEFORRES_X * 8) + 24)
    #endif // DEF_X

  #ifndef RES_X
    #define RES_X ((DEFORRES_X * 8) + 24)
    #endif // RES_X

  #endif // DEFORRES_X

#ifdef DEFORRES_Y

  #ifndef DEF_LABEL_Y
    #define DEF_LABEL_Y (DEFORRES_Y + 0)
    #endif // DEF_LABEL_Y

  #ifndef RES_LABEL_Y
    #define RES_LABEL_Y (DEFORRES_Y + 0)
    #endif // RES_LABEL_Y

  #ifndef DEF_Y
    #define DEF_Y ((DEFORRES_Y * 8) + 0)
    #endif // DEF_Y

  #ifndef RES_Y
    #define RES_Y ((DEFORRES_Y * 8) + 0)
    #endif // RES_Y

  #endif // DEFORRES_Y


void DefenseOrResistance_Init(struct PlayerInterfaceProc* proc)
{
  /* Copies the defense and resistance labels to VRAM.
   */
  void* graphicsDest;

  graphicsDest = (void*)(((int)((DEF_LABEL_BASE_TILE * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);
  RegisterTileGraphics(gDefenseLabel, graphicsDest, (2 * CHR_SIZE));

  graphicsDest = (void*)(((int)((RES_LABEL_BASE_TILE * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);
  RegisterTileGraphics(gResistanceLabel, graphicsDest, (2 * CHR_SIZE));

  return;
}


void DefenseOrResistance_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Alternates between drawing defense and resistance.
   */

  int bufferPosition;
  int tile0, tile1;

  s16 frame = proc->hoverFramecount;

  if ( (frame & 0x40) == 0 )
  {
    bufferPosition = TILEMAP_INDEX(DEF_LABEL_X, DEF_LABEL_Y);

    tile0 = DEF_LABEL_TILE0;
    tile1 = DEF_LABEL_TILE1;
  }
  else
  {
    bufferPosition = TILEMAP_INDEX(RES_LABEL_X, RES_LABEL_Y);

    tile0 = RES_LABEL_TILE0;
    tile1 = RES_LABEL_TILE1;
  }

  // Copy to both the scratchpad and full tilemap buffer,
  // and then flag the buffer to be copied.

  gUiTmScratchA[bufferPosition] = tile0;
  gUiTmScratchA[bufferPosition + TILEMAP_INDEX(1, 0)] = tile1;

  gBg0MapBuffer[bufferPosition] = tile0;
  gBg0MapBuffer[bufferPosition + TILEMAP_INDEX(1, 0)] = tile1;

  EnableBgSyncByMask(BG0_SYNC_BIT);

  if ( proc->busyFlag )
    return;

  if ( (frame & 0x40) == 0 )
    GenericNumberTwoDigits(udp->battleDefense, DEF_X, DEF_Y, proc);

  else
    GenericNumberTwoDigits(GetUnitResistance(udp->unit), RES_X, RES_Y, proc);

  return;
}
