
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#ifndef TILEMAP_DESTINATION
  /* This dummy macro will have its definition replaced
   * with a series of function pointers, terminated with NULL.
   * These functions are then called like
   * coords = SomeFunction(proc, coords);
   * for each <TilemapDestination> tag in a module.
   * Generally, there should be exactly one (from
   * whatever handles the window tilemap).
   */
  #define TILEMAP_DESTINATION NULL
  #endif // TILEMAP_DESTINATION

// These definitions should be overridden by a module or
// the config, but are defined here for safety.

#ifndef GE_WIDTH
  #define GE_WIDTH 13
  #endif // GE_WIDTH

#ifndef GE_HEIGHT
  #define GE_HEIGHT 6
  #endif // GE_HEIGHT

typedef struct Vec2 (*copytilemap_func) (struct PlayerInterfaceProc* proc, struct Vec2 coords);


const copytilemap_func gCopyTilemapFunctions[] = {
  TILEMAP_DESTINATION
};


void GE_CopyTilemap(struct PlayerInterfaceProc* proc)
{
  /* Copy the entire window tilemap to the BG0 buffer.
   */

  int i;
  copytilemap_func copytilemap;

  struct Vec2 coords = {0, 0};

  for ( i = 0, copytilemap = gCopyTilemapFunctions[i]; copytilemap != NULL; i++, copytilemap = gCopyTilemapFunctions[i] )
  {
    coords = copytilemap(proc, coords);
  }

  BgMapCopyRect(
      gUiTmScratchA,
      gBg0MapBuffer + TILEMAP_INDEX(coords.x, coords.y),
      GE_WIDTH, GE_HEIGHT
    );
  EnableBgSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT);

  return;
}
