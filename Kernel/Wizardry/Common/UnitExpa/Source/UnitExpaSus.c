#include "common-chax.h"
#include "unit-expa.h"
#include "kernel-lib.h"

struct DemoUnitExpaSus *GetDemoUnitExpaSus(struct Unit *unit)
{
	if (unit == &gBattleActor.unit)
		return &sDemoUnitExpaSusBattle[0];
	else if (unit == &gBattleTarget.unit)
		return &sDemoUnitExpaSusBattle[1];

	return gpDemoUnitExpaSusPool[unit->index & 0xFF];
}

void MSU_SaveDemoUnitExpaSus(u8 *dst, const u32 size)
{
	if (size < (sizeof(sDemoUnitExpaSusAlly) + sizeof(sDemoUnitExpaSusEnemy) + sizeof(sDemoUnitExpaSusNpc))) {
		Errorf("ENOMEM: %d", size);
		hang();
	}

	WriteAndVerifySramFast(
		sDemoUnitExpaSusAlly,
		dst,
		sizeof(sDemoUnitExpaSusAlly));

	dst += sizeof(sDemoUnitExpaSusAlly);

	WriteAndVerifySramFast(
		sDemoUnitExpaSusEnemy,
		dst,
		sizeof(sDemoUnitExpaSusEnemy));

	dst += sizeof(sDemoUnitExpaSusEnemy);

	WriteAndVerifySramFast(
		sDemoUnitExpaSusNpc,
		dst,
		sizeof(sDemoUnitExpaSusNpc));
}

void MSU_LoadDemoUnitExpaSus(u8 *src, const u32 size)
{
	if (size < (sizeof(sDemoUnitExpaSusAlly) + sizeof(sDemoUnitExpaSusEnemy) + sizeof(sDemoUnitExpaSusNpc))) {
		Errorf("ENOMEM: %d", size);
		hang();
	}

	ReadSramFast(
		src,
		sDemoUnitExpaSusAlly,
		sizeof(sDemoUnitExpaSusAlly));

	src += sizeof(sDemoUnitExpaSusAlly);

	WriteAndVerifySramFast(
		src,
		sDemoUnitExpaSusEnemy,
		sizeof(sDemoUnitExpaSusEnemy));

	src += sizeof(sDemoUnitExpaSusEnemy);

	WriteAndVerifySramFast(
		src,
		sDemoUnitExpaSusNpc,
		sizeof(sDemoUnitExpaSusNpc));
}

STATIC_DECLAR void ResetDemoUnitExpaSus(void)
{
	CpuFastFill16(0, sDemoUnitExpaSusAlly, sizeof(sDemoUnitExpaSusAlly));
	CpuFastFill16(0, sDemoUnitExpaSusEnemy, sizeof(sDemoUnitExpaSusEnemy));
	memset(sDemoUnitExpaSusNpc, 0, sizeof(sDemoUnitExpaSusNpc));
}

/**
 * Hooks
 */
void DemoUnitExpaSus_OnNewGameInit(void)
{
	ResetDemoUnitExpaSus();
}

void DemoUnitExpaSus_OnClearUnit(struct Unit *unit)
{
	struct DemoUnitExpaSus *expa = GetDemoUnitExpaSus(unit);

	memset(expa, 0, sizeof(struct DemoUnitExpaSus));
}

void DemoUnitExpaSus_OnChapterInit(void)
{
	/**
	 * External hooks
	 */
}

void DemoUnitExpaSus_OnLoadUnit(struct Unit *unit)
{
	struct DemoUnitExpaSus *expa = GetDemoUnitExpaSus(unit);

	memset(expa, 0, sizeof(struct DemoUnitExpaSus));

	/**
	 * External hooks
	 */
}

void DemoUnitExpaSus_OnRefrain(struct Unit *unit)
{
	/**
	 * External hooks
	 */
}

void DemoUnitExpaSus_OnCopyUnit(struct Unit *from, struct Unit *to)
{
	memcpy(
		GetDemoUnitExpaSus(to),
		GetDemoUnitExpaSus(from),
		sizeof(struct DemoUnitExpaSus));
}

void DemoUnitExpaSus_OnUnitToBattle(struct Unit *unit, struct BattleUnit *bu)
{
	*GetDemoUnitExpaSus(&bu->unit) = *GetDemoUnitExpaSus(unit);
}

void DemoUnitExpaSus_OnBattleToUnit(struct Unit *unit, struct BattleUnit *bu)
{
	*GetDemoUnitExpaSus(unit) = *GetDemoUnitExpaSus(&bu->unit);
}
