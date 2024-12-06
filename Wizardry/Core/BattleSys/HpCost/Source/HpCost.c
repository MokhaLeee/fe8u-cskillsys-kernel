#include <common-chax.h>
#include <battle-system.h>

#include <gaiden-magic.h>

void InitBattleHpCostData(void)
{
	CpuFill16(0, gBattleHpCostArray, sizeof(gBattleHpCostArray));
}

void AddBattleHpCost(int round, int cost)
{
	gBattleHpCostArray[round].cost += cost;
}

void BattleGenerateHitHpCost(struct BattleUnit *attacker, struct BattleUnit *defender)
{
	BattleGenerateHitHpCostForGaidenMagic(attacker, defender);

	/* On end */
	attacker->unit.curHP -= gBattleHpCostArray[GetCurrentBattleHitRound()].cost;
	if (attacker->unit.curHP < 0)
		attacker->unit.curHP = 0;
}

/**
 * Map anim
 */
static void MapAnimHitRound_StartCost(ProcPtr proc)
{
	int round = GetBattleHitRound(gManimSt.pCurrentRound - 1); // now this is next round!
	int cost = gBattleHpCostArray[round].cost;

	if (cost > 0) {
		gManimSt.hp_changing = true;
		RegisterMapHpChangeAnim(gManimSt.subjectActorId, cost);
	}
}

static void MapAnimHitRound_WaitCost(ProcPtr proc)
{
	if (gManimSt.hp_changing == false)
		Proc_Break(proc);
}

static void MapAnimHitRound_Vanilla(ProcPtr proc)
{
	Proc_StartBlocking(GetItemAnim6CCode(), proc);
}

static const struct ProcCmd ProcScr_MapAnimDisplayRoundWithHpCost[] = {
	PROC_YIELD,
	PROC_CALL(MapAnimHitRound_StartCost),
	PROC_REPEAT(MapAnimHitRound_WaitCost),
	PROC_CALL(MapAnimHitRound_Vanilla),
	PROC_YIELD,
	PROC_END
};

LYN_REPLACE_CHECK(MapAnim_DisplayRoundAnim);
void MapAnim_DisplayRoundAnim(ProcPtr proc)
{
	Proc_StartBlocking(ProcScr_MapAnimDisplayRoundWithHpCost, proc);
}

/**
 * Efx anim
 */
