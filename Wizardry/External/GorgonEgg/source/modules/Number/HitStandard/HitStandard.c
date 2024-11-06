
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"


void HitStandard_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws the unit's hit, right-aligned.
   */

  if ( proc->busyFlag )
    return;

  GenericNumberThreeDigits(udp->battleHitRate, HIT_X, HIT_Y, proc);

  return;
}
