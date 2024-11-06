
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

struct Vec2 GE_GetWindowPosition(struct PlayerInterfaceProc* proc);

#define INVENTORY_ICON_TILE (OAM2_CHR(INVENTORY_ICON_BASE_TILE) | OAM2_LAYER(0) | OAM2_PAL(INVENTORY_ICON_PALETTE))


void FullInventoryStandard_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Copies a unit's inventory icons to VRAM.
   *
   * Also copies the palette.
   */

  int i;
  int item;

  for ( i = 0; i < GetUnitItemCount(udp->unit); i++ )
  {
    item = udp->unit->items[i];

    LoadIconObjectGraphics(
        (ITEM_ICON_PAGE << 8) | GetItemIconId(item),
        INVENTORY_ICON_TILE + (2 * i)
      );
  }

  LoadIconPalette(0, INVENTORY_ICON_PALETTE);

  return;
}


void FullInventoryStandard_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Renders a unit's inventory icons to the window, as a single row.
   */

  int i, itemCount;
  struct Vec2 base_coords;

  itemCount = GetUnitItemCount(udp->unit);

  if ( proc->busyFlag || (itemCount == 0) )
    return;

  base_coords = GE_GetWindowPosition(proc);

  for ( i = 0; i < itemCount; i++ )
  {
    CallARM_PushToSecondaryOAM(
        ((base_coords.x + (i * 2)) * 8) + INVENTORY_ICON_X,
        (base_coords.y * 8) + INVENTORY_ICON_Y,
        &gObj_16x16,
        INVENTORY_ICON_TILE + (2 * i)
      );
  }

  return;
}
