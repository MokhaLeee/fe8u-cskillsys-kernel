#include "common-chax.h"
#include "battle-system.h"
#include "skill-system.h"
#include "combat-art.h"
#include "class-types.h"
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

#if (defined(SID_Nullify) && (SID_Nullify < MAX_SKILL_NUM))
    /* Check unit */
    if (SkillTester(unit, SID_Nullify))
        return true;
#endif

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
    int jid_target = UNIT_CLASS_ID(target);

    /* Check combat-art */
    if (actor->index == gBattleActor.unit.index)
    {
        int cid = GetCombatArtInForce(actor);
        if (COMBART_VALID(cid))
        {
            const struct CombatArtInfo * info = &gpCombatArtInfos[cid];
            if (info->effective_all)
                goto check_null_effective;

            if (info->effective_dragon)
            {
                if (CheckClassDragon(jid_target))
                    goto check_null_effective;
            }
            else if (info->effective_monster)
            {
                if (CheckClassBeast(jid_target))
                    goto check_null_effective;
            }
            else if (info->effective_armor)
            {
                if (CheckClassArmor(jid_target))
                    goto check_null_effective;
            }
            else if (info->effective_fly)
            {
                if (CheckClassFlier(jid_target))
                    goto check_null_effective;
            }
            else if (info->effective_ride)
            {
                if (CheckClassCavalry(jid_target))
                    goto check_null_effective;
            }
        }
    }

    /* Check skills */
#if (defined(SID_Slayer) && (SID_Slayer < MAX_SKILL_NUM))
    if (SkillTester(actor, SID_Slayer))
    {
        if (CheckClassBeast(jid_target))
            goto check_null_effective;
    }
#endif

#if (defined(SID_Skybreaker) && (SID_Skybreaker < MAX_SKILL_NUM))
    if (SkillTester(actor, SID_Skybreaker))
    {
        if (CheckClassFlier(jid_target))
            goto check_null_effective;
    }
#endif

    return false;

check_null_effective:
    if (CheckUnitNullEffective(target))
        return false;

    return true;
}
