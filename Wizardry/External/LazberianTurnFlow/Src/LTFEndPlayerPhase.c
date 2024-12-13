#include "LTF.h"

// NOTE: REPLACES VANILLA FUNCTION
LYN_REPLACE_CHECK(CommandEffectEndPlayerPhase);
u8 CommandEffectEndPlayerPhase(struct MenuProc * menu, struct MenuItemProc * menuItem)
{
	unsigned phase = gPlaySt.faction; // Not hardcoding to PlayerPhase in case of enemy control

	for (unsigned index = phase + 1; index < phase + 0x40; ++index)
	{
		struct Unit* unit = GetUnit(index);

		if (UNIT_IS_VALID(unit) && (unit->statusIndex != UNIT_STATUS_BERSERK))
			unit->state |= (US_UNSELECTABLE | US_HAS_MOVED);
	}

	Proc_EndEach(gProcScr_PlayerPhase);

    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A | MENU_ACT_CLEAR;
}
