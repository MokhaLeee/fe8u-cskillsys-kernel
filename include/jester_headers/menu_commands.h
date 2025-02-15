u8 RefugeEffect(struct MenuProc* menu, struct MenuItemProc* menuItem);
u8 RefugeUsability(const struct MenuItemDef* def, int number);
void RefreshUnitTakeRefugeInfoWindows(ProcPtr);
// void RefreshUnitRefugeInfoWindows(struct Unit* unit);
void RefugeSelection_OnConstruction(ProcPtr proc);
u8 RefugeSelection_OnChange(ProcPtr proc, struct SelectTarget* target);
u8 RefugeSelection_OnSelect(ProcPtr proc, struct SelectTarget* target);
u8 RefugeSelection_OnHelp(ProcPtr proc, struct SelectTarget * target);
s8 CanUnitRefuge(struct Unit* actor, struct Unit* target);
void TryAddUnitToRefugeTargetList(struct Unit* unit);
void MakeRefugeTargetList(struct Unit* unit);

extern const struct UnitInfoWindowProc * sRescueUnitInfoWindows[2];

enum
{
    LINES_MAX = 5
};

struct UnitInfoWindowProc {
    /* 00 */ PROC_HEADER;

    /* 2C */ struct Unit* unit;

    /* 30 */ struct Text name;
    /* 38 */ struct Text lines[LINES_MAX];

    /* 60 */ u8 x;
    /* 61 */ u8 y;
    /* 62 */ u8 xUnitSprite;
    /* 63 */ u8 xNameText;
};