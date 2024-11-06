
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"


void DodgeStandard_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws the unit's dodge, right-aligned.
   */

  if ( proc->busyFlag )
    return;

  GenericNumberThreeDigits(udp->battleDodgeRate, DOD_X, DOD_Y, proc);

  return;
}
