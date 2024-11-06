
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"


void DefenseStandard_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws the unit's defense, right-aligned.
   */

  if ( proc->busyFlag )
    return;

  GenericNumberTwoDigits(udp->battleDefense, DEF_X, DEF_Y, proc);

  return;
}
