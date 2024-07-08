#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "debuff.h"

static void CheckStatus(struct Unit * unit, ProcPtr proc)
{
    if (!UNIT_IS_VALID(unit))
        return;

    if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
        return;

#if defined(SID_Boon) && (COMMON_SKILL_VALID(SID_Boon))
    if (SkillTester(unit, SID_Boon))
    {
        switch (GetUnitStatusIndex(unit)) {
        case UNIT_STATUS_POISON:
        case UNIT_STATUS_SLEEP:
        case UNIT_STATUS_PETRIFY:
        case UNIT_STATUS_SILENCED:
        case UNIT_STATUS_BERSERK:
        case NEW_UNIT_STATUS_WEAKEN:
        case NEW_UNIT_STATUS_PANIC:
        case NEW_UNIT_STATUS_HEAVY_GRAVITY:
            SetUnitStatus(unit, UNIT_STATUS_NONE);
            MU_EndAll();
            StartStatusHealEffect(unit, proc);
            break;
        }
        return;
    }
#endif
}

void PrePhase_UnitStatusEffectSkills(ProcPtr proc)
{
    int uid;

    switch (gPlaySt.faction) {
    case FACTION_BLUE:
        for (uid = FACTION_BLUE + 1; uid < FACTION_BLUE + 1 + CONFIG_UNIT_AMT_ALLY; uid++)
            CheckStatus(GetUnit(uid), proc);

        break;

    case FACTION_GREEN:
        for (uid = FACTION_GREEN + 1; uid < FACTION_GREEN + 1 + CONFIG_UNIT_AMT_NPC; uid++)
            CheckStatus(GetUnit(uid), proc);

        break;

    case FACTION_RED:
        for (uid = FACTION_RED + 1; uid < FACTION_RED + 1 + CONFIG_UNIT_AMT_ENEMY; uid++)
            CheckStatus(GetUnit(uid), proc);
    
        break;
    }
}
