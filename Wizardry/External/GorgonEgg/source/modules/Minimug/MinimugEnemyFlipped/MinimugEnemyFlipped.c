
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define GetUnitMinimugID GetUnitMiniPortraitId

#define DrawMinimug PutFaceChibi
void DrawMinimug(int portraitID, u16* tilemapPosition, int baseTile, int paletteID, s8 isFlipped);


void MinimugEnemyFlipped_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draw the minimug, flipping enemies.
   */

  int portraitID;
  int isFlipped = FALSE;
  struct Unit* unit = udp->unit;

  portraitID = GetUnitMinimugID(unit);

  // This is for Lyon
  if ( unit->state & US_BIT23 )
    portraitID++;

  if ( UNIT_FACTION(unit) == FACTION_RED )
    isFlipped = TRUE;

  DrawMinimug(
      portraitID,
      gUiTmScratchA + ((32 * MINIMUG_Y) + MINIMUG_X),
      MINIMUG_BASE_TILE,
      MINIMUG_PALETTE,
      isFlipped
    );

  return;
}
