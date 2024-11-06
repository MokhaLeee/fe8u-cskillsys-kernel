
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"


void AttackSpeedStandard_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws the unit's attack speed, right-aligned.
   */

  if ( proc->busyFlag )
    return;

  GenericNumberTwoDigits(udp->battleSpeed, AS_X, AS_Y, proc);

  return;
}
