
/* Common definitions
 *
 * Many of the definitions here are translations between
 * CLib and the decompilation's names. Some things are
 * for definitions that are missing from CLib and/or the
 * decompilation.
 */

#include "gbafe.h"


// Proc definitions

  enum GEProcLabels
  {
    /* Proc instruction labels
     */
    GE_PROC_EXTEND         = 0,
    GE_PROC_CHECK_FOR_UNIT = 1,
    GE_PROC_UPDATE         = 2,
    GE_PROC_RETRACT        = 3,
  };

  struct PlayerInterfaceProc
  {
    /* This proc is used by the vanilla game for
     * the minimug box, burst window, terrain window,
     * and goal window.
     */

    /* 00 */ PROC_FIELDS;

    /* 2C */ struct TextHandle textLines[2];

    /* 3C */ s8 burstTilemapX;
    /* 3D */ s8 burstTilemapY;
    /* 3E */ s8 burstPendingTilemapWidth;
    /* 3F */ s8 burstPendingTilemapHeight;

    /* 40 */ s16* statusTilemap;

    /* 44 */ s16 hoverFramecount;

    /* 46 */ s16 hpX;
    /* 48 */ s16 hpY;

    /* 4A */ u8 burstOldUnit;
    /* 4B */ u8 burstNewUnit;

    /* 4C */ u8 xCursorPrevious;
    /* 4D */ u8 yCursorPrevious;
    /* 4E */ u8 xCursor;
    /* 4F */ u8 yCursor;

    /* 50 */ s8 cursorQuadrant;

    /* 51 */ u8 currentHPUpperDigit;
    /* 52 */ u8 currentHPLowerDigit;
    /* 53 */ u8 maxHPUpperDigit;
    /* 54 */ u8 maxHPLowerDigit;

    /* 55 */ u8 busyFlag;
    /* 56 */ s8 isRetracting;
    /* 57 */ s8 windowQuadrant;
    /* 58 */ int movementFrame;

  };

  struct UnitDataProc
  {
    /* This proc tracks various unit-related
     * variables for the window. The latter part
     * of it is the non-Unit-struct part of the
     * `BattleUnit` struct.
     */

    /* 00 */ PROC_FIELDS;

    /* 2C */ struct Unit* unit;

    /* 30 */ u16 weapon;
    /* 32 */ u16 weaponBefore;
    /* 34 */ u32 weaponAttributes;
    /* 38 */ u8 weaponType;
    /* 39 */ u8 weaponSlotIndex;

    /* 3A */ s8 canCounter;

    /* 3B */ s8 wTriangleHitBonus;
    /* 3C */ s8 wTriangleDmgBonus;

    /* 3D */ u8 terrainId;
    /* 3E */ s8 terrainDefense;
    /* 3F */ s8 terrainAvoid;
    /* 40 */ s8 terrainResistance;
    /* 41 */ /* pad */

    /* 42 */ short battleAttack;
    /* 44 */ short battleDefense;
    /* 46 */ short battleSpeed;
    /* 48 */ short battleHitRate;
    /* 4A */ short battleAvoidRate;
    /* 4C */ short battleEffectiveHitRate;
    /* 4E */ short battleCritRate;
    /* 50 */ short battleDodgeRate;
    /* 52 */ short battleEffectiveCritRate;
    /* 54 */ short battleSilencerRate;

    /* 56 */ s8 expGain;
    /* 57 */ s8 statusOut;
    /* 58 */ u8 levelPrevious;
    /* 59 */ u8 expPrevious;

    /* 5A */ u8 hpInitial;

    /* 5B */ s8 changeHP;
    /* 5C */ s8 changePow;
    /* 5D */ s8 changeSkl;
    /* 5E */ s8 changeSpd;
    /* 5F */ s8 changeDef;
    /* 60 */ s8 changeRes;
    /* 61 */ s8 changeLck;
    /* 62 */ s8 changeCon;

    /* 63 */ s8 wexpMultiplier;
    /* 64 */ s8 nonZeroDamage;
    /* 65 */ s8 weaponBroke;

    /* 66 */ s8 hasItemEffectTarget;

  };


// Definitions and translations

  #define DISPLAY_WIDTH  240
  #define DISPLAY_HEIGHT 160

  #define DISPLAY_WIDTH_TILES  (DISPLAY_WIDTH / 8)
  #define DISPLAY_HEIGHT_TILES (DISPLAY_HEIGHT / 8)

  extern u16 gpStatScreenPageBg0Map[0x280];
  #define gUiTmScratchA gpStatScreenPageBg0Map

  extern u16 gpStatScreenPageBg1Map[0x280];
  #define gUiTmScratchB gpStatScreenPageBg1Map

  #define TILE_HFLIP   (1 << 10)
  #define TILE_VFLIP   (1 << 11)
  #define TILE_PALETTE 0xF000

  #define CHR_SIZE 0x20

  #define FID_FACTION_CHIBI 0x7F00 // Decomp's name
  #define IS_GENERIC_MINIMUG(portraitID) (portraitID >= FID_FACTION_CHIBI)

  #define OAM0_Y(ay)         ((ay) & 0x00FF)
  #define OAM0_AFFINE_ENABLE 0x0100
  #define OAM0_DOUBLESIZE    0x0200
  #define OAM0_DISABLE       0x0200
  #define OAM0_BLEND         0x0400
  #define OAM0_WINDOW        0x0800
  #define OAM0_MOSAIC        0x1000
  #define OAM0_256COLORS     0x2000
  #define OAM0_SHAPE_8x8     0x0000
  #define OAM0_SHAPE_16x16   0x0000
  #define OAM0_SHAPE_32x32   0x0000
  #define OAM0_SHAPE_64x64   0x0000
  #define OAM0_SHAPE_16x8    0x4000
  #define OAM0_SHAPE_32x8    0x4000
  #define OAM0_SHAPE_32x16   0x4000
  #define OAM0_SHAPE_64x32   0x4000
  #define OAM0_SHAPE_8x16    0x8000
  #define OAM0_SHAPE_8x32    0x8000
  #define OAM0_SHAPE_16x32   0x8000
  #define OAM0_SHAPE_32x64   0x8000

  #define OAM1_X(ax)         ((ax) & 0x01FF)
  #define OAM1_AFFINE_ID(ai) (((ai) << 9) & 0x3E00)
  #define OAM1_HFLIP         0x1000
  #define OAM1_VFLIP         0x2000
  #define OAM1_SIZE_8x8      0x0000
  #define OAM1_SIZE_16x8     0x0000
  #define OAM1_SIZE_8x16     0x0000
  #define OAM1_SIZE_16x16    0x4000
  #define OAM1_SIZE_32x8     0x4000
  #define OAM1_SIZE_8x32     0x4000
  #define OAM1_SIZE_32x32    0x8000
  #define OAM1_SIZE_32x16    0x8000
  #define OAM1_SIZE_16x32    0x8000
  #define OAM1_SIZE_64x64    0xC000
  #define OAM1_SIZE_64x32    0xC000
  #define OAM1_SIZE_32x64    0xC000

  #define OAM2_CHR(ac)   ((ac) & 0x3FF)
  #define OAM2_LAYER(al) (((al) & 0x3) << 10)
  #define OAM2_PAL(ap)   (((ap) & 0xF) << 12)

  #define ITEM_INDEX(aItem) ((aItem) & 0xFF)
  #define ITEM_USES(aItem) ((aItem) >> 8)

  struct PlayerInterfaceConfigEntry {
    /* This is different from the decomp version of
     * this struct in that I need to manually pad
     * entries to the next word boundary or it'll
     * generate the assembly wrong.
     */
    s8 xTerrain, yTerrain;
    s8 xMinimug, yMinimug;
    s8 xGoal, yGoal;
    s8 pad1, pad2;
  };

  extern struct PlayerInterfaceConfigEntry sPlayerInterfaceConfigLut[];

  int GetCursorQuadrant(void);
  int GetWindowQuadrant(int x, int y);

  extern struct ProcInstruction gProc_CameraMovement[];


// Helpers

  // Cursor position getter

  static inline struct Unit* GetUnitAtCursor(void)
  {
    /* Get a unit under the cursor in a mid-battle map.
     */
    struct Unit* unit = GetUnit(
        gMapUnit[gGameState.cursorMapPos.y][gGameState.cursorMapPos.x]
      );
    return unit;
  }

  // Number drawing helpers to avoid repetition
  // See modules/Number

  struct Vec2 GE_GetWindowPosition(struct PlayerInterfaceProc* proc);
  void DrawUnsignedSmallNumber(int number, int x, int y);

  static inline void GenericNumberTwoDigits(int number, int x, int y, struct PlayerInterfaceProc* proc)
  {
    /* Draw a two digit number as objects.
     *
     * Numbers above 99 are drawn as `--`.
     */
    struct Vec2 coords = GE_GetWindowPosition(proc);

    DrawUnsignedSmallNumber(
        (number >= 100) ? 0xFF : number,
        (coords.x * 8) + x,
        (coords.y * 8) + y
      );

    return;
  }

  static inline void GenericNumberThreeDigits(int number, int x, int y, struct PlayerInterfaceProc* proc)
  {
    /* Draw a three digit number as objects.
     *
     * Numbers above 254 will be drawn as `--`.
     */
    struct Vec2 coords = GE_GetWindowPosition(proc);

    DrawUnsignedSmallNumber(
        (number >= 255) ? 0xFF : number,
        (coords.x * 8) + x,
        (coords.y * 8) + y
      );

    return;
  }
