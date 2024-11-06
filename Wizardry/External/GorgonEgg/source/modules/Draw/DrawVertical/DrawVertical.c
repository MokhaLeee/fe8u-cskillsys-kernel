
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

extern const struct ProcInstruction gProc_TI[];

void GE_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp);

struct UnitDataProc* GetUnitDataProc(struct PlayerInterfaceProc* proc);

extern const u16 GE_Tilemap[];

#define CallARM_FillTileRect BgMap_ApplyTsa
#define TileMap_FillRect BgMapFillRect
#define TileMap_CopyRect BgMapCopyRect

// These should be defined by the config/tilemap module
// but in case they were not:

#ifndef GE_WIDTH
  #define GE_WIDTH 13
  #endif // GE_WIDTH

#ifndef GE_HEIGHT
  #define GE_HEIGHT 6
  #endif // GE_HEIGHT


struct Vec2 DrawVertical_TilemapDestination(struct PlayerInterfaceProc* proc, struct Vec2 coords)
{
  /* Copy the full tilemap to the same spot that vanilla does.
   */

  coords.x = (
    ( sPlayerInterfaceConfigLut[proc->cursorQuadrant].xMinimug < 0 )
    ? 0
    : (DISPLAY_WIDTH_TILES + 1) - GE_WIDTH
    );

  coords.y = (
    ( sPlayerInterfaceConfigLut[proc->cursorQuadrant].yMinimug < 0 )
    ? 0
    : DISPLAY_HEIGHT_TILES - GE_HEIGHT
    );

  return coords;
}


bool DrawVertical_InvalidPosition(struct PlayerInterfaceProc* proc, int quadrant, bool invalidPosition)
{
  /* Avoid drawing the window on top of the terrain window.
   */

  struct PlayerInterfaceProc* tiProc = (struct PlayerInterfaceProc*)ProcFind(gProc_TI);

  if ((tiProc) && (tiProc->windowQuadrant > -1) && (tiProc->windowQuadrant == quadrant))
    invalidPosition = TRUE;

  return invalidPosition;
}


const u8 gDrawVerticalExtendLUT[] = {GE_HEIGHT / 4, GE_HEIGHT / 2, (GE_HEIGHT * 3) / 4, GE_HEIGHT};


void DrawVertical_Extend(struct PlayerInterfaceProc* proc)
{
  /* Extend the window from the top/bottom of the screen
   */

  int tilemapIndex;
  int height;
  int x, y;

  y = (
    ( sPlayerInterfaceConfigLut[proc->cursorQuadrant].yMinimug < 0 )
    ? 0
    : DISPLAY_HEIGHT_TILES - GE_HEIGHT
    );

  x = (
    ( sPlayerInterfaceConfigLut[proc->cursorQuadrant].xMinimug < 0 )
    ? 0
    : DISPLAY_WIDTH_TILES - GE_WIDTH
    );

  tilemapIndex = TILEMAP_INDEX(x, y);

  TileMap_FillRect(gBg0MapBuffer + tilemapIndex, GE_WIDTH, GE_HEIGHT, TILEREF(0, 0));
  TileMap_FillRect(gBg1MapBuffer + tilemapIndex, GE_WIDTH, GE_HEIGHT, TILEREF(0, 0));

  EnableBgSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT);

  height = gDrawVerticalExtendLUT[proc->movementFrame];

  tilemapIndex = TILEMAP_INDEX(x, 0);

  if ( sPlayerInterfaceConfigLut[proc->cursorQuadrant].yMinimug < 0 )
  {
    TileMap_CopyRect(gUiTmScratchA + TILEMAP_INDEX(x, GE_HEIGHT - height), gBg0MapBuffer + tilemapIndex, GE_WIDTH, height);
    TileMap_CopyRect(gUiTmScratchB + TILEMAP_INDEX(x, GE_HEIGHT - height), gBg1MapBuffer + tilemapIndex, GE_WIDTH, height);
  }
  else
  {
    TileMap_CopyRect(gUiTmScratchA, gBg0MapBuffer + TILEMAP_INDEX(x, DISPLAY_HEIGHT_TILES - height), GE_WIDTH, height);
    TileMap_CopyRect(gUiTmScratchB, gBg1MapBuffer + TILEMAP_INDEX(x, DISPLAY_HEIGHT_TILES - height), GE_WIDTH, height);
  }

  proc->movementFrame++;

  if (proc->movementFrame == (sizeof(gDrawVerticalExtendLUT) / sizeof(s8)))
  {
    proc->busyFlag = FALSE;
    proc->movementFrame = 0;

    BreakProcLoop((void*)proc);

    GE_Dynamic(proc, GetUnitDataProc(proc));
  }

  return;
}


const u8 gDrawVerticalRetractLUT[] = {(GE_HEIGHT * 3) / 4, GE_HEIGHT / 2, 0};


void DrawVertical_Retract(struct PlayerInterfaceProc* proc)
{
  /* Retract the window from the top/bottom of the screen
   */

  int tilemapIndex;
  int height;
  int x, y;

  proc->busyFlag = TRUE;

  y = (
    ( sPlayerInterfaceConfigLut[proc->cursorQuadrant].yMinimug < 0 )
    ? 0
    : DISPLAY_HEIGHT_TILES - GE_HEIGHT
    );

  x = (
    ( sPlayerInterfaceConfigLut[proc->cursorQuadrant].xMinimug < 0 )
    ? 0
    : DISPLAY_WIDTH_TILES - GE_WIDTH
    );

  tilemapIndex = TILEMAP_INDEX(x, y);

  TileMap_FillRect(gBg0MapBuffer + tilemapIndex, GE_WIDTH, GE_HEIGHT, TILEREF(0, 0));
  TileMap_FillRect(gBg1MapBuffer + tilemapIndex, GE_WIDTH, GE_HEIGHT, TILEREF(0, 0));

  EnableBgSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT);

  height = gDrawVerticalRetractLUT[proc->movementFrame];

  tilemapIndex = TILEMAP_INDEX(x, 0);

  if ( sPlayerInterfaceConfigLut[proc->cursorQuadrant].yMinimug < 0 )
  {
    TileMap_CopyRect(gUiTmScratchA + TILEMAP_INDEX(x, GE_HEIGHT - height), gBg0MapBuffer + tilemapIndex, GE_WIDTH, height);
    TileMap_CopyRect(gUiTmScratchB + TILEMAP_INDEX(x, GE_HEIGHT - height), gBg1MapBuffer + tilemapIndex, GE_WIDTH, height);
  }
  else
  {
    TileMap_CopyRect(gUiTmScratchA, gBg0MapBuffer + TILEMAP_INDEX(x, DISPLAY_HEIGHT_TILES - height), GE_WIDTH, height);
    TileMap_CopyRect(gUiTmScratchB, gBg1MapBuffer + TILEMAP_INDEX(x, DISPLAY_HEIGHT_TILES - height), GE_WIDTH, height);
  }

  proc->movementFrame++;

  if (proc->movementFrame == (sizeof(gDrawVerticalRetractLUT) / sizeof(s8)))
  {
    proc->isRetracting = FALSE;
    proc->movementFrame = 0;
    proc->cursorQuadrant = -1;

    BreakProcLoop((void*)proc);
  }

  return;
}
