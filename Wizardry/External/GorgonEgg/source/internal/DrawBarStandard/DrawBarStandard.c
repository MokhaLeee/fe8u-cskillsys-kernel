
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

// These values are how many different filled states
// the three kinds of bar tiles can have.
#define LEFT_CAP_LEVELS 5
#define MIDDLE_LEVELS 8
#define RIGHT_CAP_LEVELS 5

// These values are the positions vanilla graphics' different
// types of bar tiles, measured from the start of the bar graphics in tiles.
// In addition to the tiles with filled portions, each type begins
// with an empty variant.
#define LEFT_CAP_START 0
#define MIDDLE_START (LEFT_CAP_START + LEFT_CAP_LEVELS + 1)
#define RIGHT_CAP_START (MIDDLE_START + MIDDLE_LEVELS + 1)

#define LEFT_CAP_EMPTY LEFT_CAP_START
#define LEFT_CAP_FULL  (LEFT_CAP_START + LEFT_CAP_LEVELS)

#define MIDDLE_EMPTY MIDDLE_START
#define MIDDLE_FULL  (MIDDLE_START + MIDDLE_LEVELS)

#define RIGHT_CAP_EMPTY RIGHT_CAP_START
#define RIGHT_CAP_FULL  (RIGHT_CAP_START + RIGHT_CAP_LEVELS)


void DrawBarStandardLeftCap(u16* tilemapPosition, s16 filled, int baseTile)
{
  /* Draws the first tile of a bar.
   */

  // Draw a full segment if we have more than one to fill.
  s16 level = (filled > LEFT_CAP_LEVELS) ? LEFT_CAP_FULL : filled + LEFT_CAP_START;

  *tilemapPosition = baseTile + level;

  return;
}


void DrawBarStandardMiddle(u16* tilemapPosition, s16 filled, int tiles, int baseTile)
{
  /* Draws the middle of a bar.
   */

  int current;

  int fullTiles = filled >> 3; // Divide by 8 but sign is an issue
  int remaining = filled & 7;

  // `tiles` here is the total number of middle tiles,
  // and we need to place down tiles for all of them,
  // even if they're not all `filled`.

  for ( current = 0; current < tiles ; current++ )
  {

    if ( current < fullTiles )
      *tilemapPosition = baseTile + MIDDLE_FULL;

    else if ( current == fullTiles )
      *tilemapPosition = baseTile + MIDDLE_START + remaining;

    else
      *tilemapPosition = baseTile + MIDDLE_EMPTY;

    tilemapPosition++;
  }

  return;
}


void DrawBarStandardRightCap(u16* tilemapPosition, s16 filled, int baseTile)
{
  /* Draws the last tile of a bar.
   */

  // Draw a full segment if we are full.
  s16 level = (filled >= RIGHT_CAP_LEVELS) ? RIGHT_CAP_FULL : filled + RIGHT_CAP_START;

  // If we ran out of levels to draw before getting here,
  // draw an empty tile.
  if ( filled < 0 )
    level = RIGHT_CAP_EMPTY;

  *tilemapPosition = baseTile + level;

  return;
}


void DrawBarStandard(u16* tilemapPosition, s16 filled, int tiles, int baseTile)
{
  /* Draws a bar in a way that is compatible with vanilla tiles.
   */

  DrawBarStandardLeftCap(tilemapPosition, filled, baseTile);

  tilemapPosition += 1;
  filled -= LEFT_CAP_LEVELS;

  // The middle occupies the width of the bar minus
  // the two endcaps.

  DrawBarStandardMiddle(tilemapPosition, filled, (tiles - 2), baseTile);

  tilemapPosition += (tiles - 2);
  filled -= ((tiles - 2) * MIDDLE_LEVELS);

  DrawBarStandardRightCap(tilemapPosition, filled, baseTile);

  return;
}
