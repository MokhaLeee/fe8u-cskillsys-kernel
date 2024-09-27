#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

/**
 * It seems still hold some problem to put such hook
 * in pre-pahse (PROC_START_CHILD_BLOCKING(gProcScr_GorgonEggHatchDisplay) in BMAPMAIN).
 * But since no problems for corner cases were found in the current test, let's leave it this way for now
 */

enum weather_skills_priority {
	WEATHER_PRIORITY_0,
	WEATHER_PRIORITY_SAND,
	WEATHER_PRIORITY_SNOW,
	WEATHER_PRIORITY_RAIN,
	WEATHER_PRIORITY_FLAMES,
	WEATHER_PRIORITY_AIR_LOCK,
};

static void _check_weather_skill(struct Unit *unit, int *priv, int *weather)
{
	if (!UNIT_IS_VALID(unit))
		return;

	if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
		return;

#if defined(SID_Airlock) && (COMMON_SKILL_VALID(SID_Airlock))
	if (SkillTester(unit, SID_Airlock)) {
		/* Highest priority */
		*weather = WEATHER_FINE;
		*priv = WEATHER_PRIORITY_AIR_LOCK;
		return;
	}
#endif

#if defined(SID_Drought) && (COMMON_SKILL_VALID(SID_Drought))
	if (SkillTester(unit, SID_Drought) && *priv < WEATHER_PRIORITY_FLAMES) {
		*weather = WEATHER_FLAMES;
		*priv = WEATHER_PRIORITY_FLAMES;
	}
#endif

#if defined(SID_Drizzle) && (COMMON_SKILL_VALID(SID_Drizzle))
	if (SkillTester(unit, SID_Drizzle) && *priv < WEATHER_PRIORITY_RAIN) {
		*weather = WEATHER_RAIN;
		*priv = WEATHER_PRIORITY_RAIN;
	}
#endif

#if defined(SID_SnowWarning) && (COMMON_SKILL_VALID(SID_SnowWarning))
	if (SkillTester(unit, SID_SnowWarning) && *priv < WEATHER_PRIORITY_SNOW) {
		*weather = WEATHER_SNOWSTORM;
		*priv = WEATHER_PRIORITY_SNOW;
	}
#endif

#if defined(SID_SandStream) && (COMMON_SKILL_VALID(SID_SandStream))
	if (SkillTester(unit, SID_SandStream) && *priv < WEATHER_PRIORITY_SAND) {
		*weather = WEATHER_SANDSTORM;
		*priv = WEATHER_PRIORITY_SAND;
	}
#endif
}

bool PrePhase_ControlWeatherSkill(ProcPtr proc)
{
	int uid;
	int priv_level = WEATHER_PRIORITY_0;
	int weather = gPlaySt.chapterWeatherId;

	switch (gPlaySt.faction) {
	case FACTION_BLUE:
		for (uid = FACTION_BLUE + 1; uid < FACTION_BLUE + 1 + CONFIG_UNIT_AMT_ALLY; uid++)
			_check_weather_skill(GetUnit(uid), &weather, &priv_level);

		break;

	case FACTION_GREEN:
		for (uid = FACTION_GREEN + 1; uid < FACTION_GREEN + 1 + CONFIG_UNIT_AMT_NPC; uid++)
			_check_weather_skill(GetUnit(uid), &weather, &priv_level);

		break;

	case FACTION_RED:
		for (uid = FACTION_RED + 1; uid < FACTION_RED + 1 + CONFIG_UNIT_AMT_ENEMY; uid++)
			_check_weather_skill(GetUnit(uid), &weather, &priv_level);

		break;
	}

	if (weather != gPlaySt.chapterWeatherId) {
		SetWeather(weather);
		return true;
	}
	return false;
}
