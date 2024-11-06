
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"


void ResistanceStandard_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Draws the unit's resistance, right-aligned.
   *
   * As a note, this is resistance without bonuses, because it's
   * difficult to generate.
   */

  if ( proc->busyFlag )
    return;

  GenericNumberTwoDigits(GetUnitResistance(udp->unit), RES_X, RES_Y, proc);

  return;
}
