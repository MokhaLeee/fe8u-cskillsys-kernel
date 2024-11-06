
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"


void AvoidStandard_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws the unit's avoid, right-aligned.
   */

  if ( proc->busyFlag )
    return;

  GenericNumberThreeDigits(udp->battleAvoidRate, AVO_X, AVO_Y, proc);

  return;
}
