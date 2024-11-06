
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

struct Vec2 GE_GetWindowPosition(struct PlayerInterfaceProc* proc);

#define EQUIP_ICON_TILE (OAM2_CHR(EQUIP_ICON_BASE_TILE) | OAM2_LAYER(0) | OAM2_PAL(EQUIP_ICON_PALETTE))


void EquipIconStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Copies a unit's equipped weapon icon to VRAM, if they have one.
   *
   * Also copies the palette.
   */

  if ( udp->weapon != 0 )
  {
    LoadIconObjectGraphics(
        (EQUIP_ICON_PAGE << 8) | GetItemIconId(udp->weapon),
        EQUIP_ICON_BASE_TILE
      );

    LoadIconPalette(0, EQUIP_ICON_PALETTE);
  }

  return;
}


void EquipIconStandard_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Renders a unit's equipped weapon icon to the window.
   */

  struct Vec2 coords;

  if ( proc->busyFlag || (udp->weapon == 0) )
    return;

  coords = GE_GetWindowPosition(proc);

  CallARM_PushToSecondaryOAM(
      (coords.x * 8) + EQUIP_ICON_X,
      (coords.y * 8) + EQUIP_ICON_Y,
      &gObj_16x16,
      EQUIP_ICON_TILE
    );

  return;
}
