
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

struct Vec2 GE_GetWindowPosition(struct PlayerInterfaceProc* proc);

#define INVENTORY_ICON_TILE (OAM2_CHR(INVENTORY_ICON_BASE_TILE) | OAM2_LAYER(0) | OAM2_PAL(INVENTORY_ICON_PALETTE))
#define DROPPABLE_ICON_TILE (OAM2_CHR(DROPPABLE_ICON_BASE_TILE) | OAM2_LAYER(0) | OAM2_PAL(DROPPABLE_ICON_PALETTE))

extern const u8 gDroppableItemIcon[(1 * CHR_SIZE)];


void FullInventoryDroppable_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Copies a unit's inventory icons to VRAM.
   *
   * Also copies the palettes and droppables icon.
   */

  int i;
  int item;
  void* graphicsDest;

  for ( i = 0; i < GetUnitItemCount(udp->unit); i++ )
  {
    item = udp->unit->items[i];

    LoadIconObjectGraphics(
        (ITEM_ICON_PAGE << 8) | GetItemIconId(item),
        INVENTORY_ICON_TILE + (2 * i)
      );
  }

  LoadIconPalette(0, INVENTORY_ICON_PALETTE);

  // Copy the droppable item palette and graphics.

  graphicsDest = (void*)((DROPPABLE_ICON_BASE_TILE * CHR_SIZE) + VRAM_OBJ);

  RegisterTileGraphics(gDroppableItemIcon, graphicsDest, sizeof(gDroppableItemIcon));
  ApplyPalette(gPal_MiscUIGraphics, DROPPABLE_ICON_PALETTE);

  return;
}


void FullInventoryDroppable_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Renders a unit's inventory icons to the window, as a single row, with an icon for droppables.
   */

  int i, itemCount;
  struct Vec2 base_coords;

  itemCount = GetUnitItemCount(udp->unit);

  if ( proc->busyFlag || (itemCount == 0) )
    return;

  base_coords = GE_GetWindowPosition(proc);

  if ( (udp->unit->state & US_DROP_ITEM) )
  {
    CallARM_PushToSecondaryOAM(
        ((base_coords.x + ((itemCount - 1) * 2)) * 8) + INVENTORY_ICON_X,
        (base_coords.y * 8) + INVENTORY_ICON_Y + 8,
        &gObj_8x8,
        DROPPABLE_ICON_TILE
      );
  }

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
