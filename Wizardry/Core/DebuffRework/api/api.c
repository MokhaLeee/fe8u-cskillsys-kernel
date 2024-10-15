#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"

bool UnitHasNegativeStatus(struct Unit *unit)
{
	int i;
	const struct DebuffInfo *debuff;
	struct StatDebuffStatus *sdebuff;

	/* Debuff */
	debuff = &gpDebuffInfos[GetUnitStatusIndex(unit)];
	if (debuff->positive_type == STATUS_DEBUFF_NEGATIVE)
		return true;

	/* StatDebuff */
	sdebuff = GetUnitStatDebuffStatus(unit);
	for (i = UNIT_STAT_DEBUFF_IDX_START; i < UNIT_STAT_DEBUFF_MAX; i++) {
		if (!_BIT_CHK(sdebuff->st.bitmask, i))
			continue;

		if (gpStatDebuffInfos[i].positive_type == STATUS_DEBUFF_NEGATIVE)
			return true;
	}
	return false;
}

bool UnitHasPositiveStatus(struct Unit *unit)
{
	int i;
	const struct DebuffInfo *debuff;
	struct StatDebuffStatus *sdebuff;

	/* Debuff */
	debuff = &gpDebuffInfos[GetUnitStatusIndex(unit)];
	if (debuff->positive_type == STATUS_DEBUFF_POSITIVE)
		return true;

	/* StatDebuff */
	sdebuff = GetUnitStatDebuffStatus(unit);
	for (i = UNIT_STAT_DEBUFF_IDX_START; i < UNIT_STAT_DEBUFF_MAX; i++) {
		if (!_BIT_CHK(sdebuff->st.bitmask, i))
			continue;

		if (gpStatDebuffInfos[i].positive_type == STATUS_DEBUFF_POSITIVE)
			return true;
	}
	return false;
}

void RemoveUnitNegativeStatus(struct Unit *unit)
{
	int i, debuff;
	struct StatDebuffStatus *sdebuff;

	/* Debuff */
	debuff = GetUnitStatusIndex(unit);
	if (IsDebuff(debuff)) {
		switch (debuff) {
		case UNIT_STATUS_PETRIFY:
		case UNIT_STATUS_13:
			unit->state = unit->state & ~US_UNSELECTABLE;
		}

		SetUnitStatus(unit, UNIT_STATUS_NONE);
	}

	/* StatDebuff */
	sdebuff = GetUnitStatDebuffStatus(unit);
	for (i = UNIT_STAT_DEBUFF_IDX_START; i < UNIT_STAT_DEBUFF_MAX; i++) {
		if (!_BIT_CHK(sdebuff->st.bitmask, i))
			continue;

		if (gpStatDebuffInfos[i].positive_type == STATUS_DEBUFF_NEGATIVE)
			_BIT_CLR(sdebuff->st.bitmask, i);
	}
}

void RemoveUnitPositiveStatus(struct Unit *unit)
{
	int i, debuff;
	struct StatDebuffStatus *sdebuff;

	/* Debuff */
	debuff = GetUnitStatusIndex(unit);
	if (!IsDebuff(debuff))
		SetUnitStatus(unit, UNIT_STATUS_NONE);

	/* StatDebuff */
	sdebuff = GetUnitStatDebuffStatus(unit);
	for (i = UNIT_STAT_DEBUFF_IDX_START; i < UNIT_STAT_DEBUFF_MAX; i++) {
		if (!_BIT_CHK(sdebuff->st.bitmask, i))
			continue;

		if (gpStatDebuffInfos[i].positive_type == STATUS_DEBUFF_POSITIVE)
			_BIT_CLR(sdebuff->st.bitmask, i);
	}
}
