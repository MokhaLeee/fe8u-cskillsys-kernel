#include "global.h"
#include "proc.h"
#include "uiselecttarget.h"
#include "bmunit.h"

#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "strmag.h"

void PrePhaseFunc_SetWeatherType(ProcPtr proc)
{
    int uid;

    /* Flags to keep track of which weathers have been triggered.
     * Airlock not provided as it has the highest priority.
     * SandStream not provided as it has the lowest priority.  
     */
    bool Weather_Sunny_Set = false;
    bool Weather_Rain_Set = false;
    bool Weather_Snow_Set = false;

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
        gPlaySt.chapterWeatherId = WEATHER_CLOUDS;
        break;  // Exit the loop once this weather has been set, as it has the highest priority
    }
#endif

#if defined(SID_Drought) && (COMMON_SKILL_VALID(SID_Drought))
    if (SkillTester(unit, SID_Drought))
    {
        gPlaySt.chapterWeatherId = WEATHER_FLAMES;
        Weather_Sunny_Set = true;
    }
#endif

#if defined(SID_Drizzle) && (COMMON_SKILL_VALID(SID_Drizzle))
    if (SkillTester(unit, SID_Drizzle) && !Weather_Sunny_Set)
    {
        gPlaySt.chapterWeatherId = WEATHER_RAIN;
        Weather_Rain_Set = true;
    }
#endif

#if defined(SID_SnowWarning) && (COMMON_SKILL_VALID(SID_SnowWarning))
    if (SkillTester(unit, SID_SnowWarning) && !Weather_Rain_Set)
    {
        gPlaySt.chapterWeatherId = WEATHER_SNOWSTORM;
        Weather_Snow_Set = true;
    }
#endif

#if defined(SID_SandStream) && (COMMON_SKILL_VALID(SID_SandStream))
    if (SkillTester(unit, SID_SandStream) && !Weather_Snow_Set)
    {
        gPlaySt.chapterWeatherId = WEATHER_SANDSTORM;
    }
#endif
    }

    return;
}