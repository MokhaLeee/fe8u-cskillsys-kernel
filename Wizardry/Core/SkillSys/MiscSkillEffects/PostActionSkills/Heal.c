#include "common-chax.h"
#include "debuff.h"
#include "map-anims.h"
#include "battle-system.h"
#include "skill-system.h"
#include "constants/skills.h"

bool PostAction_BattleActorHeal(ProcPtr parent)
{
    int heal = 0;

    int hp_cur = GetUnitCurrentHp(gActiveUnit);
    int hp_max = GetUnitMaxHp(gActiveUnit);

    int missingHP = hp_max - hp_cur;

    if (!UNIT_ALIVE(gActiveUnit) || UNIT_STONED(gActiveUnit))
        return false;

#if defined(SID_Lifetaker) && (COMMON_SKILL_VALID(SID_Lifetaker))
    if (SkillTester(gActiveUnit, SID_Lifetaker) && gBattleActorGlobalFlag.enimy_defeated)
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

    if (heal == 0)
        return false;

    if ((heal >= (hp_max - hp_cur)))
        heal = hp_max - hp_cur;

    CallMapAnim_Heal(parent, gActiveUnit, heal);
    return true;
}
