#include "common-chax.h"
#include "kernel-lib.h"
#include "debuff.h"
#include "skill-system.h"
#include "kernel-lib.h"
#include "class-types.h"
#include "constants/skills.h"

bool PrePhaseFunc_HoneFortify(ProcPtr proc)
{
	int i;

	FOR_UNITS_ONMAP_FACTION(gPlaySt.faction, unit, {
		bool PowHone_eff = false;
		bool MagHone_eff = false;
		bool SklHone_eff = false;
		bool SpdHone_eff = false;
		bool LckHone_eff = false;
		bool DefHone_eff = false;
		bool ResHone_eff = false;
		bool MovHone_eff = false;
		bool JobHone_eff = false;
		bool JobFortify_eff = false;

		bool Hone_eff = false;

		if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
			continue;

#if defined(SID_PowHone) && (COMMON_SKILL_VALID(SID_PowHone))
		if (SkillTester(unit, SID_PowHone)) {
			PowHone_eff = true;
			Hone_eff = true;
		}
#endif

#if defined(SID_MagHone) && (COMMON_SKILL_VALID(SID_MagHone))
		if (SkillTester(unit, SID_MagHone)) {
			MagHone_eff = true;
			Hone_eff = true;
		}
#endif

#if defined(SID_SklHone) && (COMMON_SKILL_VALID(SID_SklHone))
		if (SkillTester(unit, SID_SklHone)) {
			SklHone_eff = true;
			Hone_eff = true;
		}
#endif

#if defined(SID_SpdHone) && (COMMON_SKILL_VALID(SID_SpdHone))
		if (SkillTester(unit, SID_SpdHone)) {
			SpdHone_eff = true;
			Hone_eff = true;
		}
#endif

#if defined(SID_LckHone) && (COMMON_SKILL_VALID(SID_LckHone))
		if (SkillTester(unit, SID_LckHone)) {
			LckHone_eff = true;
			Hone_eff = true;
		}
#endif

#if defined(SID_DefHone) && (COMMON_SKILL_VALID(SID_DefHone))
		if (SkillTester(unit, SID_DefHone)) {
			DefHone_eff = true;
			Hone_eff = true;
		}
#endif

#if defined(SID_ResHone) && (COMMON_SKILL_VALID(SID_ResHone))
		if (SkillTester(unit, SID_ResHone)) {
			ResHone_eff = true;
			Hone_eff = true;
		}
#endif

#if defined(SID_MovHone) && (COMMON_SKILL_VALID(SID_MovHone))
		if (SkillTester(unit, SID_MovHone)) {
			MovHone_eff = true;
			Hone_eff = true;
		}
#endif

#if defined(SID_HoneCavalry) && (COMMON_SKILL_VALID(SID_HoneCavalry))
		if (CheckClassCavalry(UNIT_CLASS_ID(unit))) {
			if (SkillTester(unit, SID_HoneCavalry)) {
				JobHone_eff = true;
				Hone_eff = true;
			}
		}
#endif

#if defined(SID_HoneFlier) && (COMMON_SKILL_VALID(SID_HoneFlier))
		if (CheckClassFlier(UNIT_CLASS_ID(unit))) {
			if (SkillTester(unit, SID_HoneFlier)) {
				JobHone_eff = true;
				Hone_eff = true;
			}
		}
#endif

#if defined(SID_HoneArmor) && (COMMON_SKILL_VALID(SID_HoneArmor))
		if (CheckClassArmor(UNIT_CLASS_ID(unit))) {
			if (SkillTester(unit, SID_HoneArmor)) {
				JobHone_eff = true;
				Hone_eff = true;
			}
		}
#endif

#if defined(SID_FortifyArmor) && (COMMON_SKILL_VALID(SID_FortifyArmor))
		if (CheckClassArmor(UNIT_CLASS_ID(unit))) {
			if (SkillTester(unit, SID_FortifyArmor)) {
				JobFortify_eff = true;
				Hone_eff = true;
			}
		}
#endif

		if (Hone_eff) {
			for (i = 0; i < ARRAY_COUNT_RANGE2x2; i++) {
				int x = unit->xPos + gVecs_2x2[i].x;
				int y = unit->yPos + gVecs_2x2[i].y;
				struct Unit *tunit = GetUnitAtPosition(x, y);

				if (!tunit)
					continue;

				if (tunit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
					continue;

				if (AreUnitsAllied(unit->index, tunit->index)) {
					if (PowHone_eff)
						SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_POW);

					if (MagHone_eff)
						SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_MAG);

					if (SklHone_eff)
						SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_SKL);

					if (SpdHone_eff)
						SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_SPD);

					if (LckHone_eff)
						SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_LCK);

					if (DefHone_eff)
						SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_DEF);

					if (ResHone_eff)
						SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_RES);

					if (MovHone_eff)
						SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_MOV);

					if (JobHone_eff) {
						if (CheckSameClassType(UNIT_CLASS_ID(unit), UNIT_CLASS_ID(tunit)))
							SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_JOB_HONE);
					}

					if (JobFortify_eff) {
						if (CheckSameClassType(UNIT_CLASS_ID(unit), UNIT_CLASS_ID(tunit)))
							SetUnitStatDebuff(tunit, UNIT_STAT_BUFF_JOB_FORTIFY);
					}
				}
			}
		}
	})
	return false;
}
