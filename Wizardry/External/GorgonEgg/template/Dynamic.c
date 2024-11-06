
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#ifndef DYNAMIC_CALLS
  /* This dummy macro will have its definition replaced
   * with a series of function pointers, terminated with NULL.
   * These functions are called like
   * SomeFunction(proc, udp);
   * for each <Dynamic> tag in a module.
   */
  #define DYNAMIC_CALLS NULL
  #endif // DYNAMIC_CALLS

typedef void (*dynamic_func) (struct PlayerInterfaceProc* proc, struct UnitDataProc* udp);


const dynamic_func gDynamicFunctions[] = {
  DYNAMIC_CALLS
};


void GE_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draw dynamic elements.
   *
   * Handles all window elements that must be
   * updated or redrawn every frame.
   */

  int i;
  dynamic_func dynamic;

  for ( i = 0, dynamic = gDynamicFunctions[i]; dynamic != NULL; i++, dynamic = gDynamicFunctions[i] )
  {
    dynamic(proc, udp);
  }

  return;
}
