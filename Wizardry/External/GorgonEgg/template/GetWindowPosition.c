
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#ifndef TILEMAP_DESTINATION
  /* This dummy macro will have its definition replaced
   * with a series of function pointers, terminated with NULL.
   * These functions are then called like
   * coords = SomeFunction(proc, coords);
   * for each <TilemapDestination> tag in a module.
   * Generally, there should be exactly one (from
   * whatever handles the window tilemap).
   */
  #define TILEMAP_DESTINATION NULL
  #endif // TILEMAP_DESTINATION

// These definitions should be overridden by a module or
// the config, but are defined here for safety.

#ifndef GE_WIDTH
  #define GE_WIDTH 13
  #endif // GE_WIDTH

#ifndef GE_HEIGHT
  #define GE_HEIGHT 6
  #endif // GE_HEIGHT

typedef struct Vec2 (*getwindowposition_func) (struct PlayerInterfaceProc* proc, struct Vec2 coords);


const getwindowposition_func gGetWindowPosition[] = {
  TILEMAP_DESTINATION
};



struct Vec2 GE_GetWindowPosition(struct PlayerInterfaceProc* proc)
{
  /* Get the position of the (fully extended) window.
   */

  int i;
  getwindowposition_func getwindowposition;

  struct Vec2 coords = {0, 0};

  for ( i = 0, getwindowposition = gGetWindowPosition[i]; getwindowposition != NULL; i++, getwindowposition = gGetWindowPosition[i] )
  {
    coords = getwindowposition(proc, coords);
  }

  return coords;
}
