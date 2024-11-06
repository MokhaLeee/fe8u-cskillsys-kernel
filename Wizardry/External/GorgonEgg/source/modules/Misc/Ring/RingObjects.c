
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#include "RingDefinitions.h"


// Sprite data

  static const struct ObjData RingFrame0 = {
    4,
    {
      RING_OBJ(-8, -8, 3, 14, 1, 8x8, TRUE,  FALSE),
      RING_OBJ( 0, -8, 3, 14, 1, 8x8, FALSE, FALSE),
      RING_OBJ(-8,  0, 3, 14, 1, 8x8, TRUE,  TRUE),
      RING_OBJ( 0,  0, 3, 14, 1, 8x8, FALSE, TRUE)
    }
  };


  static const struct ObjData RingFrame1 = {
    4,
    {
      RING_OBJ(-16, -16, 1, 14, 2, 16x16, TRUE,  FALSE),
      RING_OBJ(  0, -16, 1, 14, 2, 16x16, FALSE, FALSE),
      RING_OBJ(-16,   0, 1, 14, 2, 16x16, TRUE,  TRUE),
      RING_OBJ(  0,   0, 1, 14, 2, 16x16, FALSE, TRUE)
    }
  };


  static const struct ObjData RingFrame2 = {
    4,
    {
      RING_OBJ(-32, -32, 1, 10, 0, 32x32, TRUE,  FALSE),
      RING_OBJ(  0, -32, 1, 10, 0, 32x32, FALSE, FALSE),
      RING_OBJ(-32,   0, 1, 10, 0, 32x32, TRUE,  TRUE),
      RING_OBJ(  0,   0, 1, 10, 0, 32x32, FALSE, TRUE)
    }
  };


  static const struct ObjData RingFrame3 = {
    4,
    {
      RING_OBJ(-32, -32, 1, 6, 0, 32x32, TRUE,  FALSE),
      RING_OBJ(  0, -32, 1, 6, 0, 32x32, FALSE, FALSE),
      RING_OBJ(-32,   0, 1, 6, 0, 32x32, TRUE,  TRUE),
      RING_OBJ(  0,   0, 1, 6, 0, 32x32, FALSE, TRUE)
    }
  };


  static const struct ObjData RingFrame4 = {
    12,
    {
      RING_OBJ(-48, -48, 1, 2, 0, 32x32, TRUE,  FALSE),
      RING_OBJ(-16, -48, 1, 0, 0, 16x16, TRUE,  FALSE),
      RING_OBJ(-48, -16, 1, 0, 2, 16x16, TRUE,  FALSE),

      RING_OBJ( 16, -48, 1, 2, 0, 32x32, FALSE, FALSE),
      RING_OBJ(  0, -48, 1, 0, 0, 16x16, FALSE, FALSE),
      RING_OBJ( 32, -16, 1, 0, 2, 16x16, FALSE, FALSE),

      RING_OBJ(-48,  16, 1, 2, 0, 32x32, TRUE,  TRUE),
      RING_OBJ(-16,  32, 1, 0, 0, 16x16, TRUE,  TRUE),
      RING_OBJ(-48,   0, 1, 0, 2, 16x16, TRUE,  TRUE),

      RING_OBJ( 16,  16, 1, 2, 0, 32x32, FALSE, TRUE),
      RING_OBJ(  0,  32, 1, 0, 0, 16x16, FALSE, TRUE),
      RING_OBJ( 32,   0, 1, 0, 2, 16x16, FALSE, TRUE)
    }
  };


  static const struct ObjData* RingFrames[] = {
    &RingFrame0, &RingFrame0,
    &RingFrame1, &RingFrame1,
    &RingFrame2, &RingFrame2
  };


// Ring object functions

  void RenderRingFrame(int x, int y, int frame)
  {
    /* Renders a ring around a point.
     */

    CallARM_PushToSecondaryOAM(OAM1_X(x), OAM0_Y(y), RingFrames[frame], 0);

    return;
  }


  void RenderFullRing(int x, int y)
  {
    /* Renders the fully-extended ring.
     */

    CallARM_PushToSecondaryOAM(OAM1_X(x), OAM0_Y(y), &RingFrame3, 0);

    return;
  }


  void RenderExtendedRing(int x, int y)
  {
    /* Renders a large ring for when the unit is close to a map edge.
     */

    CallARM_PushToSecondaryOAM(OAM1_X(x), OAM0_Y(y), &RingFrame4, 0);

    return;
  }
