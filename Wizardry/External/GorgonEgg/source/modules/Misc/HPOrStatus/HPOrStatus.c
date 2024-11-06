
#include "gbafe.h"
#include "CommonDefinitions.h"
#include "GeneratedDefinitions.h"

struct Vec2 GE_GetWindowPosition(struct PlayerInterfaceProc* proc);
void GE_CopyTilemap(struct PlayerInterfaceProc* proc);

void DrawUnsignedSmallNumber(int number, int x, int y);

extern u8 gGfx_StatusText[];

// This is the gorgon egg regeneration status.
#define UNIT_STATUS_GORGONEGG UNIT_STATUS_RECOVER

#define BLANK_TILE TILEREF(0, 0)

#define HP_LABEL_TILE0 TILEREF(HP_LABEL_BASE_TILE, HP_LABEL_PALETTE)
#define HP_LABEL_TILE1 TILEREF(HP_LABEL_BASE_TILE + 1, HP_LABEL_PALETTE)

#define HP_DIVIDER_TILE TILEREF(HP_DIVIDER_BASE_TILE, HP_DIVIDER_PALETTE)

#define STATUS_TILE TILEREF(STATUS_BASE_TILE, STATUS_PALETTE)

#define BG_NUMBER_TILE TILEREF(BG_NUMBER_BASE_TILE, BG_NUMBER_PALETTE)

// These definitions are formatted this way to allow users to simply
// specify `HPORSTATUS_X` and `HPORSTATUS_Y` instead of defining the
// positions of components, but still allowing component definitions if
// desired.

#ifdef HPORSTATUS_X

  #ifndef HP_LABEL_X
    #define HP_LABEL_X (HPORSTATUS_X + 0)
    #endif // HP_LABEL_X

  #ifndef STATUS_X
    #define STATUS_X (HPORSTATUS_X + 0)
    #endif // STATUS_X

  #ifndef CURRENT_HP_X
    #define CURRENT_HP_X ((HPORSTATUS_X * 8) + 24)
    #endif // CURRENT_HP_X

  #ifndef MAX_HP_X
    #define MAX_HP_X ((HPORSTATUS_X * 8) + 48)
    #endif // MAX_HP_X

  #endif // HPORSTATUS_X

#ifdef HPORSTATUS_Y

  #ifndef HP_LABEL_Y
    #define HP_LABEL_Y (HPORSTATUS_Y + 0)
    #endif // HP_LABEL_Y

  #ifndef STATUS_Y
    #define STATUS_Y (HPORSTATUS_Y + 0)
    #endif // STATUS_Y

  #ifndef CURRENT_HP_Y
    #define CURRENT_HP_Y ((HPORSTATUS_Y * 8) + 0)
    #endif // CURRENT_HP_Y

  #ifndef MAX_HP_Y
    #define MAX_HP_Y ((HPORSTATUS_Y * 8) + 0)
    #endif // MAX_HP_Y

  #endif // HPORSTATUS_Y


void DrawStatus(struct Unit* unit)
{
  /* Draws the status image and tilemap.
   */
  int imageOffset, imageTileIndex;
  u16* tilemapPosition;

  #define STATUS_IMAGE_SIZE (5 * CHR_SIZE)

  enum VanillaStatusTextImages
  {
    PoisonImage  = 0,
    SleepImage   = 1,
    SilenceImage = 2,
    BerserkImage = 3,
    AttackImage  = 4,
    DefenseImage = 5,
    CritImage    = 6,
    AvoidImage   = 7,
    PetrifyImage = 8,
  };

  #define StatusImage(Status, Index)             \
    case (Status):                               \
      imageOffset = STATUS_IMAGE_SIZE * (Index); \
      break;

  switch ( unit->statusIndex )
  {
    StatusImage(UNIT_STATUS_POISON,   PoisonImage);
    StatusImage(UNIT_STATUS_SLEEP,    SleepImage);
    StatusImage(UNIT_STATUS_SILENCED, SilenceImage);
    StatusImage(UNIT_STATUS_BERSERK,  BerserkImage);
    StatusImage(UNIT_STATUS_ATTACK,   AttackImage);
    StatusImage(UNIT_STATUS_DEFENSE,  DefenseImage);
    StatusImage(UNIT_STATUS_CRIT,     CritImage);
    StatusImage(UNIT_STATUS_AVOID,    AvoidImage);
    StatusImage(UNIT_STATUS_PETRIFY,  PetrifyImage);

    // If the unit has a status we don't recognize
    // (or no status at all), don't draw.

    default:
      return;

  }

  CpuFastCopy(
      gGfx_StatusText + imageOffset,
      (void*)(VRAM + (STATUS_BASE_TILE * CHR_SIZE)),
      STATUS_IMAGE_SIZE
    );

  imageTileIndex = STATUS_TILE;
  tilemapPosition = gUiTmScratchA + TILEMAP_INDEX(STATUS_X, STATUS_Y);

  tilemapPosition[0] = imageTileIndex++;
  tilemapPosition[1] = imageTileIndex++;
  tilemapPosition[2] = imageTileIndex++;
  tilemapPosition[3] = imageTileIndex++;
  tilemapPosition[4] = imageTileIndex++;
  tilemapPosition[5] = BLANK_TILE;
  tilemapPosition[6] = BG_NUMBER_TILE + unit->statusDuration;

  return;
}


void HPOrStatus_Dynamic(struct PlayerInterfaceProc* proc, struct UnitDataProc* udp)
{
  /* Alternates between drawing HP and status, as vanilla does.
   */
  int currentHP, maxHP;
  struct Vec2 coords;
  struct Unit* unit = udp->unit;

  s16 frame = proc->hoverFramecount;

  // Gorgon eggs should always display HP.
  if ( unit->statusIndex == UNIT_STATUS_GORGONEGG )
    frame = 0;

  // Every 64 frames, either draw the status or
  // draw the HP label.
  if ( ((frame & 0x3F) == 0 ) && (proc->busyFlag == FALSE) )
  {
    if ( (frame & 0x40) != 0 )
    {
      DrawStatus(unit);
    }
    else
    {
      gUiTmScratchA[TILEMAP_INDEX(HP_LABEL_X + 0, HP_LABEL_Y)] = HP_LABEL_TILE0;
      gUiTmScratchA[TILEMAP_INDEX(HP_LABEL_X + 1, HP_LABEL_Y)] = HP_LABEL_TILE1;
      gUiTmScratchA[TILEMAP_INDEX(HP_LABEL_X + 2, HP_LABEL_Y)] = BLANK_TILE;
      gUiTmScratchA[TILEMAP_INDEX(HP_LABEL_X + 3, HP_LABEL_Y)] = BLANK_TILE;
      gUiTmScratchA[TILEMAP_INDEX(HP_LABEL_X + 4, HP_LABEL_Y)] = HP_DIVIDER_TILE;
      gUiTmScratchA[TILEMAP_INDEX(HP_LABEL_X + 5, HP_LABEL_Y)] = BLANK_TILE;
      gUiTmScratchA[TILEMAP_INDEX(HP_LABEL_X + 6, HP_LABEL_Y)] = BLANK_TILE;
    }

    GE_CopyTilemap(proc);
  }

  // During the time that the status is not being displayed,
  // draw the HP numbers.
  if ( (proc->busyFlag == FALSE) && (((frame & 0x40) == 0) || (unit->statusIndex == UNIT_STATUS_NONE)) )
  {

    coords = GE_GetWindowPosition(proc);

    // Units with more than 2 digits of HP
    // have their HP drawn as `--`.
    currentHP = GetUnitCurrentHp(unit);
    maxHP = GetUnitMaxHp(unit);

    DrawUnsignedSmallNumber(
        (currentHP >= 100) ? 0xFF : currentHP,
        (coords.x * 8) + CURRENT_HP_X,
        (coords.y * 8) + CURRENT_HP_Y
      );

    DrawUnsignedSmallNumber(
        (maxHP >= 100) ? 0xFF : maxHP,
        (coords.x * 8) + MAX_HP_X,
        (coords.y * 8) + MAX_HP_Y
      );

  }

  return;
}
