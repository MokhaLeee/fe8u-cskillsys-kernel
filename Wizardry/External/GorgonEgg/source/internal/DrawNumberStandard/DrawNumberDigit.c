
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define DIGIT_BASE_TILE (OAM2_CHR(NUMBER_BASE_TILE) | OAM2_LAYER(0) | OAM2_PAL(NUMBER_PALETTE))


void DrawNumberDigitOBJ(int number, int x, int y)
{
  /* Draws a single digit or dash as an object.
   *
   * Pass in 0-9 for digits or 10 for a dash.
   */

  CallARM_PushToSecondaryOAM(x, y, &gObj_8x8, DIGIT_BASE_TILE + number);

  return;
}
