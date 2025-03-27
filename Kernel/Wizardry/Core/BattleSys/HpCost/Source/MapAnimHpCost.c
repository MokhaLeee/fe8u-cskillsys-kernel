#include <common-chax.h>
#include <battle-system.h>

#include <gaiden-magic.h>

STATIC_DECLAR void MapAnimHitRound_StartCost(ProcPtr proc)
{
	gManimSt.hp_changing = true;
	PlaySeSpacial(SONG_75, gManimSt.actor[gManimSt.subjectActorId].unit->xPos * 0x10 - gBmSt.camera.x);
	RegisterMapHpChangeAnim(gManimSt.subjectActorId, GetExtBattleHitFromHit(gManimSt.pCurrentRound)->hp_cost);
}

STATIC_DECLAR void MapAnimHitRound_WaitCost(ProcPtr proc)
{
	if (gManimSt.hp_changing == false)
		Proc_Break(proc);
}

STATIC_DECLAR const struct ProcCmd ProcScr_MapAnimDisplayRoundWithHpCost[] = {
	PROC_YIELD,
	PROC_CALL(MapAnimHitRound_StartCost),
	PROC_REPEAT(MapAnimHitRound_WaitCost),
	PROC_YIELD,
	PROC_END
};

bool MapAnimRoundAnim_DisplayHpCost(ProcPtr parent)
{
	int round = GetBattleHitRound(gManimSt.pCurrentRound);
	int cost = GetExtBattleHit(round)->hp_cost;

	if (cost <= 0)
		return false;

	/* NO info box */
	if (GetSpellAssocReturnBool(gManimSt.actor[0].bu->weaponBefore) == false)
		return false;

	Proc_StartBlocking(ProcScr_MapAnimDisplayRoundWithHpCost, parent);
	return true;
}
