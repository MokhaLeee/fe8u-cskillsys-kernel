
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define GetUnitMinimugID GetUnitMiniPortraitId

#define DrawMinimug PutFaceChibi
void DrawMinimug(int portraitID, u16* tilemapPosition, int baseTile, int paletteID, s8 isFlipped);


void MinimugStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draw the minimug the way that vanilla does.
   */

  int portraitID;
  struct Unit* unit = udp->unit;

  portraitID = GetUnitMinimugID(unit);

  // This is for Lyon
  if ( unit->state & US_BIT23 )
    portraitID++;

  DrawMinimug(
      portraitID,
      gUiTmScratchA + ((32 * MINIMUG_Y) + MINIMUG_X),
      MINIMUG_BASE_TILE,
      MINIMUG_PALETTE,
      FALSE
    );

  return;
}
