
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#define ConvertNumberToString DebugIntToStringOrDashes
void ConvertNumberToString(int number);

#define NumberStringSpace gDebugNumberString
extern char NumberStringSpace[9];

#define LOWEST_DIGIT ((sizeof(NumberStringSpace) / sizeof(char)) - 2)

void DrawNumberDigitOBJ(int number, int x, int y);

#define NUMBER_WIDTH 7


void DrawUnsignedSmallNumber(int number, int x, int y)
{
  /* Draws a two-digit number < 255 as objects.
   *
   * Passing in `0xFF` or `-1` will draw `--`.
   */
  int i;
  char current;

  ConvertNumberToString(number);

  for ( i = LOWEST_DIGIT; i >= 0; i-- )
  {
    current = NumberStringSpace[i];

    if ( current == ' ' )
      return;

    DrawNumberDigitOBJ(current - '0', x, y);

    x -= NUMBER_WIDTH;
  }

  return;
}
