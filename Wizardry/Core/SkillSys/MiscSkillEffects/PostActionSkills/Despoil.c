#include "common-chax.h"
#include "debuff.h"
#include "status-getter.h"
#include "action-expa.h"
#include "unit-expa.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

bool PostActionItemAcquisition(ProcPtr parent)
{
    FORCE_DECLARE struct Unit * unit = gActiveUnit;

    if (!UNIT_IS_VALID(unit) || UNIT_STONED(unit))
        return false;

    if (gActionData.unitActionType == UNIT_ACTION_COMBAT && gBattleActorGlobalFlag.enimy_defeated)
    {
#if defined(SID_Despoil) && (COMMON_SKILL_VALID(SID_Despoil))
        if (SkillTester(unit, SID_Despoil) && gBattleActorGlobalFlag.enimy_defeated)
        {
            NewPopup_ItemGot(parent, unit, ITEM_REDGEM);
            return true;
        }
#endif
    }
    return false;
}
