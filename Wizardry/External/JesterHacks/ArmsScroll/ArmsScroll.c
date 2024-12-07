#include "common-chax.h"
#include "skill-system.h"
#include "icon-rework.h"
#include "kernel-lib.h"
#include "constants/texts.h"



/* External hooks */
char * GetArmsScrollItemName(int item)
{
    return GetSkillNameStr(ITEM_USES(item));
}

int GetArmsScrollItemDescId(int item)
{
    return GetSkillDescMsg(ITEM_USES(item));
}

int GetArmsScrollItemUseDescId(int item)
{
    return MSG_ITEM_ArmsScroll_USEDESC;
}

int GetArmsScrollItemIconId(int item)
{
    return SKILL_ICON(ITEM_USES(item));
}

STATIC_DECLAR int GetHighestWeaponRank(struct Unit * unit) 
{
    int weaponLevel = 0;
    int weaponRank = -1;

    for (int i = 0; i < 7; i++)
    {
        if (unit->ranks[i] > weaponLevel && unit->ranks[i] < WPN_EXP_S)
        {
            weaponLevel = unit->ranks[i];
            weaponRank = i;
        }
    }

    return weaponRank;
}

void ItemUseEffect_ArmsScroll(struct Unit * unit)
{
    gActionData.unk08 = -1;
    int weaponRank = GetHighestWeaponRank(unit);

    if (weaponRank > 7)
        return;
    else 
        if (unit->ranks[weaponRank] >= WPN_EXP_A)
            unit->ranks[weaponRank] = WPN_EXP_S;
        else if (unit->ranks[weaponRank] >= WPN_EXP_B)
            unit->ranks[weaponRank] = WPN_EXP_A;
        else if (unit->ranks[weaponRank] >= WPN_EXP_C)
            unit->ranks[weaponRank] = WPN_EXP_B;
        else if (unit->ranks[weaponRank] >= WPN_EXP_D)
            unit->ranks[weaponRank] = WPN_EXP_C;
        else if (unit->ranks[weaponRank] >= WPN_EXP_E)
            unit->ranks[weaponRank] = WPN_EXP_D;
}

void ItemUseAction_ArmsScroll(ProcPtr proc)
{
    struct Unit * unit = GetUnit(gActionData.subjectIndex);
    int slot = gActionData.itemSlotIndex;
    UnitUpdateUsedItem(unit, slot);
    NewPopup_VerySimple(MSG_Weapon_Rank_Increased, 0x5A, proc);
}

bool ItemUsbility_ArmsScroll(struct Unit * unit, int item)
{
    if (GetHighestWeaponRank(unit) == -1)
        return false;
    else 
        return true;
}

/* Prep item use */
static void PrepItemUseScroll_OnDraw(struct ProcPrepItemUseJunaFruit * proc, int item, int x, int y)
{
    const char * str = GetStringFromIndex(MSG_Weapon_Rank_Increased);
    struct Text * text = &gPrepItemTexts[TEXT_PREPITEM_POPUP];
    int width = GetStringTextLen(str);

    ClearText(text);
    PutDrawText(
        text,
        TILEMAP_LOCATED(gBG2TilemapBuffer, x + 2, y),
        TEXT_COLOR_SYSTEM_WHITE,
        0, 0, str
    );

    BG_EnableSyncByMask(BG2_SYNC_BIT);

    proc->xpos = x * 8 - 4;
    proc->ypos = y * 8 - 4;
    proc->width = width / 8 + 3;
    proc->height = 2;
}

STATIC_DECLAR void PrepItemUseScroll_OnInit(struct ProcPrepItemUseJunaFruit * proc)
{
    struct ProcPrepItemUse * parent = proc->proc_parent;

    DrawPrepScreenItemUseStatBars(parent->unit, 0);
    DrawPrepScreenItemUseStatValues(parent->unit);

    PrepItemUseScroll_OnDraw(proc, parent->unit->items[parent->slot], 0x11, 0x0E);

    proc->timer = 0x78;
    PlaySoundEffect(0x5A);
}

STATIC_DECLAR void PrepItemUseScroll_OnEnd(struct ProcPrepItemUseJunaFruit * proc)
{
    struct ProcPrepItemUse * parent = proc->proc_parent;
    UnitUpdateUsedItem(parent->unit, parent->slot);
    PrepItemUseJuna_OnEnd(proc);
}

STATIC_DECLAR const struct ProcCmd ProcScr_PrepItemUseScroll[] = {
    PROC_SET_END_CB(PrepItemUseScroll_OnEnd),
    PROC_CALL(PrepItemUseScroll_OnInit),
    PROC_REPEAT(PrepItemUseJuna_IDLE),
    PROC_END
};

void PrepItemEffect_ArmsScroll(struct ProcPrepItemUse * proc, u16 item)
{
    Proc_StartBlocking(ProcScr_PrepItemUseScroll, proc);
}