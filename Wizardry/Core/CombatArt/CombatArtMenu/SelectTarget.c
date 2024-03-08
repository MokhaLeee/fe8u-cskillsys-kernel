#include "global.h"
#include "uimenu.h"
#include "uiselecttarget.h"
#include "bmtarget.h"
#include "bksel.h"
#include "proc.h"
#include "bm.h"
#include "event.h"
#include "icon.h"
#include "face.h"
#include "bmmenu.h"
#include "fontgrp.h"
#include "hardware.h"
#include "menuitempanel.h"
#include "constants/classes.h"

#include "common-chax.h"
#include "combat-art.h"

STATIC_DECLAR u8 CombatSelectTargetOnCancel(ProcPtr proc, struct SelectTarget * target);
STATIC_DECLAR const struct ProcCmd ProcScr_PostCombatArtSelectTarget[];

const struct SelectInfo gSelectInfoCombatArt = {
    .onInit = (void *)NewBattleForecast,
    .onEnd = (void *)AttackMapSelect_End,
    .onSwitchIn = AttackMapSelect_SwitchIn,
    .onSelect = AttackMapSelect_Select,
    .onCancel = CombatSelectTargetOnCancel,
    .onHelp = (void *)StartBattleForecastHelpBox,
};

STATIC_DECLAR u8 CombatSelectTargetOnCancel(ProcPtr proc, struct SelectTarget * target)
{
    if (EventEngineExists() == 1)
        return 0;

    Proc_Start(ProcScr_PostCombatArtSelectTarget, PROC_TREE_3);
    return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6B;
}

STATIC_DECLAR void RebuildCombatArtWeaponSelMenu(void);

STATIC_DECLAR const struct ProcCmd ProcScr_PostCombatArtSelectTarget[] = {
    PROC_NAME("PostCombatArtSelectTarget"),
    PROC_CALL(LockGame),
    PROC_CALL(sub_8022E38),
    PROC_WHILE_EXISTS(gProcScr_BKSEL),
    PROC_WHILE_EXISTS(gProcScr_CamMove),
    PROC_CALL(RebuildCombatArtWeaponSelMenu),
    PROC_CALL(UnlockGame),
    PROC_END
};

STATIC_DECLAR void RebuildCombatArtWeaponSelMenu(void)
{
    struct MenuProc * menu;

    ResetTextFont();
    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

    /* The same as CombatArtActionCommandEffect() */
    ResetIconGraphics();
    LoadIconPalettes(4);

    menu = StartOrphanMenu(&gCombatArtItemMenuDef);

    if (gActiveUnit->pClassData->number != CLASS_PHANTOM)
    {
        StartFace(0, GetUnitPortraitId(gActiveUnit), 0xB0, 0xC, 2);
        SetFaceBlinkControlById(0, 5);
    }

    ForceMenuItemPanel(menu, gActiveUnit, 0xF, 0xB);
}
