
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#ifndef INVALID_POSITION_CALLS
  /* This dummy macro will have its definition replaced
   * with a series of function pointers, terminated with NULL.
   * These functions are then called like
   * invalidPosition |= SomeFunction(proc, quadrant, invalidPosition);
   * for each <InvalidPosition> tag in a module.
   * Generally, there should be only one
   * (from whatever handles extending/retracting).
   */
  #define INVALID_POSITION_CALLS NULL
  #endif // INVALID_POSITION_CALLS

void GE_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp);
void GE_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp);

struct UnitDataProc* GetUnitDataProc(struct PlayerInterfaceProc* proc);

typedef bool (*invalidposition_func) (struct PlayerInterfaceProc* proc, int quadrant, bool invalidPosition);


const invalidposition_func gInvalidPositionFunctions[] = {
  INVALID_POSITION_CALLS
};


void GE_Reset(struct PlayerInterfaceProc* proc)
{
  /* Check if the window needs to be rebuilt.
   */

  int i;
  invalidposition_func invalid;

  struct UnitDataProc* udp;
  int invalidPosition, quadrant;

  struct Unit* unit = GetUnitAtCursor();

  if ( !unit )
    return;

  proc->busyFlag = TRUE;
  proc->cursorQuadrant = GetCursorQuadrant();

  quadrant = GetWindowQuadrant(
      sPlayerInterfaceConfigLut[proc->cursorQuadrant].xMinimug,
      sPlayerInterfaceConfigLut[proc->cursorQuadrant].yMinimug
    );

  invalidPosition = FALSE;

  for ( i = 0, invalid = gInvalidPositionFunctions[i]; invalid != NULL; i++, invalid = gInvalidPositionFunctions[i] )
  {
    invalidPosition |= invalid(proc, quadrant, invalidPosition);
  }

  if ( invalidPosition )
    return;

  proc->windowQuadrant = quadrant;

  proc->xCursor = gGameState.cursorMapPos.x;
  proc->yCursor = gGameState.cursorMapPos.y;

  udp = GetUnitDataProc(proc);

  GE_Static(proc, udp);
  GE_Dynamic(proc, udp);

  BreakProcLoop((Proc*)proc);

  return;
}
