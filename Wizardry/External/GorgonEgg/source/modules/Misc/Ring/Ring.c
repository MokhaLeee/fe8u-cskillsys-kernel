
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#include "RingDefinitions.h"

void GE_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp);

struct UnitDataProc* GetUnitDataProc(struct PlayerInterfaceProc* proc);

// These helpers get the coordinates of the center of the ring.

#define RING_X ((proc->xCursor * 16) + 8)
#define RING_Y ((proc->yCursor * 16) + 8)
#define RING_PREVIOUS_X ((proc->xCursorPrevious * 16) + 8)
#define RING_PREVIOUS_Y ((proc->yCursorPrevious * 16) + 8)


static const struct ObjData* RingFrames[6];

#define MAX_RING_FRAMES (sizeof(RingFrames) / sizeof(RingFrames[0]))


void Ring_Init(struct PlayerInterfaceProc* proc)
{
  /* Copies ring graphics and palette.
   */

  void* graphicsDest = (void*)((RING_BASE_TILE * CHR_SIZE) + VRAM_OBJ);

  RegisterTileGraphics(gRingGraphics, graphicsDest, sizeof(gRingGraphics));

  ApplyPalette(aRingPalette, RING_PALETTE);

  LoadIconPalette(0, INVENTORY_ICON_PALETTE);

  return;
}


void Ring_Extend(struct PlayerInterfaceProc* proc)
{
  /* Draws an expanding ring animation.
   */

  RenderRingFrame(RING_X, RING_Y, proc->movementFrame);

  proc->movementFrame++;

  if (proc->movementFrame == (sizeof(RingFrames) / sizeof(RingFrames[0])))
  {
    proc->busyFlag = FALSE;
    proc->movementFrame = 0;

    BreakProcLoop((void*)proc);

  }

  return;
}


void Ring_Static(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Copies graphics for the unit's inventory.
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

  return;
}


void Ring_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Renders the fully-extended rings, HP, and items every frame.
   */

  int itemCount;
  const struct RingItemCoordinateList* coordList;

  struct Vec2 baseCoords = {RING_X, RING_Y};

  if ( proc->busyFlag )
    return;

  itemCount = GetUnitItemCount(udp->unit);

  coordList = GetRingCoordList(proc, itemCount);

  RenderRingHPOrb(
      baseCoords.x + coordList->healthPosition.x,
      baseCoords.y + coordList->healthPosition.y,
      udp->unit
    );

  RenderRingItems(baseCoords.x, baseCoords.y, itemCount, coordList);

  RenderFullRing(baseCoords.x, baseCoords.y);

  if ( ((proc->xCursor <  2) || (proc->xCursor > 12)) && ((proc->yCursor <  2) || (proc->yCursor >  7)) )
    RenderExtendedRing(baseCoords.x, baseCoords.y);

  return;
}


void Ring_Retract(struct PlayerInterfaceProc* proc)
{
  /* Draws a shrinking ring animation.
   */

  RenderRingFrame(
      RING_PREVIOUS_X,
      RING_PREVIOUS_Y,
      MAX_RING_FRAMES - 1 - proc->movementFrame
    );

  proc->movementFrame++;

  if ( proc->movementFrame == MAX_RING_FRAMES )
  {

    proc->isRetracting = FALSE;
    proc->movementFrame = 0;
    proc->cursorQuadrant = -1;

    BreakProcLoop((void*)proc);
  }

  return;
}
