#include "global.h"
#include "bmunit.h"
#include "bmbattle.h"
#include "bmitem.h"
#include "constants/items.h"

#include "common-chax.h"
#include "battle-system.h"
#include "skill-system.h"
#include "combat-art.h"
#include "constants/skills.h"

STATIC_DECLAR bool CheckUnitNullEffective(struct Unit * unit)
{
    int i;
    u32 attributes = 0;

    /* Check item */
    for (i = 0; i < UNIT_ITEM_COUNT; ++i)
        attributes |= GetItemAttributes(unit->items[i]);

    if (attributes & IA_NEGATE_FLYING)
        return true;

    /* Check unit */
    if (SkillTester(unit, SID_Nullify))
        return true;

    return false;
}

/* LynJump */
bool IsItemEffectiveAgainst(u16 item, struct Unit * unit)
{
    int i, jid;
    const u8 * list;

    if (!unit->pClassData)
        return false;

    jid = UNIT_CLASS_ID(unit);
    list = GetItemEffectiveness(item);

    if (!list)
        return false;

    for (i = 0; list[i]; i++)
        if (list[i] == jid)
            goto check_null_effective;

    return false;

check_null_effective:
    if (CheckUnitNullEffective(unit))
        return false;

    return true;
}

/* LynJump */
bool IsUnitEffectiveAgainst(struct Unit * actor, struct Unit * target)
{
    const u8 * list = NULL;
    int i, jid = UNIT_CLASS_ID(target);

    /* Check combat-art */
    if (actor->index == gBattleActor.unit.index)
    {
        int cid = GetCombatArtInForce(actor);
        if (COMBART_VALID(cid))
        {
            const struct CombatArtInfo * info = &gpCombatArtInfos[cid];
            if (info->effective_all)
                return true;

            if (info->effective_dragon)
                list = GetItemEffectiveness(ITEM_ELIXIR);
            else if (info->effective_monster)
                list = GetItemEffectiveness(ITEM_LIGHT_IVALDI);
            else if (info->effective_armor)
                list = GetItemEffectiveness(ITEM_AXE_HAMMER);
            else if (info->effective_fly)
                list = GetItemEffectiveness(ITEM_BOW_IRON);
            else if (info->effective_ride)
                list = GetItemEffectiveness(ITEM_SWORD_ZANBATO);
        }
    }

    /* Check skills */
    if (SkillTester(actor, SID_Slayer))
        list = GetItemEffectiveness(ITEM_LIGHT_IVALDI);

    if (SkillTester(actor, SID_Skybreaker))
        list = GetItemEffectiveness(ITEM_BOW_IRON);

    if (!list)
        return false;

    for (i = 0; list[i]; i++)
        if (list[i] == jid)
            goto check_null_effective;

    return false;

check_null_effective:
    if (CheckUnitNullEffective(target))
        return false;

    return true;
}
