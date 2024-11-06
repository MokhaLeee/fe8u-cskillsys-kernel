
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

#include "RingDefinitions.h"


// Ring tile data

  // These structs define the positions of the HP orb and
  // items on the ring. Each tile gets a struct for all of the
  // combinations of item counts a unit can have.

  RICL RingTile_Center_0 = { { -8, -36}, {                                                            } };
  RICL RingTile_Center_1 = { { -8, -36}, { { -8,  20}                                                 } };
  RICL RingTile_Center_2 = { { -8, -36}, { { 15,   8}, {-31,   8}                                     } };
  RICL RingTile_Center_3 = { { -8, -36}, { { 20,  -8}, { -8,  20}, {-36,  -8}                         } };
  RICL RingTile_Center_4 = { { -8, -36}, { { 18, -17}, {  7,  15}, {-23,  15}, {-34, -17}             } };
  RICL RingTile_Center_5 = { { -8, -36}, { { 16, -22}, { 16,   6}, { -8,  20}, {-32,   6}, {-32, -22} } };
  RT(Center)

  RICL RingTile_FarLeft_0 = { { 20,  -8}, {                                                            } };
  RICL RingTile_FarLeft_1 = { { 12, -28}, { { 12,  12}                                                 } };
  RICL RingTile_FarLeft_2 = { {  3, -33}, { { 20,  -8}, {  3,  17}                                     } };
  RICL RingTile_FarLeft_3 = { {  1, -35}, { { 17, -19}, { 17,   3}, {  1,  19}                         } };
  RICL RingTile_FarLeft_4 = { { -7, -36}, { { 12, -27}, { 20,  -8}, { 13,  11}, { -7,  20}             } };
  RICL RingTile_FarLeft_5 = { { -7, -36}, { {  9, -31}, { 18, -16}, { 18,   0}, {  9,  15}, { -7,  20} } };
  RT(FarLeft)

  RICL RingTile_Left_0 = { { 20,  -8}, {                                                            } };
  RICL RingTile_Left_1 = { {  0, -34}, { {  0,  18}                                                 } };
  RICL RingTile_Left_2 = { {-16, -35}, { { 20,  -8}, {-16,  19}                                     } };
  RICL RingTile_Left_3 = { {-16, -35}, { { 14, -24}, { 14,   8}, {-16,  19}                         } };
  RICL RingTile_Left_4 = { {-20, -34}, { {  8, -30}, { 20,  -8}, {  8,  14}, { 20, -18}             } };
  RICL RingTile_Left_5 = { {-21, -33}, { {  1, -35}, { 18, -20}, { 18,   4}, {  1,  19}, {-21,  17} } };
  RT(Left)

  RICL RingTile_FarTop_0 = { { -8,  20}, {                                                            } };
  RICL RingTile_FarTop_1 = { { 12,  12}, { {-28,  12}                                                 } };
  RICL RingTile_FarTop_2 = { { 17,   3}, { { -8,  20}, {-33,   3}                                     } };
  RICL RingTile_FarTop_3 = { { 19,   1}, { {  3,  17}, {-19,  17}, {-35,   1}                         } };
  RICL RingTile_FarTop_4 = { { 20,  -7}, { { 11,  12}, { -8,  20}, {-27,  13}, {-36,  -7}             } };
  RICL RingTile_FarTop_5 = { { 20,  -7}, { { 15,   9}, {  0,  18}, {-16,  18}, {-31,   9}, {-36,  -7} } };
  RT(FarTop)

  RICL RingTile_Top_0 = { { -8,  20}, {                                                            } };
  RICL RingTile_Top_1 = { { 18,   0}, { {-34,   0}                                                 } };
  RICL RingTile_Top_2 = { { 19, -16}, { { -8,  20}, {-35, -16}                                     } };
  RICL RingTile_Top_3 = { { 19, -16}, { {  8,  14}, {-24,  14}, {-35, -16}                         } };
  RICL RingTile_Top_4 = { { 18, -20}, { { 14,   8}, { -8,  20}, {-30,   8}, {-34, -20}             } };
  RICL RingTile_Top_5 = { { 17, -21}, { { 19,   1}, {  4,  18}, {-20,  18}, {-35,   1}, {-33, -31} } };
  RT(Top)

  RICL RingTile_FarRight_0 = { {-36,  -8}, {                                                            } };
  RICL RingTile_FarRight_1 = { {-28, -28}, { {-28,  12}                                                 } };
  RICL RingTile_FarRight_2 = { {-19, -33}, { {-36,  -8}, {-19,  17}                                     } };
  RICL RingTile_FarRight_3 = { {-17, -35}, { {-33, -19}, {-33,   3}, {-19,  19}                         } };
  RICL RingTile_FarRight_4 = { { -9, -36}, { {-28, -27}, {-36,  -8}, {-29,  11}, { -9,  20}             } };
  RICL RingTile_FarRight_5 = { { -9, -36}, { {-25, -31}, {-34, -16}, {-34,   0}, {-25,  15}, { -9,  20} } };
  RT(FarRight)

  RICL RingTile_Right_0 = { {-36,  -8}, {                                                            } };
  RICL RingTile_Right_1 = { {-16, -34}, { {-16,  18}                                                 } };
  RICL RingTile_Right_2 = { {  0, -35}, { {-36,  -8}, {  0,  19}                                     } };
  RICL RingTile_Right_3 = { {  0, -35}, { {-30, -24}, {-30,   8}, {  0,  19}                         } };
  RICL RingTile_Right_4 = { {  4, -34}, { {-24, -30}, {-36,  -8}, {-24,  14}, {  4,  18}             } };
  RICL RingTile_Right_5 = { {  5, -33}, { {-17, -35}, {-34, -20}, {-34,   4}, {-17,  19}, {  5,  17} } };
  RT(Right)

  RICL RingTile_FarBottom_0 = { { -8, -36}, {                                                            } };
  RICL RingTile_FarBottom_1 = { {-28, -28}, { { 12, -28}                                                 } };
  RICL RingTile_FarBottom_2 = { {-33, -19}, { { -8, -36}, { 17, -19}                                     } };
  RICL RingTile_FarBottom_3 = { {-35, -17}, { {-19, -33}, {  3, -33}, { 19, -17}                         } };
  RICL RingTile_FarBottom_4 = { {-36,  -9}, { {-27, -28}, { -8, -36}, { 11, -29}, { 20,  -9}             } };
  RICL RingTile_FarBottom_5 = { {-36,  -9}, { {-31, -25}, {-16, -34}, {  0, -34}, { 15, -25}, { 20,  -9} } };
  RT(FarBottom)

  RICL RingTile_Bottom_0 = { { -8, -36}, {                                                            } };
  RICL RingTile_Bottom_1 = { {-34, -16}, { { 18, -16}                                                 } };
  RICL RingTile_Bottom_2 = { {-35,   0}, { { -8, -36}, { 19,   0}                                     } };
  RICL RingTile_Bottom_3 = { {-35,   0}, { {-24, -30}, {  8, -30}, { 19,   0}                         } };
  RICL RingTile_Bottom_4 = { {-34,   4}, { {-30, -24}, { -8, -36}, { 14, -24}, { 18,   4}             } };
  RICL RingTile_Bottom_5 = { {-33,   5}, { {-35, -17}, {-20, -34}, {  4, -34}, { 19, -17}, { 17,   5} } };
  RT(Bottom)

  RICL RingTile_0x0_0 = { { 12,  12}, {                                                            } };
  RICL RingTile_0x0_1 = { { 12,  12}, { { 25,  25}                                                 } };
  RICL RingTile_0x0_2 = { { 12,  12}, { { 35,   2}, {  2,  35}                                     } };
  RICL RingTile_0x0_3 = { { 12,  12}, { { 35,   2}, { 25,  25}, {  2,  35}                         } };
  RICL RingTile_0x0_4 = { { 12,  12}, { { 35,  -4}, { 30,  16}, { 16,  30}, { -4,  35}             } };
  RICL RingTile_0x0_5 = { { 12,  12}, { { 36,  -8}, { 33,   9}, { 25,  25}, {  9,  33}, { -8,  36} } };
  RT(0x0)

  RICL RingTile_1x0_0 = { {  6,  16}, {                                                            } };
  RICL RingTile_1x0_1 = { {  6,  16}, { { 22,  24}                                                 } };
  RICL RingTile_1x0_2 = { {  6,  16}, { { 30,  14}, { -3,  36}                                     } };
  RICL RingTile_1x0_3 = { {  6,  16}, { { 35,  -1}, { 22,  24}, { -8,  36}                         } };
  RICL RingTile_1x0_4 = { {  6,  16}, { { 36,  -4}, { 29,  16}, { 10,  33}, {-15,  36}             } };
  RICL RingTile_1x0_5 = { {  6,  16}, { { 36,  -6}, { 30,  13}, { 17,  28}, { -1,  35}, {-22,  34} } };
  RT(1x0)

  RICL RingTile_0x1_0 = { { 16,   6}, {                                                            } };
  RICL RingTile_0x1_1 = { { 16,   6}, { { 24,  22}                                                 } };
  RICL RingTile_0x1_2 = { { 16,   6}, { { 36,  -3}, { 14,  30}                                     } };
  RICL RingTile_0x1_3 = { { 16,   6}, { { 36,  -8}, { 24,  22}, { -1,  35}                         } };
  RICL RingTile_0x1_4 = { { 16,   6}, { { 36, -15}, { 33,  10}, { 16,  29}, { -4,  36}             } };
  RICL RingTile_0x1_5 = { { 16,   6}, { { 34, -22}, { 35,  -1}, { 28,  17}, { 13,  30}, { -6,  36} } };
  RT(0x1)

  RICL RingTile_1x1_0 = { { 12,  12}, {                                                            } };
  RICL RingTile_1x1_1 = { { 12,  12}, { { 25,  25}                                                 } };
  RICL RingTile_1x1_2 = { { 12,  12}, { { 35,  -3}, { -3,  35}                                     } };
  RICL RingTile_1x1_3 = { { 12,  12}, { { 35, -14}, { 25,  25}, {-14,  35}                         } };
  RICL RingTile_1x1_4 = { { 12,  12}, { { 33, -23}, { 33,   8}, {  8,  33}, {-23,  33}             } };
  RICL RingTile_1x1_5 = { { 12,  12}, { { 33, -23}, { 35,   2}, { 25,  25}, {  2,  35}, {-23,  33} } };
  RT(1x1)

  RICL RingTile_14x0_0 = { {-28,  12}, {                                                            } };
  RICL RingTile_14x0_1 = { {-28,  12}, { {-41,  25}                                                 } };
  RICL RingTile_14x0_2 = { {-28,  12}, { {-51,   2}, {-18,  35}                                     } };
  RICL RingTile_14x0_3 = { {-28,  12}, { {-51,   2}, {-41,  25}, {-18,  35}                         } };
  RICL RingTile_14x0_4 = { {-28,  12}, { {-51,  -4}, {-46,  16}, {-32,  30}, { 12,  35}             } };
  RICL RingTile_14x0_5 = { {-28,  12}, { {-52,  -8}, {-49,   9}, {-41,  25}, {-25,  33}, {  8,  36} } };
  RT(14x0)

  RICL RingTile_13x0_0 = { {-22,  16}, {                                                            } };
  RICL RingTile_13x0_1 = { {-22,  16}, { {-38,  24}                                                 } };
  RICL RingTile_13x0_2 = { {-22,  16}, { {-46,  14}, { 13,  36}                                     } };
  RICL RingTile_13x0_3 = { {-22,  16}, { {-51,  -1}, {-38,  24}, { -8,  36}                         } };
  RICL RingTile_13x0_4 = { {-22,  16}, { {-51,  -4}, {-45,  16}, {-26,  33}, {  1,  36}             } };
  RICL RingTile_13x0_5 = { {-22,  16}, { {-51,  -6}, {-46,  13}, {-33,  28}, { 15,  35}, {  6,  34} } };
  RT(13x0)

  RICL RingTile_14x1_0 = { {-32,   6}, {                                                            } };
  RICL RingTile_14x1_1 = { {-32,   6}, { {-40,  22}                                                 } };
  RICL RingTile_14x1_2 = { {-32,   6}, { {-52,  -3}, {-30,  30}                                     } };
  RICL RingTile_14x1_3 = { {-32,   6}, { {-52,  -8}, {-40,  22}, { 15,  35}                         } };
  RICL RingTile_14x1_4 = { {-32,   6}, { {-52, -15}, {-49,  10}, {-32,  29}, { 12,  36}             } };
  RICL RingTile_14x1_5 = { {-32,   6}, { {-50, -22}, {-51,  -1}, {-44,  17}, {-29,  30}, {-10,  36} } };
  RT(14x1)

  RICL RingTile_13x1_0 = { {-38,  12}, {                                                            } };
  RICL RingTile_13x1_1 = { {-38,  12}, { {-41,  25}                                                 } };
  RICL RingTile_13x1_2 = { {-38,  12}, { {-51,  -3}, { 13,  35}                                     } };
  RICL RingTile_13x1_3 = { {-38,  12}, { {-51, -14}, {-41,  25}, { -2,  35}                         } };
  RICL RingTile_13x1_4 = { {-38,  12}, { {-49, -23}, {-49,   8}, {-24,  33}, {  7,  33}             } };
  RICL RingTile_13x1_5 = { {-38,  12}, { {-49, -23}, {-51,   2}, {-41,  25}, {-18,  35}, {  7,  33} } };
  RT(13x1)

  RICL RingTile_0x9_0 = { { 12, -28}, {                                                            } };
  RICL RingTile_0x9_1 = { { 12, -28}, { { 25, -41}                                                 } };
  RICL RingTile_0x9_2 = { { 12, -28}, { {  2, -51}, { 35, -18}                                     } };
  RICL RingTile_0x9_3 = { { 12, -28}, { {  2, -51}, { 25, -41}, { 35, -18}                         } };
  RICL RingTile_0x9_4 = { { 12, -28}, { { -4, -51}, { 16, -46}, { 30, -32}, { 35, -12}             } };
  RICL RingTile_0x9_5 = { { 12, -28}, { { -8, -52}, {  9, -49}, { 25, -41}, { 33, -25}, { 36,  -8} } };
  RT(0x9)

  RICL RingTile_1x9_0 = { {  6, -32}, {                                                            } };
  RICL RingTile_1x9_1 = { {  6, -32}, { { 22, -40}                                                 } };
  RICL RingTile_1x9_2 = { {  6, -32}, { { -3, -52}, { 30, -30}                                     } };
  RICL RingTile_1x9_3 = { {  6, -32}, { { -8, -52}, { 22, -40}, { 35, -15}                         } };
  RICL RingTile_1x9_4 = { {  6, -32}, { {-15, -52}, { 10, -49}, { 29, -32}, { 36, -12},            } };
  RICL RingTile_1x9_5 = { {  6, -32}, { {-22, -50}, { -1, -51}, { 17, -44}, { 30, -29}, { 36, -10} } };
  RT(1x9)

  RICL RingTile_0x8_0 = { { 16,  -22}, {                                                            } };
  RICL RingTile_0x8_1 = { { 16,  -22}, { { 24, -38}                                                 } };
  RICL RingTile_0x8_2 = { { 16,  -22}, { { 14, -46}, { 36, -13}                                     } };
  RICL RingTile_0x8_3 = { { 16,  -22}, { { -1, -51}, { 24, -38}, { 36,  -8}                         } };
  RICL RingTile_0x8_4 = { { 16,  -22}, { { -4, -52}, { 16, -45}, { 33, -26}, { 36,  -1}             } };
  RICL RingTile_0x8_5 = { { 16,  -22}, { { -6, -52}, { 13, -46}, { 28, -33}, { 35, -15}, { 34,   6} } };
  RT(0x8)

  RICL RingTile_1x8_0 = { { 12, -28}, {                                                            } };
  RICL RingTile_1x8_1 = { { 12, -28}, { { 25, -41}                                                 } };
  RICL RingTile_1x8_2 = { { 12, -28}, { { -3, -51}, { 35, -13}                                     } };
  RICL RingTile_1x8_3 = { { 12, -28}, { {-14, -51}, { 25, -41}, { 35,  -2}                         } };
  RICL RingTile_1x8_4 = { { 12, -28}, { {-23, -49}, {  8, -49}, { 33, -24}, { 33,   7}             } };
  RICL RingTile_1x8_5 = { { 12, -28}, { {-23, -49}, {  2, -51}, { 25, -41}, { 35, -18}, { 33,   7} } };
  RT(1x8)

  RICL RingTile_13x8_0 = { {-28, -28}, {                                                            } };
  RICL RingTile_13x8_1 = { {-28, -28}, { {-41, -41}                                                 } };
  RICL RingTile_13x8_2 = { {-28, -28}, { {-13, -51}, {-51, -13}                                     } };
  RICL RingTile_13x8_3 = { {-28, -28}, { { -2, -51}, {-41, -41}, {-51,  -2}                         } };
  RICL RingTile_13x8_4 = { {-28, -28}, { {  7, -49}, {-24, -49}, {-49, -24}, {-49,   7}             } };
  RICL RingTile_13x8_5 = { {-28, -28}, { {  7, -49}, {-18, -51}, {-41, -41}, {-51, -18}, {-49,   7} } };
  RT(13x8)

  RICL RingTile_14x9_0 = { {-28, -28}, {                                                            } };
  RICL RingTile_14x9_1 = { {-28, -28}, { {-41, -41}                                                 } };
  RICL RingTile_14x9_2 = { {-28, -28}, { {-18, -51}, {-51, -18}                                     } };
  RICL RingTile_14x9_3 = { {-28, -28}, { {-18, -51}, {-41, -41}, {-51, -18}                         } };
  RICL RingTile_14x9_4 = { {-28, -28}, { {-12, -51}, {-31, -46}, {-46, -32}, {-51, -12}             } };
  RICL RingTile_14x9_5 = { {-28, -28}, { { -8, -52}, {-25, -49}, {-41, -41}, {-49, -25}, {-52,  -8} } };
  RT(14x9)

  RICL RingTile_14x8_0 = { {-32,  -22}, {                                                            } };
  RICL RingTile_14x8_1 = { {-32,  -22}, { {-40, -38}                                                 } };
  RICL RingTile_14x8_2 = { {-32,  -22}, { {-30, -46}, {-52, -13}                                     } };
  RICL RingTile_14x8_3 = { {-32,  -22}, { {-15, -51}, {-40, -38}, {-52,  -8}                         } };
  RICL RingTile_14x8_4 = { {-32,  -22}, { {-12, -52}, {-32, -45}, {-49, -26}, {-52,  -1}             } };
  RICL RingTile_14x8_5 = { {-32,  -22}, { {-10, -52}, {-29, -46}, {-44, -33}, {-51, -15}, {-50,   6} } };
  RT(14x8)

  RICL RingTile_13x9_0 = { { -22, -32}, {                                                            } };
  RICL RingTile_13x9_1 = { { -22, -32}, { {-38, -40}                                                 } };
  RICL RingTile_13x9_2 = { { -22, -32}, { {-13, -52}, {-46, -30}                                     } };
  RICL RingTile_13x9_3 = { { -22, -32}, { { -8, -52}, {-38, -40}, {-51, -15}                         } };
  RICL RingTile_13x9_4 = { { -22, -32}, { { -1, -52}, {-26, -49}, {-45, -32}, {-52, -12},            } };
  RICL RingTile_13x9_5 = { { -22, -32}, { {  6, -50}, {-15, -51}, {-33, -44}, {-46, -29}, {-52, -10} } };
  RT(13x9)

  // This table maps the following enum's values to the actual
  // ring tile struct pointers.

  static const struct RingTile* RingTileLUT[] = {

    &RingTile_Center,
    &RingTile_FarLeft,
    &RingTile_Left,
    &RingTile_FarTop,
    &RingTile_Top,
    &RingTile_FarRight,
    &RingTile_Right,
    &RingTile_FarBottom,
    &RingTile_Bottom,
    &RingTile_0x0,
    &RingTile_1x0,
    &RingTile_0x1,
    &RingTile_1x1,
    &RingTile_0x9,
    &RingTile_1x9,
    &RingTile_0x8,
    &RingTile_1x8,
    &RingTile_13x0,
    &RingTile_14x0,
    &RingTile_14x1,
    &RingTile_13x1,
    &RingTile_13x9,
    &RingTile_14x9,
    &RingTile_13x8,
    &RingTile_14x8,

  };

  enum RingTileIndices {

    RT_C  = 0,
    RT_FL,
    RT_L,
    RT_FT,
    RT_T,
    RT_FR,
    RT_R,
    RT_FB,
    RT_B,
    RT_0x0,
    RT_1x0,
    RT_0x1,
    RT_1x1,
    RT_0x9,
    RT_1x9,
    RT_0x8,
    RT_1x8,
    RT_13x0,
    RT_14x0,
    RT_14x1,
    RT_13x1,
    RT_13x9,
    RT_14x9,
    RT_13x8,
    RT_14x8,
  };

  // This table is used to pick a ring layout for each tile on the map.

  static const u8 RingTiles[DISPLAY_HEIGHT_TILES / 2][DISPLAY_WIDTH_TILES / 2] = {
    {RT_0x0, RT_1x0, RT_FT, RT_FT, RT_FT, RT_FT, RT_FT, RT_FT, RT_FT, RT_FT, RT_FT, RT_FT, RT_FT, RT_13x0, RT_14x0},
    {RT_0x1, RT_1x1, RT_T,  RT_T,  RT_T,  RT_T,  RT_T,  RT_T,  RT_T,  RT_T,  RT_T,  RT_T,  RT_T,  RT_13x1, RT_14x1},
    {RT_FL,  RT_L,   RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_R,    RT_FR},
    {RT_FL,  RT_L,   RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_R,    RT_FR},
    {RT_FL,  RT_L,   RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_R,    RT_FR},
    {RT_FL,  RT_L,   RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_R,    RT_FR},
    {RT_FL,  RT_L,   RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_R,    RT_FR},
    {RT_FL,  RT_L,   RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_C,  RT_R,    RT_FR},
    {RT_0x8, RT_1x8, RT_B,  RT_B,  RT_B,  RT_B,  RT_B,  RT_B,  RT_B,  RT_B,  RT_B,  RT_B,  RT_B,  RT_13x8, RT_14x8},
    {RT_0x9, RT_1x9, RT_FB, RT_FB, RT_FB, RT_FB, RT_FB, RT_FB, RT_FB, RT_FB, RT_FB, RT_FB, RT_FB, RT_13x9, RT_14x9},
  };


// Ring tile functions

  const struct RingItemCoordinateList* GetRingCoordList(struct PlayerInterfaceProc* proc, int count)
  {
    /* Gets the unit's tile's item icon layout for the unit's item count.
     */

    u8 variant = RingTiles[proc->yCursor][proc->xCursor];

    return RingTileLUT[variant]->itemVariants[count];
  }


  void RenderRingHPOrb(int x, int y, struct Unit* unit)
  {
    /* Draws an HP orb for the unit based on their current health.
     */

    int hpRatio = Div(
        HP_ORB_LEVELS * GetUnitCurrentHp(unit),
        GetUnitMaxHp(unit)
      );

    CallARM_PushToSecondaryOAM(
        OAM1_X(x), OAM0_Y(y), &gObj_16x16,
        (
          (hpRatio > RING_HP_ORB_ROW_SIZE)
          ? RING_HP_ORB_ROW1 + ((hpRatio - RING_HP_ORB_ROW_SIZE) * 2)
          : RING_HP_ORB_ROW0 + (hpRatio * 2)
        )
      );

    return;
  }


  void RenderRingItems(int baseX, int baseY, int itemCount, const struct RingItemCoordinateList* coordList)
  {
    /* Draws a unit's inventory around the ring.
     */

    int i;
    struct RingItemCoordinates itemPosition;

    for ( i = 0; i < itemCount; i++ )
    {

      itemPosition = coordList->data[i];

      CallARM_PushToSecondaryOAM(
          OAM1_X(baseX + itemPosition.x),
          OAM0_Y(baseY + itemPosition.y),
          &gObj_16x16,
          INVENTORY_ICON_TILE + (2 * i)
        );

      CallARM_PushToSecondaryOAM(
          OAM1_X(baseX + itemPosition.x),
          OAM0_Y(baseY + itemPosition.y),
          &gObj_16x16,
          ITEM_BACKGROUND
        );

    }

    return;
  }


