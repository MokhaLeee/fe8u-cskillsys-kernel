
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"


void AttackStandard_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws the unit's attack, right-aligned.
   */

  if ( proc->busyFlag )
    return;

  GenericNumberTwoDigits(udp->battleAttack, ATK_X, ATK_Y, proc);

  return;
}
