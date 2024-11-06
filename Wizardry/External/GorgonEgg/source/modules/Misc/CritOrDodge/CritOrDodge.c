
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define CRT_LABEL_TILE0 TILEREF(CRT_LABEL_BASE_TILE, CRT_LABEL_PALETTE)
#define CRT_LABEL_TILE1 TILEREF(CRT_LABEL_BASE_TILE + 1, CRT_LABEL_PALETTE)

#define DOD_LABEL_TILE0 TILEREF(DOD_LABEL_BASE_TILE, DOD_LABEL_PALETTE)
#define DOD_LABEL_TILE1 TILEREF(DOD_LABEL_BASE_TILE + 1, DOD_LABEL_PALETTE)

extern const u8 gCritLabel[];
extern const u8 gDodgeLabel[];

// These definitions are formatted this way to
// allow users to simply specify `CRTORDOD_X` and `CRTORDOD_Y`
// instead of defining the positions of components, but still
// allowing definitions if desired.

#ifdef CRTORDOD_X

  #ifndef CRT_LABEL_X
    #define CRT_LABEL_X (CRTORDOD_X + 0)
    #endif // CRT_LABEL_X

  #ifndef DOD_LABEL_X
    #define DOD_LABEL_X (CRTORDOD_X + 0)
    #endif // DOD_LABEL_X

  #ifndef CRT_X
    #define CRT_X ((CRTORDOD_X * 8) + 24)
    #endif // CRT_X

  #ifndef DOD_X
    #define DOD_X ((CRTORDOD_X * 8) + 24)
    #endif // DOD_X

  #endif // CRTORDOD_X

#ifdef CRTORDOD_Y

  #ifndef CRT_LABEL_Y
    #define CRT_LABEL_Y (CRTORDOD_Y + 0)
    #endif // CRT_LABEL_Y

  #ifndef DOD_LABEL_Y
    #define DOD_LABEL_Y (CRTORDOD_Y + 0)
    #endif // DOD_LABEL_Y

  #ifndef CRT_Y
    #define CRT_Y ((CRTORDOD_Y * 8) + 0)
    #endif // CRT_Y

  #ifndef DOD_Y
    #define DOD_Y ((CRTORDOD_Y * 8) + 0)
    #endif // DOD_Y

  #endif // CRTORDOD_Y


void CritOrDodge_Init(struct PlayerInterfaceProc* proc)
{
  /* Copies the crit and dodge labels to VRAM.
   */
  void* graphicsDest;

  graphicsDest = (void*)(((int)((CRT_LABEL_BASE_TILE * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);
  RegisterTileGraphics(gCritLabel, graphicsDest, (2 * CHR_SIZE));

  graphicsDest = (void*)(((int)((DOD_LABEL_BASE_TILE * CHR_SIZE) + VRAM) & 0x1FFFF) + VRAM);
  RegisterTileGraphics(gDodgeLabel, graphicsDest, (2 * CHR_SIZE));

  return;
}


void CritOrDodge_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Alternates between drawing crit and dodge.
   */

  int bufferPosition;
  int tile0, tile1;

  s16 frame = proc->hoverFramecount;

  if ( (frame & 0x40) == 0 )
  {
    bufferPosition = TILEMAP_INDEX(CRT_LABEL_X, CRT_LABEL_Y);

    tile0 = CRT_LABEL_TILE0;
    tile1 = CRT_LABEL_TILE1;
  }
  else
  {
    bufferPosition = TILEMAP_INDEX(DOD_LABEL_X, DOD_LABEL_Y);

    tile0 = DOD_LABEL_TILE0;
    tile1 = DOD_LABEL_TILE1;
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
    GenericNumberThreeDigits(udp->battleCritRate, CRT_X, CRT_Y, proc);

  else
    GenericNumberThreeDigits(udp->battleDodgeRate, DOD_X, DOD_Y, proc);

  return;
}


