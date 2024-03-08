#include "global.h"
#include "fontgrp.h"
#include "bksel.h"
#include "hardware.h"
#include "statscreen.h"
#include "helpbox.h"

#include "common-chax.h"
#include "stat-screen.h"
#include "help-box.h"
#include "battle-system.h"
#include "combat-art.h"
#include "constants/texts.h"

extern _DECL_INFO * const RTextCombatBkselStandard, * const RTextCombatBkselExtended;
extern _DECL_INFO * const RTextCombatBkselStandardNoArts, * const RTextCombatBkselExtendedNoArts;

/* LynJump */
u8 StartBattleForecastHelpBox(ProcPtr parent, struct SelectTarget * target)
{
    int x;

    struct BattleForecastProc * proc = Proc_Find(gProcScr_BKSEL);
    if (proc == 0)
        return 0;

    if (proc->needContentUpdate != 0)
        return 0;

#if CHAX
    if (!COMBART_VALID(GetCombatArtInForce(gActiveUnit)))
        return 0;
#endif

    if (proc->side < 0)
        x = 0;
    else
        x = 20;

    LoadHelpBoxGfx(0, -1);

    switch (proc->frameKind) {
    case 1:
        if (CanUnitPlayCombatArt(gActiveUnit, gActiveUnit->items[0]))
            StartMovingHelpBoxExt(RTextCombatBkselStandard, parent, x, 0);
        else
            StartMovingHelpBoxExt(RTextCombatBkselStandardNoArts, parent, x, 0);
        break;

    case 2:
        if (CanUnitPlayCombatArt(gActiveUnit, gActiveUnit->items[0]))
            StartMovingHelpBoxExt(RTextCombatBkselExtended, parent, x, 0);
        else
            StartMovingHelpBoxExt(RTextCombatBkselExtendedNoArts, parent, x, 0);
        break;
    }
    return 0;
}

void DrawHelpBoxCombatArtBkselLabels(void)
{
    Text_InsertDrawString(&gHelpBoxSt.text[0], 0x00, TEXT_COLOR_47CF, GetStringFromIndex(0x503)); // "Mt"
    Text_InsertDrawString(&gHelpBoxSt.text[0], 0x30, TEXT_COLOR_47CF, GetStringFromIndex(MSG_COMBATART_BKSEL_HB_Cost));
    Text_InsertDrawString(&gHelpBoxSt.text[0], 0x60, TEXT_COLOR_47CF, GetStringFromIndex(0x4F5)); // "avo"

    Text_InsertDrawString(&gHelpBoxSt.text[1], 0x00, TEXT_COLOR_47CF, GetStringFromIndex(0x4F4)); // "hit"
    Text_InsertDrawString(&gHelpBoxSt.text[1], 0x30, TEXT_COLOR_47CF, GetStringFromIndex(0x501)); // "crit"
    Text_InsertDrawString(&gHelpBoxSt.text[1], 0x60, TEXT_COLOR_47CF, GetStringFromIndex(0x51E)); // "dodge"
}

void DrawHelpBoxCombatArtBkselStats(void)
{
    int cid = GetCombatArtInForce(gActiveUnit);
    const struct CombatArtInfo * info = &gpCombatArtInfos[cid];

    Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[0], 0x20, TEXT_COLOR_456F, info->battle_status.atk);
    Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[0], 0x50, TEXT_COLOR_456F, info->cost);
    Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[0], 0x80, TEXT_COLOR_456F, info->battle_status.avo);

    Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[1], 0x20, TEXT_COLOR_456F, info->battle_status.hit);
    Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[1], 0x50, TEXT_COLOR_456F, info->battle_status.crit);
    Text_InsertDrawNumberOrBlank(&gHelpBoxSt.text[1], 0x80, TEXT_COLOR_456F, info->battle_status.dodge);
}
