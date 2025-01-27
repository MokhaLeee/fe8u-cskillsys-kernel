#include "common-chax.h"
#include "kernel-lib.h"
#include "map-anims.h"
#include "skill-system.h"
#include "event-rework.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "unit-expa.h"
#include "action-expa.h"
#include "jester_headers/class-pairs.h"

#if (defined(SID_Doppleganger) && (COMMON_SKILL_VALID(SID_Doppleganger)))

// Predefine an array of key-value pairs
const int dopplegangerPairs[1][2] = { { CHARACTER_EIRIKA, CLASS_EIRIKA_LORD } };
// Define the size of the array
const int dopplegangerListSize = sizeof(dopplegangerPairs) / sizeof(dopplegangerPairs[0]);

#endif

u8 Doppleganger_Usability(const struct MenuItemDef * def, int number)
{
    if (gActiveUnit->state & US_CANTOING)
        return MENU_NOTSHOWN;

    if (!HasSelectTarget(gActiveUnit, MakeTargetListForBarrier))
        return MENU_DISABLED;

    return MENU_ENABLED;
}

static u8 Doppleganger_OnSelectTarget(ProcPtr proc, struct SelectTarget * target)
{
    gActionData.targetIndex = target->uid;

    gActionData.xOther = target->x;
    gActionData.yOther = target->y;

    HideMoveRangeGraphics();

    BG_Fill(gBG2TilemapBuffer, 0);
    BG_EnableSyncByMask(BG2_SYNC_BIT);

#if (defined(SID_Doppleganger) && (COMMON_SKILL_VALID(SID_Doppleganger)))
    gActionData.unk08 = SID_Doppleganger;
#endif
    gActionData.unitActionType = CONFIG_UNIT_ACTION_EXPA_ExecSkill;

    return TARGETSELECTION_ACTION_ENDFAST | TARGETSELECTION_ACTION_END | TARGETSELECTION_ACTION_SE_6A |
        TARGETSELECTION_ACTION_CLEARBGS;
}

u8 Doppleganger_OnSelected(struct MenuProc * menu, struct MenuItemProc * item)
{
    if (item->availability == MENU_DISABLED)
    {
        MenuFrozenHelpBox(menu, MSG_MenuSkill_Doppleganger_FRtext);
        return MENU_ACT_SND6B;
    }

    ClearBg0Bg1();

    MakeTargetListForBarrier(gActiveUnit);
    BmMapFill(gBmMapMovement, -1);

    StartSubtitleHelp(
        NewTargetSelection_Specialized(&gSelectInfo_PutTrap, Doppleganger_OnSelectTarget),
        GetStringFromIndex(MSG_MenuSkill_Common_Target));

    PlaySoundEffect(0x6A);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

static void callback_anim(ProcPtr proc)
{
    PlaySoundEffect(0x269);
    Proc_StartBlocking(ProcScr_DanceringAnim, proc);

    BG_SetPosition(BG_0, -SCREEN_TILE_IX(gActiveUnit->xPos - 1), -SCREEN_TILE_IX(gActiveUnit->yPos - 2));
}

static void callback_exec(ProcPtr proc)
{
#if (defined(SID_Doppleganger) && (COMMON_SKILL_VALID(SID_Doppleganger)))
    for (int i = 0; i < dopplegangerListSize; i++)
        if (gActiveUnit->pCharacterData->number == dopplegangerPairs[i][0])
        {
            gActiveUnit->pClassData = GetUnit(gActionData.targetIndex)->pClassData;
            gActionDataExpa.refrain_action = true;
            EndAllMus();
        }
#endif
}

bool Action_Doppleganger(ProcPtr parent)
{
    NewMuSkillAnimOnActiveUnit(gActionData.unk08, callback_anim, callback_exec);
    return true;
}