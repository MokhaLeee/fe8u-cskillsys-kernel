#include "common-chax.h"
#include "debuff.h"
#include "status-getter.h"
#include "action-expa.h"
#include "unit-expa.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

bool PostActionGetItem(ProcPtr parent)
{
    FORCE_DECLARE struct Unit * unit = gActiveUnit;

    if (!UNIT_IS_VALID(unit) || UNIT_STONED(unit))
        return false;

    if (gActionData.unitActionType == UNIT_ACTION_COMBAT && gBattleActorGlobalFlag.enemy_defeated)
    {

#if (defined(CONFIG_ITEM_INDEX_SKILL_STEALER))
        if (UNIT_CATTRIBUTES(&gBattleTarget.unit) & CA_BOSS)
            for (int i = 0; i < 5; i++) 
                if(GetItemIndex(unit->items[i]) == CONFIG_ITEM_INDEX_SKILL_STEALER)
                {
                    struct SkillList *list;
                    list = GetUnitSkillList(&gBattleTarget.unit);
                    NewPopup_ItemGot(parent, unit, (list->sid[0] << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL);
                }
#endif


#if defined(SID_Despoil) && (COMMON_SKILL_VALID(SID_Despoil))
        if (SkillTester(unit, SID_Despoil) && gBattleActorGlobalFlag.enemy_defeated)
        {
            NewPopup_ItemGot(parent, unit, ITEM_REDGEM);
            return true;
        }
#endif

#if defined(SID_GoldDigger) && (COMMON_SKILL_VALID(SID_GoldDigger))
        if (SkillTester(unit, SID_GoldDigger) && gBattleActorGlobalFlag.enemy_defeated)
        {
            NewPopup_GoldGot(parent, unit, SKILL_EFF0(SID_GoldDigger));
            return true;
        }
#endif

#if defined(SID_MakeAKilling) && (COMMON_SKILL_VALID(SID_MakeAKilling))
        if (SkillTester(unit, SID_MakeAKilling) && gBattleActorGlobalFlag.enemy_defeated)
        {
            if (Roll1RN(unit->lck))
            {
                NewPopup_GoldGot(parent, unit, SKILL_EFF0(SID_MakeAKilling) * 100);
                return true;
            }
        }
#endif
    }   
    return false;
}
