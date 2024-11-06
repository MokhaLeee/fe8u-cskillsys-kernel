
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#ifndef INIT_CALLS
  /* This dummy macro will have its definition replaced
   * with a series of function pointers, terminated with NULL.
   * These functions are then called like
   * SomeFunction(proc);
   * for each <Init> tag in a module.
   */
  #define INIT_CALLS NULL
  #endif // INIT_CALLS

extern const struct ProcInstruction ProcUnitData[];

typedef void (*init_func) (struct PlayerInterfaceProc* proc);


const init_func gInitFunctions[] = {
  INIT_CALLS
};


void GE_Init(struct PlayerInterfaceProc* proc)
{
  /* Handle first-time setup.
   */

  int i;
  init_func init;

  proc->windowQuadrant = -1;

  START_PROC(ProcUnitData, proc);

  for ( i = 0, init = gInitFunctions[i]; init != NULL; i++, init = gInitFunctions[i] )
  {
    init(proc);
  }

  proc->movementFrame = 0;
  proc->isRetracting = FALSE;

  return;
}
