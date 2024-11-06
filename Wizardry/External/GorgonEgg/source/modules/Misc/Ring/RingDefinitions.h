
/* Ring definitions
 *
 * This file is to avoid cluttering the actual
 * Ring sources.
 */

#include "gbafe.h"

// Object helpers

  #define RING_TILE_INDEX(tileX, tileY) (((32 * tileY) + tileX) + RING_BASE_TILE)

  #define RING_OAM0(yCoord, style) (OAM0_Y(yCoord) | (OAM0_SHAPE_ ## style))

  #define RING_OAM1(xCoord, style, xFlip, yFlip) ( \
      OAM1_X(xCoord)                               \
    | (OAM1_SIZE_ ## style)                        \
    | (xFlip ? OAM1_HFLIP : 0)                     \
    | (yFlip ? OAM1_VFLIP : 0)                     \
    )

  #define RING_OAM2(tileX, tileY, layer) (    \
      OAM2_CHR(RING_TILE_INDEX(tileX, tileY)) \
    | OAM2_LAYER(layer)                       \
    | OAM2_PAL(RING_PALETTE)                  \
    )

  #define RING_OBJ(xCoord, yCoord, layer, tileX, tileY, style, xFlip, yFlip) \
    RING_OAM0(yCoord, style), RING_OAM1(xCoord, style, xFlip, yFlip), RING_OAM2(tileX, tileY, layer)

  #define RING_HP_ORB_ROW_SIZE 7
  #define RING_HP_ORB_ROW0 RING_OAM2(18, 0, 0)
  #define RING_HP_ORB_ROW1 RING_OAM2(18, 2, 0)

  #define ITEM_BACKGROUND RING_OAM2(16, 2, 1)

  // This is the start of where item icons are drawn.
  #define INVENTORY_ICON_TILE (OAM2_CHR(INVENTORY_ICON_BASE_TILE) | OAM2_LAYER(0) | OAM2_PAL(INVENTORY_ICON_PALETTE))


// Misc. definitions

  // This is the number of partially-filled levels
  // for the HP orb graphics.
  #define HP_ORB_LEVELS 13


// Ring macros

  struct RingItemCoordinates {
    s8 x;
    s8 y;
  };

  struct RingItemCoordinateList {
    const struct RingItemCoordinates healthPosition;
    const struct RingItemCoordinates data[];
  };

  struct RingTile {
    const struct RingItemCoordinateList* itemVariants[6];
  };

  #define RICL static const struct RingItemCoordinateList

  #define RT(name)                                     \
    static const struct RingTile RingTile_ ## name = { \
      {                                                \
        &RingTile_ ## name ## _0,                      \
        &RingTile_ ## name ## _1,                      \
        &RingTile_ ## name ## _2,                      \
        &RingTile_ ## name ## _3,                      \
        &RingTile_ ## name ## _4,                      \
        &RingTile_ ## name ## _5,                      \
      }                                                \
    };


// Forward declarations

  // Ring objects

    void RenderRingFrame(int x, int y, int frame);
    void RenderFullRing(int x, int y);
    void RenderExtendedRing(int x, int y);


  // Ring tiles

    const struct RingItemCoordinateList* GetRingCoordList(struct PlayerInterfaceProc* proc, int count);
    void RenderRingHPOrb(int x, int y, struct Unit* unit);
    void RenderRingItems(int baseX, int baseY, int itemCount, const struct RingItemCoordinateList* coordList);

  // Graphics

    extern const u8 gRingGraphics[(32 * 4 * CHR_SIZE)];
    extern const u16 aRingPalette[];


