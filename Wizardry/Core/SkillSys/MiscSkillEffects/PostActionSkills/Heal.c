#include "common-chax.h"
#include "debuff.h"
#include "map-anims.h"
#include "battle-system.h"
#include "skill-system.h"
#include "constants/skills.h"

/* 
** Seem to have an odd Heisenbug softlock crash here
*/
bool PostAction_BattleActorHeal(ProcPtr parent)
{
    int heal = 0;

    int hp_cur = GetUnitCurrentHp(gActiveUnit);
    int hp_max = GetUnitMaxHp(gActiveUnit);

    FORCE_DECLARE int missingHP = hp_max - hp_cur;

    if (!UNIT_ALIVE(gActiveUnit) || UNIT_STONED(gActiveUnit))
        return false;

    switch (gActionData.unitActionType)
    {
    case UNIT_ACTION_COMBAT:
    
#if defined(SID_BoundlessVitality) && (COMMON_SKILL_VALID(SID_BoundlessVitality))
        if (SkillTester(gActiveUnit, SID_BoundlessVitality))
            heal += hp_max * SKILL_EFF0(SID_BoundlessVitality) / 100;
#endif

    }

#if defined(SID_Lifetaker) && (COMMON_SKILL_VALID(SID_Lifetaker))
    if (SkillTester(gActiveUnit, SID_Lifetaker) && gBattleActorGlobalFlag.enemy_defeated)
        heal += hp_max * SKILL_EFF0(SID_Lifetaker) / 100;
#endif

#if defined(SID_MysticBoost) && (COMMON_SKILL_VALID(SID_MysticBoost))
    if (SkillTester(gActiveUnit, SID_MysticBoost))
        heal += SKILL_EFF0(SID_MysticBoost);
#endif

#if defined(SID_DarkBargain) && (COMMON_SKILL_VALID(SID_DarkBargain))
    if (SkillTester(gActiveUnit, SID_DarkBargain))
    {
        if (gActiveUnit->exp >= missingHP)
        {
            gActiveUnit->exp -= missingHP;
            heal += missingHP;
        }
        else
        {
            heal += gActiveUnit->exp;
            gActiveUnit->exp = 0;
        }
    }
#endif

#if defined(SID_HoardersBane) && (COMMON_SKILL_VALID(SID_HoardersBane))
    if (SkillTester(gActiveUnit, SID_HoardersBane))
    {
        if (UNIT_CATTRIBUTES(gActiveUnit) & CA_SUPPLY || SkillTester(gActiveUnit, SID_HoardersBane))
        {
            if (gActiveUnit->curHP < gActiveUnit->maxHP)
            {
                if (GetConvoyItemSlot(ITEM_VULNERARY) != -1)
                {
                    u16 *convoy;
                    convoy = GetConvoyItemArray();
                    u16 usedItem = GetItemAfterUse(convoy[GetConvoyItemSlot(ITEM_VULNERARY)]);
                    heal += SKILL_EFF0(SID_HoardersBane);
                    RemoveItemFromConvoy(GetConvoyItemSlot(ITEM_VULNERARY));

                    if (ITEM_USES(usedItem) != 0)
                        AddItemToConvoy(usedItem);
                }
            }
        }
    }
#endif

#ifdef CONFIG_RESTORE_HP_ON_LEVEL_UP
    if (gEventSlots[EVT_SLOT_7] == 410) /* 'Heal' expressed as a hexidecimal and then convert back into decimal and summed */
        heal = gActiveUnit->maxHP - gActiveUnit->curHP;
#endif

    if (heal == 0)
        return false;

    if ((heal >= (hp_max - hp_cur)))
        heal = hp_max - hp_cur;

    CallMapAnim_Heal(parent, gActiveUnit, heal);
    return true;
}

bool PostAction_BattleTargetHeal(ProcPtr parent)
{
    int heal = 0;

    int hp_cur = GetUnitCurrentHp(GetUnit(gBattleTarget.unit.index));
    int hp_max = GetUnitMaxHp(GetUnit(gBattleTarget.unit.index));

    if (!UNIT_ALIVE(GetUnit(gBattleTarget.unit.index)) || UNIT_STONED(GetUnit(gBattleTarget.unit.index)))
        return false;

    switch (gActionData.unitActionType)
    {
    case UNIT_ACTION_COMBAT:
#if defined(SID_BoundlessVitality) && (COMMON_SKILL_VALID(SID_BoundlessVitality))
        if (SkillTester(GetUnit(gBattleTarget.unit.index), SID_BoundlessVitality))
            heal += hp_max * SKILL_EFF0(SID_BoundlessVitality) / 100;
#endif
    }

    if (heal == 0)
        return false;

    if ((heal >= (hp_max - hp_cur)))
        heal = hp_max - hp_cur;

    CallMapAnim_Heal(parent, GetUnit(gBattleTarget.unit.index), heal);
    return true;
}

