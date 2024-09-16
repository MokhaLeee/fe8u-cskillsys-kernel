#include "common-chax.h"
#include "stat-screen.h"
#include "kernel-lib.h"
#include "strmag.h"
#include "skill-system.h"
#include "constants/skills.h"

LYN_REPLACE_CHECK(DisplayPage0);
void DisplayPage0(void)
{
    struct Unit * unit = gStatScreen.unit;

    int limitBreaker = 0;

#if defined(SID_LimitBreaker) && (COMMON_SKILL_VALID(SID_LimitBreaker))
    if (SkillTester(unit, SID_LimitBreaker))
        limitBreaker = SKILL_EFF0(SID_LimitBreaker);
#endif

    u8 max_vals[] = {
        UNIT_POW_MAX(unit) + limitBreaker,
        GetUnitMaxMagic(unit) + limitBreaker,
        UNIT_SKL_MAX(unit) + limitBreaker,
        UNIT_SPD_MAX(unit) + limitBreaker,
        UNIT_LCK_MAX(unit) + limitBreaker,
        UNIT_DEF_MAX(unit) + limitBreaker,
        UNIT_RES_MAX(unit) + limitBreaker,
    };

    gStatScreenStExpa.unitpage_max = SortMax(max_vals, ARRAY_COUNT(max_vals));
    gStatScreenStExpa.talkee = GetTalkee(gStatScreen.unit);

    InstallExpandedTextPal();

    switch (gpKernelDesigerConfig->unit_page_style) {
    case CONFIG_PAGE1_WITH_BWL:
    default:
        DisplayPage_WithBWL();
        break;

    case CONFIG_PAGE1_WITH_LEADERSHIP:
        DisplayPage_WithLeadership();
        break;
    }
}

/* External hook */
void StartUnitScreenHelp(int pageid, struct Proc * proc)
{
    switch (gpKernelDesigerConfig->unit_page_style) {
    case CONFIG_PAGE1_WITH_BWL:
    default:
        gStatScreen.help = RTextPageUnit_WithBWL;
        break;

    case CONFIG_PAGE1_WITH_LEADERSHIP:
        gStatScreen.help = RTextPageUnit_WithLeadership;
        break;
    }
}
