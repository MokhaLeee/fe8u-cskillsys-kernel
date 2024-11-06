
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"


void MaxHPStandard_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws the unit's max HP, right-aligned.
   */

  if ( proc->busyFlag )
    return;

  GenericNumberTwoDigits(GetUnitMaxHp(udp->unit), MAX_HP_X, MAX_HP_Y, proc);

  return;
}
