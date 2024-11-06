
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"


void RatingStandard_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws the unit's rating, right-aligned.
   *
   * Rating is the sum of a unit's combat stats.
   */

  int rating;

  if ( proc->busyFlag )
    return;

  rating = (
      GetUnitMaxHp(udp->unit) +
      GetUnitPower(udp->unit) +
      GetUnitSkill(udp->unit) +
      GetUnitSpeed(udp->unit) +
      GetUnitLuck(udp->unit) +
      GetUnitDefense(udp->unit) +
      GetUnitResistance(udp->unit)
    );

  GenericNumberThreeDigits(rating, RTG_X, RTG_Y, proc);

  return;
}
