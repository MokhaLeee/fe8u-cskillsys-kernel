#include "common-chax.h"
#include "kernel-lib.h"

extern struct DefeatTalkEnt * pr_gDefeatTalkList;

LYN_REPLACE_CHECK(GetUnitLeaderCharId);
int GetUnitLeaderCharId(struct Unit *unit)
{
	int i;
	struct DefeatTalkEnt *it;

	if (!(unit->index & 0xC0))
		return 0;

	/**
	 * Step1: try to find the BOSS of the chapter
	 */
	for (it = gDefeatTalkList; it->pid != 0xFFFF; it++) {
		if ((it->route != 0xFF) && (it->route != gPlaySt.chapterModeIndex))
			continue;

		if ((it->chapter != 0xFF) && (it->chapter != gPlaySt.chapterIndex))
			continue;

		if (it->flag == EVFLAG_DEFEAT_BOSS)
			return it->pid;
	}

	/**
	 * Step2: anyone who claims himself as boss is okay ╮(╯▽╰)╭
	 */
	for (i = UNIT_FACTION(unit) + 1; i < UNIT_FACTION(unit) + GetFactionUnitAmount(UNIT_FACTION(unit)); i++) {
		struct Unit *_unit = GetUnit(i);

		if (!UNIT_ALIVE(unit))
			continue;

		if (UNIT_CATTRIBUTES(_unit) & CA_BOSS)
			return UNIT_CHAR_ID(_unit);
	}

	return 0;
}
