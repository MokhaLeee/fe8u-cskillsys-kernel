
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

void GE_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp);
void GE_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp);
void GE_CopyTilemap(struct PlayerInterfaceProc* proc);

struct UnitDataProc* GetUnitDataProc(struct PlayerInterfaceProc* proc);


void GE_CheckForUnit(struct PlayerInterfaceProc* proc)
{
  /* Retract or redraw after moving the cursor.
   */
  struct UnitDataProc* udp;

  struct Unit* unit = GetUnitAtCursor();

  // If we didn't move onto a unit, retract.
  // Otherwise, redraw the window.

  if ( !unit )
  {
    ProcGoto((Proc*)proc, GE_PROC_RETRACT);
  }
  else
  {
    udp = GetUnitDataProc(proc);
    GE_Static(proc, udp);
    GE_Dynamic(proc, udp);
    GE_CopyTilemap(proc);
  }

  return;
}
