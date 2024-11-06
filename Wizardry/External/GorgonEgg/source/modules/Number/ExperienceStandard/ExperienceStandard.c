
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"


void ExperienceStandard_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws the unit's experience, right-aligned.
   */

  if ( proc->busyFlag )
    return;

  GenericNumberTwoDigits(udp->unit->exp, EXP_X, EXP_Y, proc);

  return;
}
