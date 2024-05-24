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
