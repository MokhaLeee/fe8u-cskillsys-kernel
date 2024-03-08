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

/* Decomp port */
void StartStatusHealEffect(struct Unit * unit, ProcPtr proc);

bool PostActionGaleForce(ProcPtr parent)
{
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
    return false;
}
