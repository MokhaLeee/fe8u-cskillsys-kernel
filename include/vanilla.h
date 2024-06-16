#pragma once

// Some functions/variables that not in decomp header
// Later we need to fix them

#define CHAR_NEWLINE 0x01
extern struct Font * gActiveFont;

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
