#include "common-chax.h"
#include "unit-expa.h"
#include "kernel-lib.h"

struct DemoUnitExpa *GetDemoUnitExpa(struct Unit *unit)
{
	if (unit == &gBattleActor.unit)
		return &sDemoUnitExpaBattle[0];
	else if (unit == &gBattleTarget.unit)
		return &sDemoUnitExpaBattle[1];

	return gpDemoUnitExpaPool[unit->index & 0xFF];
}

void MSU_SaveDemoUnitExpa(u8 *dst, const u32 size)
{
	if (size < (sizeof(sDemoUnitExpaAlly) + sizeof(sDemoUnitExpaEnemy) + sizeof(sDemoUnitExpaNpc))) {
		Errorf("ENOMEM: %d", size);
		hang();
	}

	WriteAndVerifySramFast(
		sDemoUnitExpaAlly,
		dst,
		sizeof(sDemoUnitExpaAlly));

	dst += sizeof(sDemoUnitExpaAlly);

	WriteAndVerifySramFast(
		sDemoUnitExpaEnemy,
		dst,
		sizeof(sDemoUnitExpaEnemy));

	dst += sizeof(sDemoUnitExpaEnemy);

	WriteAndVerifySramFast(
		sDemoUnitExpaNpc,
		dst,
		sizeof(sDemoUnitExpaNpc));
}

void MSU_LoadDemoUnitExpa(u8 *src, const u32 size)
{
	if (size < (sizeof(sDemoUnitExpaAlly) + sizeof(sDemoUnitExpaEnemy) + sizeof(sDemoUnitExpaNpc))) {
		Errorf("ENOMEM: %d", size);
		hang();
	}

	ReadSramFast(
		src,
		sDemoUnitExpaAlly,
		sizeof(sDemoUnitExpaAlly));

	src += sizeof(sDemoUnitExpaAlly);

	WriteAndVerifySramFast(
		src,
		sDemoUnitExpaEnemy,
		sizeof(sDemoUnitExpaEnemy));

	src += sizeof(sDemoUnitExpaEnemy);

	WriteAndVerifySramFast(
		src,
		sDemoUnitExpaNpc,
		sizeof(sDemoUnitExpaNpc));
}

void ResetDemoUnitExpa(void)
{
	CpuFastFill16(0, sDemoUnitExpaAlly, sizeof(sDemoUnitExpaAlly));
	CpuFastFill16(0, sDemoUnitExpaEnemy, sizeof(sDemoUnitExpaEnemy));
	memset(sDemoUnitExpaNpc, 0, sizeof(sDemoUnitExpaNpc));
}

/**
 * Hooks
 */
void DemoUnitExpa_OnNewGameInit(void)
{
	ResetDemoUnitExpa();
}

void DemoUnitExpa_OnClearUnit(struct Unit *unit)
{
	struct DemoUnitExpa *expa = GetDemoUnitExpa(unit);

	Printf("UID=0x%02X, expa=%p", unit->index & 0xFF, expa);

	memset(expa, 0, sizeof(struct DemoUnitExpa));
}

void DemoUnitExpa_OnChapterInit(void)
{
	/**
	 * External hooks
	 */
}

void DemoUnitExpa_OnLoadUnit(struct Unit *unit)
{
	struct DemoUnitExpa *expa = GetDemoUnitExpa(unit);

	memset(expa, 0, sizeof(struct DemoUnitExpa));

	/**
	 * External hooks
	 */
}

void DemoUnitExpa_OnRefrain(struct Unit *unit)
{
	/**
	 * External hooks
	 */
}

void DemoUnitExpa_OnCopyUnit(struct Unit *from, struct Unit *to)
{
	memcpy(
		GetDemoUnitExpa(to),
		GetDemoUnitExpa(from),
		sizeof(struct DemoUnitExpa));
}

void DemoUnitExpa_OnUnitToBattle(struct Unit *unit, struct BattleUnit *bu)
{
	*GetDemoUnitExpa(&bu->unit) = *GetDemoUnitExpa(unit);
}

void DemoUnitExpa_OnBattleToUnit(struct Unit *unit, struct BattleUnit *bu)
{
	*GetDemoUnitExpa(unit) = *GetDemoUnitExpa(&bu->unit);
}
