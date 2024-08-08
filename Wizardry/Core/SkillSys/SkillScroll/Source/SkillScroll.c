#include "common-chax.h"
#include "skill-system.h"
#include "icon-rework.h"
#include "constants/texts.h"

/* External hooks */
char * GetSkillScrollItemName(int item)
{
    return GetSkillNameStr(ITEM_USES(item));
}

int GetSkillScrollItemDescId(int item)
{
    return GetSkillDescMsg(ITEM_USES(item));
}

int GetSkillScrollItemUseDescId(int item)
{
    // return GetSkillDescMsg(ITEM_USES(item));
    return MSG_ITEM_SkillScrollUseDesc;
}

int GetSkillScrollItemIconId(int item)
{
    return SKILL_ICON(ITEM_USES(item));
}

/* Item use */
void ItemUseAction_SkillScroll(ProcPtr proc)
{
    struct Unit * unit = GetUnit(gActionData.subjectIndex);
    int slot = gActionData.itemSlotIndex;
    int item = unit->items[gActionData.itemSlotIndex];

    AddSkill(unit, ITEM_USES(item));
    UnitUpdateUsedItem(unit, slot);

    NewPopup_VerySimple(MSG_SkillLearned, 0x5A, proc);
}

/* Prep item use */
static void PrepItemUseScroll_OnDraw(struct ProcPrepItemUseJunaFruit * proc, int item, int x, int y)
{
    int skill = ITEM_USES(item);
    const char * str = GetStringFromIndex(MSG_SkillLearned);
    struct Text * text = &gPrepItemTexts[TEXT_PREPITEM_POPUP];
    int icon = SKILL_ICON(skill);
    int width = GetStringTextLen(str);

    DrawIcon(TILEMAP_LOCATED(gBG2TilemapBuffer, x, y), icon, 0x4000);

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
    AddSkill(parent->unit, ITEM_USES(parent->unit->items[parent->slot]));
    UnitUpdateUsedItem(parent->unit, parent->slot);
    PrepItemUseJuna_OnEnd(proc);
}

STATIC_DECLAR const struct ProcCmd ProcScr_PrepItemUseScroll[] = {
    PROC_SET_END_CB(PrepItemUseScroll_OnEnd),
    PROC_CALL(PrepItemUseScroll_OnInit),
    PROC_REPEAT(PrepItemUseJuna_IDLE),
    PROC_END
};

void PrepItemEffect_SkillScroll(struct ProcPrepItemUse * proc, u16 item)
{
    Proc_StartBlocking(ProcScr_PrepItemUseScroll, proc);
}

bool ItemUsbility_SkillScroll(struct Unit * unit, int item)
{
    return !IsSkillLearned(unit, ITEM_USES(item));
}
