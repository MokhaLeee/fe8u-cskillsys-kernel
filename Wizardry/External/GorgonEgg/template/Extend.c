
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#ifndef EXTEND_CALLS
  /* This dummy macro will have its definition replaced
   * with a series of function pointers, terminated with NULL.
   * These functions are then called like
   * SomeFunction(proc);
   * for each <Extend> tag in a module.
   * Generally, there should be exactly one
   * (from whatever handles extending/retracting).
   */
  #define EXTEND_CALLS NULL
  #endif // EXTEND_CALLS

typedef void (*extend_func) (struct PlayerInterfaceProc* proc);


const extend_func gExtendFunctions[] = {
  EXTEND_CALLS
};


void GE_Extend(struct PlayerInterfaceProc* proc)
{
  /* Extend the window every cycle.
   *
   * This handles extending the window every cycle. It
   * is up to the `EXTEND_CALLS` function(s) to increment
   * the proc's `movementFrame` and break the current
   * proc loop upon finishing the extension.
   */

  int i;
  extend_func extend;

  for ( i = 0, extend = gExtendFunctions[i]; extend != NULL; i++, extend = gExtendFunctions[i] )
  {
    extend(proc);
  }

  return;
}
