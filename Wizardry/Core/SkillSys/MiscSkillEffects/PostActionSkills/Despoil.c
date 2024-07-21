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
    u16 item_index;
    FORCE_DECLARE struct Unit * unit = gActiveUnit;

    if (!UNIT_IS_VALID(unit) || UNIT_STONED(unit))
        return false;

    switch (gActionData.unitActionType) {
    case UNIT_ACTION_COMBAT:
#if defined(SID_Despoil) && (COMMON_SKILL_VALID(SID_Despoil))
        if (SkillTester(unit, SID_Despoil) && gBattleActorGlobalFlag.skill_activated_despoil)
        {
            item_index = ITEM_REDGEM;
            goto L_exec_produce_item;
        }
#endif

    /* fall through */

    default:
        break;
    }

    return false;

L_exec_produce_item:
    NewPopup_ItemGot(parent, unit, item_index);
    return true;
}
