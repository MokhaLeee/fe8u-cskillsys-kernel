
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define HIT_LABEL_TILE0 TILEREF(HIT_LABEL_BASE_TILE, HIT_LABEL_PALETTE)
#define HIT_LABEL_TILE1 TILEREF(HIT_LABEL_BASE_TILE + 1, HIT_LABEL_PALETTE)

#define AVO_LABEL_TILE0 TILEREF(AVO_LABEL_BASE_TILE, AVO_LABEL_PALETTE)
#define AVO_LABEL_TILE1 TILEREF(AVO_LABEL_BASE_TILE + 1, AVO_LABEL_PALETTE)

extern const u8 gHitLabel[];
extern const u8 gAvoidLabel[];

// These definitions are formatted this way to
// allow users to simply specify `HITORAVO_X` and `HITORAVO_Y`
// instead of defining the positions of components, but still
// allowing definitions if desired.

#ifdef HITORAVO_X

  #ifndef HIT_LABEL_X
    #define HIT_LABEL_X (HITORAVO_X + 0)
    #endif // HIT_LABEL_X

  #ifndef AVO_LABEL_X
    #define AVO_LABEL_X (HITORAVO_X + 0)
    #endif // AVO_LABEL_X

  #ifndef HIT_X
    #define HIT_X ((HITORAVO_X * 8) + 24)
    #endif // HIT_X

  #ifndef AVO_X
    #define AVO_X ((HITORAVO_X * 8) + 24)
    #endif // AVO_X

  #endif // HITORAVO_X

#ifdef HITORAVO_Y

  #ifndef HIT_LABEL_Y
    #define HIT_LABEL_Y (HITORAVO_Y + 0)
    #endif // HIT_LABEL_Y

  #ifndef AVO_LABEL_Y
    #define AVO_LABEL_Y (HITORAVO_Y + 0)
    #endif // AVO_LABEL_Y

  #ifndef HIT_Y
    #define HIT_Y ((HITORAVO_Y * 8) + 0)
    #endif // HIT_Y

  #ifndef AVO_Y
    #define AVO_Y ((HITORAVO_Y * 8) + 0)
    #endif // AVO_Y

  #endif // HITORAVO_Y


void HitOrAvoid_Init(struct PlayerInterfaceProc* proc)
{
  /* Copies the hit and avoid labels to VRAM.
   */
  void* graphicsDest;

  graphicsDest = (void*)(((int)((HIT_LABEL_BASE_TILE * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);
  RegisterTileGraphics(gHitLabel, graphicsDest, (2 * CHR_SIZE));

  graphicsDest = (void*)(((int)((AVO_LABEL_BASE_TILE * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);
  RegisterTileGraphics(gAvoidLabel, graphicsDest, (2 * CHR_SIZE));

  return;
}


void HitOrAvoid_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Alternates between drawing hit and avoid.
   */

  int bufferPosition;
  int tile0, tile1;

  s16 frame = proc->hoverFramecount;

  if ( (frame & 0x40) == 0 )
  {
    bufferPosition = TILEMAP_INDEX(HIT_LABEL_X, HIT_LABEL_Y);

    tile0 = HIT_LABEL_TILE0;
    tile1 = HIT_LABEL_TILE1;
  }
  else
  {
    bufferPosition = TILEMAP_INDEX(AVO_LABEL_X, AVO_LABEL_Y);

    tile0 = AVO_LABEL_TILE0;
    tile1 = AVO_LABEL_TILE1;
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
    GenericNumberThreeDigits(udp->battleHitRate, HIT_X, HIT_Y, proc);

  else
    GenericNumberThreeDigits(udp->battleAvoidRate, AVO_X, AVO_Y, proc);

  return;
}


