#pragma once

// Some functions/variables that not in decomp header
// Later we need to fix them

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
