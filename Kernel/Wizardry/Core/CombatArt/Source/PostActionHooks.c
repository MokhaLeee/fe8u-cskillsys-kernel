#include "common-chax.h"
#include "combat-art.h"
#include "debuff.h"
#include "battle-system.h"
#include "map-anims.h"
#include "weapon-range.h"

STATIC_DECLAR void ExecCombatArtEffectAnim(ProcPtr proc)
{
	int i;
	struct Unit *unit = gActiveUnit;
	u8 cid = GetCombatArtInForce(unit);
	const struct CombatArtInfo *info;

	info = GetCombatArtInfo(cid);
	if (info->debuff == UNIT_STATUS_NONE)
		return;

	BmMapFill(gBmMapMovement, -1);
	BmMapFill(gBmMapRange, 0);

	if (info->aoe_debuff)
		AddMap(gCombatArtStatus.x, gCombatArtStatus.y, 0b11);
	else
		AddMap(gCombatArtStatus.x, gCombatArtStatus.y, 0b01);

	InitTargets(unit->xPos, unit->yPos);
	ForEachUnitInRange(AddUnitToTargetListIfNotAllied);

	for (i = 0; i < GetSelectTargetCount(); i++) {
		struct SelectTarget *starget = GetTarget(i);
		struct Unit *tunit = GetUnit(starget->uid);

		if (!UNIT_IS_VALID(tunit))
			continue;

		CallMapAnim_HeavyGravity(proc, starget->x, starget->y);
		SetUnitStatusExt(tunit, info->debuff, 1);
	}
}

STATIC_DECLAR const struct ProcCmd ProcScr_CombatArtPostActionEffect[] = {
	PROC_NAME("CombatArtPostActionEffect"),
	PROC_YIELD,
	PROC_CALL(ExecCombatArtEffectAnim),
	PROC_WHILE(MapAnimHeavyGravityExists),
	PROC_END
};

bool PostActionCombatArtEffect(ProcPtr parent)
{
	struct Unit *unit = gActiveUnit;
	u8 cid = GetCombatArtInForce(unit);
	const struct CombatArtInfo *info;

	if (!COMBART_VALID(cid) || !gBattleActorGlobalFlag.hitted)
		return false;

	info = GetCombatArtInfo(cid);

	if (info->debuff == UNIT_STATUS_NONE)
		return false;

	Proc_StartBlocking(ProcScr_CombatArtPostActionEffect, parent);
	return true;
}
