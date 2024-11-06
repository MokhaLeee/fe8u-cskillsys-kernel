
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

struct Vec2 GE_GetWindowPosition(struct PlayerInterfaceProc* proc);

#define AFFINITY_TILE (OAM2_CHR(AFFINITY_BASE_TILE) | OAM2_LAYER(0) | OAM2_PAL(AFFINITY_PALETTE))


void AffinityStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Copies a unit's affinity to VRAM, if they have one.
   *
   * Also copies the palette.
   */

  int affinityIcon = GetUnitAffinityIcon(udp->unit);

  if ( affinityIcon < 0 )
    return;

  // If using the FE8 skill system icon rework,
  // AFFINITY_PAGE should be the icon page that
  // affinity icons are on.

  LoadIconObjectGraphics(
      (AFFINITY_PAGE << 8) | affinityIcon,
      AFFINITY_BASE_TILE
    );

  ApplyPalette(gAffinityWeaponRankPalette, AFFINITY_PALETTE);

  return;
}


void AffinityStandard_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Renders a unit's affinity to the window.
   */

  struct Vec2 coords;
  int affinityIcon = GetUnitAffinityIcon(udp->unit);

  if ( proc->busyFlag || (affinityIcon < 0) )
    return;

  coords = GE_GetWindowPosition(proc);

  CallARM_PushToSecondaryOAM(
      (coords.x * 8) + AFFINITY_X,
      (coords.y * 8) + AFFINITY_Y,
      &gObj_16x16,
      AFFINITY_TILE
    );

  return;
}
