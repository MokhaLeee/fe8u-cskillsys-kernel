#include "global.h"
#include "bmunit.h"
#include "mu.h"
#include "proc.h"
#include "bmmind.h"

#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "battle-system.h"
#include "constants/skills.h"

extern u8 gPostActionGaleforceFlag;

bool PostActionGaleForce(ProcPtr parent)
{
#if defined(SID_Galeforce) && (SID_Galeforce < MAX_SKILL_NUM)
    struct Unit * unit = gActiveUnit;

    gPostActionGaleforceFlag = false;

    switch (gActionData.unitActionType) {
    case UNIT_ACTION_COMBAT:
        if (SkillTester(unit, SID_Galeforce) && gBattleActorGlobalFlag.skill_activated_galeforce)
        {
            gPostActionGaleforceFlag = true;
            MU_EndAll();
            StartStatusHealEffect(unit, parent);
            return true;
        }
        break;
    }
#endif /* defined(SID_Galeforce) */
    return false;
}
