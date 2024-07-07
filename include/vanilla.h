#pragma once

// Some functions/variables that not in decomp header
// Later we need to fix them

#define CHAR_NEWLINE 0x01
extern struct Font * gActiveFont;

struct Vec1  { s8 x, y; };
struct Vec1u { u8 x, y; };

struct UnknownBMUSAilmentProc {
    PROC_HEADER;

    /* 29 */ u8 _pad1[0x2C-0x29];
    /* 2C */ int unk_2C;
    /* 30 */ int _pad2;
    /* 34 */ int unk_34;
    /* 38 */ u8 _pad3[0x4C-0x38];

    /* 4C */ s16 unk_4C;
    /* 4E */ u8 _pad4[0x58-0x4E];

    /* 58 */ int unk_58;
};

void ForEachUnitInRange(void(* func)(struct Unit * unit));
void AddUnitToTargetListIfNotAllied(struct Unit * unit);
extern struct Struct030017A0 gDungeonState;
void SetupBattleMOVEUNITs(void);
void StartStatusHealEffect(struct Unit * unit, ProcPtr proc);
extern const u16 * sUiFramePaletteLookup[];
extern const u16 * sStatBarPaletteLookup[];

struct WeaponTriangleRule {
    s8 attackerWeaponType;
    s8 defenderWeaponType;
    s8 hitBonus;
    s8 atkBonus;
};
extern const struct WeaponTriangleRule sWeaponTriangleRules[];

extern struct Text gPrepUnitTexts[];
void PrepUnit_DrawSMSAndObjs(struct ProcPrepUnit * proc);
void PrepUnit_InitSMS(struct ProcPrepUnit * proc);
void PrepUnit_DrawUnitListNames(struct ProcPrepUnit * proc, int line);
void PrepUnit_InitGfx(void);
void PrepUnit_DrawUnitItems(struct Unit * unit);
void PrepUnit_DrawLeftUnitName(struct Unit * unit);
void PrepUnit_DrawLeftUnitNameCur(struct ProcPrepUnit * proc);
s8 ShouldPrepUnitMenuScroll(struct ProcPrepUnit * proc);
void sub_809AE10(struct ProcPrepUnit * proc);
void PrepUpdateMenuTsaScroll(int val);
void PrepUnitDisableDisp(void);
void PrepUnitEnableDisp(void);
void sub_809B504(struct ProcPrepUnit * proc);
void sub_809B014(void);
void sub_809B520(struct ProcPrepUnit * proc);

struct CpPerformProc {
    /* 00 */ PROC_HEADER;

    /* 2C */ s8 (* func)(struct CpPerformProc * proc);
    /* 30 */ u8 unk_30;
    /* 31 */ u8 isUnitVisible;
};

bool AiDummyAction(struct CpPerformProc *);
bool AiEscapeAction(struct CpPerformProc *);
bool AiWaitAndClearScreenAction(struct CpPerformProc *);
bool AiPillageAction(struct CpPerformProc *);
bool AiStaffAction(struct CpPerformProc *);
bool AiUseItemAction(struct CpPerformProc *);
bool AiRefreshAction(struct CpPerformProc *);
bool AiTalkAction(struct CpPerformProc *);
bool AiRideBallistaAction(struct CpPerformProc *);
bool AiExitBallistaAction(struct CpPerformProc *);
bool AiPickAction(struct CpPerformProc *);

void AiStartCombatAction(struct CpPerformProc * proc);
void AiStartEscapeAction(struct CpPerformProc * proc);
void AiStartStealAction(struct CpPerformProc * proc);
void AiDKNightmareAction(struct CpPerformProc * proc);
void AiDKSummonAction(struct CpPerformProc * proc);
void AiStartCombatAction(struct CpPerformProc * proc);
void AiStartCombatAction(struct CpPerformProc * proc);
void AiStartCombatAction(struct CpPerformProc * proc);

enum ScriptKind {
    AI_SCRIPT_AI1,
    AI_SCRIPT_AI2,
};

typedef void (* AiScrCmd)(u8 * pc);

extern s8 gAiScriptEnded;
extern int gAiScriptKind;
extern struct AiScr * gpAiScriptCurrent;

extern struct AiScr CONST_DATA gAiScript_FallbackAi1[];
extern struct AiScr CONST_DATA gAiScript_FallbackAi2[];

void AiScript_Exec(u8 *);

void SetSupportLevelGained(u8 charA, u8 charB);
s8 HasUnitGainedSupportLevel(struct Unit * unit, int num);
extern int sSupportMaxExpLookup[];

void TryAddUnitToHealTargetList(struct Unit * unit);
void TryAddUnitToRestoreTargetList(struct Unit * unit);
void TryAddUnitToBarrierTargetList(struct Unit * unit);
void TryAddUnitToRescueStaffTargetList(struct Unit * unit);
void TryAddUnitToSilenceTargetList(struct Unit * unit);
void TryAddUnitToSleepTargetList(struct Unit * unit);
void TryAddUnitToBerserkTargetList(struct Unit * unit);
void TryAddUnitToWarpTargetList(struct Unit * unit);
void TryAddUnitToHammerneTargetList(struct Unit * unit);

extern struct ProcCmd CONST_DATA ProcScr_DanceringAnim[];

void BeginUnitHealAnim(struct Unit * unit, int hp);

extern u16 sSprite_SysUpArrowA[], sSprite_SysUpArrowB[], sSprite_SysUpArrowC[];
extern u16 sSprite_SysDownArrowA[], sSprite_SysDownArrowB[], sSprite_SysDownArrowC[];

void PutChapterMarkedTileIconOam(void);

extern u16 gRNSeeds[3];

#define SPELL_ASSOC_DATA(_item, _count, _efx, _pcmd, _stat, _facing, _color)   \
{                                                                               \
    .item           = _item,                                                    \
    .count          = _count,                                                   \
    .efx            = _efx,                                                    \
    .pcmd_manim     = _pcmd,                                                    \
    .stat           = _stat,                                                    \
    .facing         = _facing,                                                  \
    .flash_color    = _color,                                                   \
}

#define SPELL_ASSOC_DATA_WPN(_item, _efx)  \
    SPELL_ASSOC_DATA(_item, 2, _efx, NULL, true, MA_FACING_OPPONENT, SPELL_ASSOC_MCOLOR_NORMAL)

#define SPELL_ASSOC_DATA_WPN_MAGIC(_item, _efx, flash_color)   \
    SPELL_ASSOC_DATA(_item, 2, _efx, NULL, true, MA_FACING_OPPONENT, flash_color)

#define SPELL_ASSOC_DATA_WPN_DEFAULT(_item) SPELL_ASSOC_DATA_WPN(_item, -1)
#define SPELL_ASSOC_DATA_ITEM(_item) SPELL_ASSOC_DATA_WPN(_item, 0x32)

extern void (* gUnknown_03003128)(void);

void BmMapInit(void * buffer, u8 *** outHandle, int width, int height);
void RevertMovementScript(u8 * begin, u8 * end);
#define TERRAIN_AT(x, y) gBmMapTerrain[y][x]
#define LAST_X_POINT gpPathArrowProc->pathX[gpPathArrowProc->pathLen]
#define LAST_Y_POINT gpPathArrowProc->pathY[gpPathArrowProc->pathLen]

static inline s8 GetBmMapPointAtCursor()
{
    return gWorkingBmMap[gBmSt.playerCursor.y][gBmSt.playerCursor.x];
}

static inline u8 GetTerrainAtCursor()
{
    return TERRAIN_AT(gBmSt.playerCursor.x, gBmSt.playerCursor.y);
}
