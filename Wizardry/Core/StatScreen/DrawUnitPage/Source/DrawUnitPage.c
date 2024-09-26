#include "common-chax.h"
#include "stat-screen.h"
#include "kernel-lib.h"
#include "strmag.h"

LYN_REPLACE_CHECK(DisplayPage0);
void DisplayPage0(void)
{
	struct Unit *unit = gStatScreen.unit;

	u8 max_vals[] = {
		UNIT_POW_MAX(unit),
		GetUnitMaxMagic(unit),
		UNIT_SKL_MAX(unit),
		UNIT_SPD_MAX(unit),
		UNIT_LCK_MAX(unit),
		UNIT_DEF_MAX(unit),
		UNIT_RES_MAX(unit),
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
void StartUnitScreenHelp(int pageid, struct Proc *proc)
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
