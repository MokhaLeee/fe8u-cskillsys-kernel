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
};

bool PrePhaseFunc_SetWeatherType(ProcPtr proc)
{
    int uid;
    int priv_level = WEATHER_PRIORITY_0;
    int weather = gPlaySt.chapterWeatherId;

    if (gPlaySt.chapterTurnNumber >= 2)
        return false;

    for (uid = gPlaySt.faction + 1; uid < gPlaySt.faction + 0x40; uid++)
    {
        struct Unit * unit = GetUnit(uid);
        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & (US_HIDDEN | US_DEAD | US_RESCUED | US_BIT16))
            continue;

#if defined(SID_Airlock) && (COMMON_SKILL_VALID(SID_Airlock))
        if (SkillTester(unit, SID_Airlock))
        {
            /* Highest priority */
            SetWeather(WEATHER_FINE);
            return true;
        }
#endif

#if defined(SID_Drought) && (COMMON_SKILL_VALID(SID_Drought))
        if (SkillTester(unit, SID_Drought))
        {
            weather = WEATHER_FLAMES;
            priv_level = WEATHER_PRIORITY_FLAMES;
        }
#endif

#if defined(SID_Drizzle) && (COMMON_SKILL_VALID(SID_Drizzle))
        if (SkillTester(unit, SID_Drizzle) && priv_level < WEATHER_PRIORITY_RAIN)
        {
            weather = WEATHER_RAIN;
            priv_level = WEATHER_PRIORITY_RAIN;
        }
#endif

#if defined(SID_SnowWarning) && (COMMON_SKILL_VALID(SID_SnowWarning))
        if (SkillTester(unit, SID_SnowWarning) && priv_level < WEATHER_PRIORITY_SNOW)
        {
            weather = WEATHER_SNOWSTORM;
            priv_level = WEATHER_PRIORITY_SNOW;
        }
#endif

#if defined(SID_SandStream) && (COMMON_SKILL_VALID(SID_SandStream))
        if (SkillTester(unit, SID_SandStream) && priv_level < WEATHER_PRIORITY_SAND)
        {
            weather = WEATHER_SANDSTORM;
            priv_level = WEATHER_PRIORITY_SAND;
        }
#endif
    }

    if (weather != gPlaySt.chapterWeatherId)
    {
        SetWeather(weather);
        return true;
    }
    return false;
}
