#include "common-chax.h"
#include "bwl.h"

STATIC_DECLAR int GetUnitStaticHiddenLevel(struct Unit *unit)
{
	int hidden_lv;

	if (!UNIT_IS_VALID(unit))
		return 0;

	hidden_lv = gpClassPreLoadHiddenLevel[UNIT_CLASS_ID(unit)];
	if (hidden_lv == 0)
		hidden_lv = UNIT_CATTRIBUTES(unit) & CA_PROMOTED ? 15 : 0;

	return hidden_lv;
}

void NewBwlRecordHiddenLevel(struct Unit *unit)
{
	int level;
	struct NewBwl *bwl = GetNewBwl(UNIT_CHAR_ID(unit));

	if (!bwl)
		return;

	level = unit->level + bwl->levelGain;
	if (level > 127)
		level = 127;

	bwl->levelGain = level;
}

int GetUnitHiddenLevel(struct Unit *unit)
{
	struct NewBwl *bwl = GetNewBwl(UNIT_CHAR_ID(unit));

	if (bwl)
		return bwl->levelGain;

	return GetUnitStaticHiddenLevel(unit);
}

void UnitHiddenLevelPreLoad(struct Unit *unit)
{
	struct NewBwl *bwl = GetNewBwl(UNIT_CHAR_ID(unit));

	if (!bwl)
		return;

	bwl->levelGain = GetUnitStaticHiddenLevel(unit);
}

void WriteUnitLevelSafe(struct Unit *unit, int new_level)
{
	struct NewBwl *bwl = GetNewBwl(UNIT_CHAR_ID(unit));

	if (bwl)
		bwl->levelGain += new_level - unit->level;

	unit->level = new_level;
}
