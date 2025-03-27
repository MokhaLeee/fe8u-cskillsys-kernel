#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"

#define LOCAL_TRACE 0
void StartStatusHealEffect(struct Unit *unit, ProcPtr proc);

LYN_REPLACE_CHECK(StatusDecayDisplay_Display);
void StatusDecayDisplay_Display(struct UnknownBMUSAilmentProc *proc)
{
	struct SelectTarget *target = GetTarget(proc->unk_4C);
	struct Unit *unit = GetUnit(gActionData.subjectIndex);
	int status = GetUnitStatusIndex(unit);

	proc->unk_58 = status;

	SetUnitStatus(unit, 0);

	if (gpDebuffInfos[status].tick_type == STATUS_DEBUFF_TICK_ON_ENEMY)
		StartStatusHealEffect(GetUnit(target->uid), proc);
}

LYN_REPLACE_CHECK(TickActiveFactionTurn);
void TickActiveFactionTurn(void)
{
	int displayMapChange = FALSE;

	InitTargets(0, 0);

	FOR_UNITS_ONMAP_FACTION(gPlaySt.faction, unit, {
		if (unit->state & (US_UNAVAILABLE | US_RESCUED))
			continue;

		if (unit->barrierDuration != 0)
			unit->barrierDuration--;

		if (unit->torchDuration != 0) {
			unit->torchDuration--;
			displayMapChange = TRUE;
		}
	})

	if (displayMapChange) {
		RenderBmMapOnBg2();
		RefreshEntityBmMaps();
		RenderBmMap();
		NewBMXFADE(TRUE);
		RefreshUnitSprites();
	}

	#define DEC_STATUS(unit) \
		do { \
			int _status = GetUnitStatusIndex(unit); \
			int _duration = GetUnitStatusDuration(unit); \
			if (_status != 0 && _duration != 0) { \
				if (_status != UNIT_STATUS_RECOVER) \
					TryTickUnitStatusDuration(unit); \
				if (GetUnitStatusDuration(unit) == 0) \
					AddTarget(unit->xPos, unit->yPos, unit->index, 0); \
				LTRACEF("[uid=0x%02X, pid=0x%02X] status=%d, dura=%d", unit->index & 0xFF, UNIT_CHAR_ID(unit), _status, _duration); \
			} \
		} while (0)

	if (gPlaySt.faction == FACTION_BLUE) {
		/* Blue buff */
		FOR_UNITS_ONMAP_FACTION(FACTION_BLUE, unit, {
			if (gpDebuffInfos[GetUnitStatusIndex(unit)].tick_type == STATUS_DEBUFF_TICK_ON_ALLY)
				DEC_STATUS(unit);

			TickUnitStatDebuff(unit, STATUS_DEBUFF_TICK_ON_ALLY);
		})

		/* Red debuff */
		FOR_UNITS_ONMAP_FACTION(FACTION_RED, unit, {
			if (gpDebuffInfos[GetUnitStatusIndex(unit)].tick_type == STATUS_DEBUFF_TICK_ON_ENEMY)
				DEC_STATUS(unit);

			TickUnitStatDebuff(unit, STATUS_DEBUFF_TICK_ON_ENEMY);
		})
	} else if (gPlaySt.faction == FACTION_RED) {
		/* Red buff */
		FOR_UNITS_ONMAP_FACTION(FACTION_RED, unit, {
			if (gpDebuffInfos[GetUnitStatusIndex(unit)].tick_type == STATUS_DEBUFF_TICK_ON_ALLY)
				DEC_STATUS(unit);

			TickUnitStatDebuff(unit, STATUS_DEBUFF_TICK_ON_ALLY);
		})

		/* Blue debuff */
		FOR_UNITS_ONMAP_FACTION(FACTION_BLUE, unit, {
			if (gpDebuffInfos[GetUnitStatusIndex(unit)].tick_type == STATUS_DEBUFF_TICK_ON_ENEMY)
				DEC_STATUS(unit);

			TickUnitStatDebuff(unit, STATUS_DEBUFF_TICK_ON_ENEMY);
		})

		/* Green debuff */
		FOR_UNITS_ONMAP_FACTION(FACTION_GREEN, unit, {
			if (gpDebuffInfos[GetUnitStatusIndex(unit)].tick_type == STATUS_DEBUFF_TICK_ON_ENEMY)
				DEC_STATUS(unit);

			TickUnitStatDebuff(unit, STATUS_DEBUFF_TICK_ON_ENEMY);
		})
	} else if (gPlaySt.faction == FACTION_GREEN) {
		/* Green buff */
		FOR_UNITS_ONMAP_FACTION(FACTION_GREEN, unit, {
			if (gpDebuffInfos[GetUnitStatusIndex(unit)].tick_type == STATUS_DEBUFF_TICK_ON_ALLY)
				DEC_STATUS(unit);

			TickUnitStatDebuff(unit, STATUS_DEBUFF_TICK_ON_ALLY);
		})
	}
}
