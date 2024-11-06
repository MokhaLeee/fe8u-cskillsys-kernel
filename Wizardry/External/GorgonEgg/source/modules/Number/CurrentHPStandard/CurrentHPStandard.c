
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"


void CurrentHPStandard_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws the unit's current HP, right-aligned.
   */

  if ( proc->busyFlag )
    return;

  GenericNumberTwoDigits(GetUnitCurrentHp(udp->unit), CURRENT_HP_X, CURRENT_HP_Y, proc);

  return;
}
