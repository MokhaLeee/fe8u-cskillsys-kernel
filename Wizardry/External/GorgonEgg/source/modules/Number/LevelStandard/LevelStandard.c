
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"


void LevelStandard_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws the unit's level, right-aligned.
   */

  if ( proc->busyFlag )
    return;

  GenericNumberTwoDigits(udp->unit->level, LEVEL_X, LEVEL_Y, proc);

  return;
}
