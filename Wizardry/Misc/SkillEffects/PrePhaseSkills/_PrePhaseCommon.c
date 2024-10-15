#include "common-chax.h"
#include "skill-system.h"
#include "class-types.h"
#include "kernel-lib.h"
#include "constants/skills.h"

#include "pre-phase.h"

void SetupAversaNight(struct Unit *unit);
void ExecAversaNight(struct Unit *unit);
void SetupChillActors(struct Unit *unit);
void SetupChillTargets(struct Unit *unit);

static void PrePhaseSkillCommon_FactionLoop(void)
{
	/**
	 * Round 1
	 */
	FOR_UNITS_FACTION(gPlaySt.faction, unit, {
		/* ArmorMarch */
		ExecArmorMarch(unit);

		/* AversaNight */
		SetupAversaNight(unit);

		/* Chill */
		SetupChillActors(unit);
	})
}

static void PrePhaseSkillCommon_AllUnitLoop(void)
{
	/**
	 * Round 2
	 */
	FOR_UNITS_ALL(unit, {
		/* AversaNight */
		ExecAversaNight(unit);

		/* Chill */
		SetupChillTargets(unit);
	})
}

bool PrePhaseSkillCommon(ProcPtr proc)
{
	Assert(sizeof(gPrePhaseSkillBuf) < 0x30);

	memset(&gPrePhaseSkillBuf, 0, sizeof(gPrePhaseSkillBuf));

	PrePhaseSkillCommon_FactionLoop();
	PrePhaseSkillCommon_AllUnitLoop();

	return false;
}
