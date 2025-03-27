#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "constants/skills.h"

static void _SetInitiativeStatDebuf(struct Unit *unit)
{
#if (defined(SID_InitStrong) && COMMON_SKILL_VALID(SID_InitStrong))
	if (SkillTester(unit, SID_InitStrong))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_POW);
#endif

#if (defined(SID_InitClever) && COMMON_SKILL_VALID(SID_InitClever))
	if (SkillTester(unit, SID_InitClever))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_MAG);
#endif

#if (defined(SID_InitDeft) && COMMON_SKILL_VALID(SID_InitDeft))
	if (SkillTester(unit, SID_InitDeft))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_SKL);
#endif

#if (defined(SID_InitQuick) && COMMON_SKILL_VALID(SID_InitQuick))
	if (SkillTester(unit, SID_InitQuick))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_SPD);
#endif

#if (defined(SID_InitLucky) && COMMON_SKILL_VALID(SID_InitLucky))
	if (SkillTester(unit, SID_InitLucky))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_LCK);
#endif

#if (defined(SID_InitSturdy) && COMMON_SKILL_VALID(SID_InitSturdy))
	if (SkillTester(unit, SID_InitSturdy))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_DEF);
#endif

#if (defined(SID_InitCalm) && COMMON_SKILL_VALID(SID_InitCalm))
	if (SkillTester(unit, SID_InitCalm))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_RES);
#endif

#if (defined(SID_InitNimble) && COMMON_SKILL_VALID(SID_InitNimble))
	if (SkillTester(unit, SID_InitNimble))
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_MOV);
#endif

#if (defined(SID_InitSpectrum) && COMMON_SKILL_VALID(SID_InitSpectrum))
	if (SkillTester(unit, SID_InitSpectrum)) {
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_POW);
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_MAG);
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_SKL);
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_SPD);
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_LCK);
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_DEF);
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_RES);
		SetUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_MOV);
	}
#endif
}

static void _ClearInitiativeStatDebuf(struct Unit *unit)
{
	ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_POW);
	ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_MAG);
	ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_SKL);
	ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_SPD);
	ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_LCK);
	ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_DEF);
	ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_RES);
	ClearUnitStatDebuff(unit, UNIT_STAT_BUFF_INIT_MOV);
}

bool ChapterInit_SetInitiativeStatus(ProcPtr proc)
{
	FOR_UNITS_ONMAP_ALL(unit, {
		_SetInitiativeStatDebuf(unit);
	})

	return false;
}

bool PrePhsae_TickInitativeSkillStatus(ProcPtr proc)
{
	if (gPlaySt.chapterTurnNumber >= 2 && gPlaySt.faction == FACTION_BLUE) {
		FOR_UNITS_ONMAP_ALL(unit, {
			_ClearInitiativeStatDebuf(unit);
		})
	}
	return false;
}
