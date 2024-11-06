
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

// These should be defined by the config/tilemap module
// but in case they were not:

#ifndef GE_WIDTH
  #define GE_WIDTH 13
  #endif // GE_WIDTH

#ifndef GE_HEIGHT
  #define GE_HEIGHT 6
  #endif // GE_HEIGHT

/* In vanilla these split the screen evenly,
 * but we're hijacking this to make the left/top
 * quadrants to reflect if the cursor would be
 * over the window.
 */

#ifndef X_THRESHOLD
  #define X_THRESHOLD (GE_WIDTH * 8)
  #endif // X_THRESHOLD

#ifndef Y_THRESHOLD
  #define Y_THRESHOLD (GE_HEIGHT * 8)
  #endif // Y_THRESHOLD

enum CursorQuadrants
{
  QUADRANT_TOPLEFT     = 0,
  QUADRANT_TOPRIGHT    = 1,
  QUADRANT_BOTTOMLEFT  = 2,
  QUADRANT_BOTTOMRIGHT = 3,
};


int GetCursorQuadrant(void)
{
  /* Checks which quadrant the cursor is over.
   */

  int cursorX;
  int camX;
  int cursorY;
  int camY;

  int x;
  int y;

  int quadrant = QUADRANT_TOPLEFT;

  cursorX = (gGameState.cursorMapPos.x * 16);
  camX = (gGameState.cameraRealPos.x - 8);

  x = cursorX - camX;

  cursorY = (gGameState.cursorMapPos.y * 16);
  camY = (gGameState.cameraRealPos.y - 8);

  y = cursorY - camY;

  if ((x < (X_THRESHOLD + 1)) && (y < (Y_THRESHOLD + 1)))
  {
    quadrant = QUADRANT_TOPLEFT;
  }

  if ((x >= (X_THRESHOLD + 1)) && (y < (Y_THRESHOLD + 1)))
  {
    quadrant = QUADRANT_TOPRIGHT;
  }

  if ((x < (X_THRESHOLD + 1)) && (y >= (Y_THRESHOLD + 1)))
  {
    quadrant = QUADRANT_BOTTOMLEFT;
  }

  if ((x >= (X_THRESHOLD + 1)) && (y >= (Y_THRESHOLD + 1)))
  {
    quadrant = QUADRANT_BOTTOMRIGHT;
  }

  return quadrant;
}
