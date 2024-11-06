
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"


void CritStandard_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws the unit's crit, right-aligned.
   */

  if ( proc->busyFlag )
    return;

  GenericNumberThreeDigits(udp->battleCritRate, CRT_X, CRT_Y, proc);

  return;
}
