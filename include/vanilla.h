#pragma once

// Some functions/variables that not in decomp header
// Later we need to fix them

#define CHAR_NEWLINE 0x01
extern struct Font *gActiveFont;

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

void ForEachUnitInRange(void(*func)(struct Unit *unit));
void AddUnitToTargetListIfNotAllied(struct Unit *unit);
extern struct Struct030017A0 gDungeonState;
void SetupBattleMOVEUNITs(void);
void StartStatusHealEffect(struct Unit *unit, ProcPtr proc);
extern const u16 *sUiFramePaletteLookup[];
extern const u16 *sStatBarPaletteLookup[];

struct WeaponTriangleRule {
	s8 attackerWeaponType;
	s8 defenderWeaponType;
	s8 hitBonus;
	s8 atkBonus;
};
extern const struct WeaponTriangleRule sWeaponTriangleRules[];

extern struct Text gPrepUnitTexts[];
void PrepUnit_DrawSMSAndObjs(struct ProcPrepUnit *proc);
void PrepUnit_InitSMS(struct ProcPrepUnit *proc);
void PrepUnit_DrawUnitListNames(struct ProcPrepUnit *proc, int line);
void PrepUnit_InitGfx(void);
void PrepUnit_DrawUnitItems(struct Unit *unit);
void PrepUnit_DrawLeftUnitName(struct Unit *unit);
void PrepUnit_DrawLeftUnitNameCur(struct ProcPrepUnit *proc);
s8 ShouldPrepUnitMenuScroll(struct ProcPrepUnit *proc);
void sub_809AE10(struct ProcPrepUnit *proc);
void PrepUpdateMenuTsaScroll(int val);
void PrepUnitDisableDisp(void);
void PrepUnitEnableDisp(void);
void sub_809B504(struct ProcPrepUnit *proc);
void sub_809B014(void);
void sub_809B520(struct ProcPrepUnit *proc);

void AiPhaseInit(struct Proc *proc);
void AiPhaseCleanup(struct Proc *proc);

struct CpPerformProc {
	/* 00 */ PROC_HEADER;

	/* 2C */ s8 (*func)(struct CpPerformProc *proc);
	/* 30 */ u8 unk_30;
	/* 31 */ u8 isUnitVisible;
};

bool AiDummyAction(struct CpPerformProc *proc);
bool AiEscapeAction(struct CpPerformProc *proc);
bool AiWaitAndClearScreenAction(struct CpPerformProc *proc);
bool AiPillageAction(struct CpPerformProc *proc);
bool AiStaffAction(struct CpPerformProc *proc);
bool AiUseItemAction(struct CpPerformProc *proc);
bool AiRefreshAction(struct CpPerformProc *proc);
bool AiTalkAction(struct CpPerformProc *proc);
bool AiRideBallistaAction(struct CpPerformProc *proc);
bool AiExitBallistaAction(struct CpPerformProc *proc);
bool AiPickAction(struct CpPerformProc *proc);

void AiStartCombatAction(struct CpPerformProc *proc);
void AiStartEscapeAction(struct CpPerformProc *proc);
void AiStartStealAction(struct CpPerformProc *proc);
void AiDKNightmareAction(struct CpPerformProc *proc);
void AiDKSummonAction(struct CpPerformProc *proc);
void AiStartCombatAction(struct CpPerformProc *proc);
void AiStartCombatAction(struct CpPerformProc *proc);
void AiStartCombatAction(struct CpPerformProc *proc);

enum ScriptKind {
	AI_SCRIPT_AI1,
	AI_SCRIPT_AI2,
};

typedef void (*AiScrCmd)(u8 *pc);

extern s8 gAiScriptEnded;
extern int gAiScriptKind;
extern struct AiScr *gpAiScriptCurrent;

extern struct AiScr CONST_DATA gAiScript_FallbackAi1[];
extern struct AiScr CONST_DATA gAiScript_FallbackAi2[];

void AiScript_Exec(u8 *);

void SetSupportLevelGained(u8 charA, u8 charB);
s8 HasUnitGainedSupportLevel(struct Unit *unit, int num);
extern int sSupportMaxExpLookup[];

void TryAddUnitToHealTargetList(struct Unit *unit);
void TryAddUnitToRestoreTargetList(struct Unit *unit);
void TryAddUnitToBarrierTargetList(struct Unit *unit);
void TryAddUnitToRescueStaffTargetList(struct Unit *unit);
void TryAddUnitToSilenceTargetList(struct Unit *unit);
void TryAddUnitToSleepTargetList(struct Unit *unit);
void TryAddUnitToBerserkTargetList(struct Unit *unit);
void TryAddUnitToWarpTargetList(struct Unit *unit);
void TryAddUnitToHammerneTargetList(struct Unit *unit);

extern struct ProcCmd CONST_DATA ProcScr_DanceringAnim[];

void BeginUnitHealAnim(struct Unit *unit, int hp);

extern u16 sSprite_SysUpArrowA[], sSprite_SysUpArrowB[], sSprite_SysUpArrowC[];
extern u16 sSprite_SysDownArrowA[], sSprite_SysDownArrowB[], sSprite_SysDownArrowC[];

void PutChapterMarkedTileIconOam(void);

extern u16 gRNSeeds[3];

#define SPELL_ASSOC_DATA(_item, _count, _efx, _pcmd, _stat, _facing, _color) \
{ \
	.item = _item, \
	.count = _count, \
	.efx = _efx, \
	.pcmd_manim = _pcmd, \
	.stat = _stat, \
	.facing = _facing, \
	.flash_color = _color, \
}

#define SPELL_ASSOC_DATA_WPN(_item, _efx)  \
	SPELL_ASSOC_DATA(_item, 2, _efx, NULL, true, MA_FACING_OPPONENT, SPELL_ASSOC_MCOLOR_NORMAL)

#define SPELL_ASSOC_DATA_WPN_MAGIC(_item, _efx, flash_color)   \
	SPELL_ASSOC_DATA(_item, 2, _efx, NULL, true, MA_FACING_OPPONENT, flash_color)

#define SPELL_ASSOC_DATA_WPN_DEFAULT(_item) SPELL_ASSOC_DATA_WPN(_item, -1)
#define SPELL_ASSOC_DATA_ITEM(_item) SPELL_ASSOC_DATA_WPN(_item, 0x32)

extern void (*gUnknown_03003128)(void);

void BmMapInit(void *buffer, u8 ***outHandle, int width, int height);
void RevertMovementScript(u8 *begin, u8 *end);
#define TERRAIN_AT(x, y) gBmMapTerrain[y][x]
#define LAST_X_POINT gpPathArrowProc->pathX[gpPathArrowProc->pathLen]
#define LAST_Y_POINT gpPathArrowProc->pathY[gpPathArrowProc->pathLen]

static inline s8 GetBmMapPointAtCursor(void)
{
	return gWorkingBmMap[gBmSt.playerCursor.y][gBmSt.playerCursor.x];
}

static inline u8 GetTerrainAtCursor(void)
{
	return TERRAIN_AT(gBmSt.playerCursor.x, gBmSt.playerCursor.y);
}

bool CanShowUnitStatScreen(struct Unit *unit);

#define EVT_CMD_LO(cmd) (((cmd) & 0x0000FFFF))
#define EVT_CMD_HI(cmd) (((cmd) & 0xFFFF0000) >> 16)
#define EVT_CMD_B1(cmd) (((cmd) & 0x000000FF))
#define EVT_CMD_B2(cmd) (((cmd) & 0x0000FF00) >> 8)
#define EVT_CMD_B3(cmd) (((cmd) & 0x00FF0000) >> 16)
#define EVT_CMD_B4(cmd) (((cmd) & 0xFF000000) >> 24)

extern struct EventListCmdInfo CONST_DATA gEventListCmdInfoTable[];

struct EvCheck03 {
	u32 unk0;
	u32 script;
	u8 pidA;
	u8 pidB;
	u16 fillerA;
	u16 unkC;
	u16 unkE;
};

bool CanUnitCrossTerrain(struct Unit *unit, int terrain);
extern CONST_DATA struct MenuItemDef gUnitActionMenuItems[];
void StartMineAnim(ProcPtr, int, int);
void AddUnitToTargetListIfAllied(struct Unit *unit);

extern CONST_DATA AnimScr AnimScr_EkrPopup[2];

struct Struct02013648 {
	/* 00 */ struct Font font;
	/* 18 */ struct Text th[16];
};

int GetUnitBestWRankType(struct Unit *unit);
u16 GetChapterDeathCount(void);
char *GetPidDefeatedEndingString(int pid);

void sub_80B6CA8(struct EndingBattleDisplayProc *proc);

bool CheckTutorialEvent(u8 type);
void StatusDecayDisplay_Display(struct UnknownBMUSAilmentProc *proc);
void sub_803B678(struct Unit *unit, u16 item);
void SetupUnitHealStaffAIFlags(struct Unit *unit, u16);
void DisplayPageNameSprite(int pageid);
void sub_808A200(const struct HelpBoxInfo *info);
void HelpBoxSetupstringLines(struct ProcHelpBoxIntro *proc);
void HelpBoxDrawstring(struct ProcHelpBoxIntro *proc);
void CpPerform_Cleanup(struct CpPerformProc *proc);
void CpPerform_PerformAction(struct CpPerformProc *proc);
void CpPerform_MoveCameraOntoTarget(struct CpPerformProc *proc);

void ForEachUnitInRange(void(*func)(struct Unit *unit));
void AddUnitToTargetListIfAllied(struct Unit *unit);
void sub_8095C2C(struct ProcAtMenu *proc);
extern struct MsgBuffer sMsgString;
extern EWRAM_DATA int sActiveMsg;
extern CONST_DATA struct ProcCmd sProcScr_BattleAnimSimpleLock[];

struct AiStaffLutEntry {
	u16 itemId;
	void (*func)(int itemIdx, s8 (*isEnemy)(struct Unit *unit));
};
extern const struct AiStaffLutEntry sAiStaffFuncLut[];
int GetAiStaffFuncIndex(u16 item);
int sub_8003ABC(u16 a, u16 b);
bool LoadBonusContentData(void *buf);

/**
 * popup
 */
int ParsePopupInstAndGetLen(struct PopupProc *proc);
void GeneratePopupText(const struct PopupInstruction *inst, struct Text th);
