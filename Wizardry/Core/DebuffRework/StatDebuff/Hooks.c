#include "common-chax.h"
#include "debuff.h"
#include "battle-system.h"

LYN_REPLACE_CHECK(ExecDanceRing);
void ExecDanceRing(ProcPtr proc)
{
	int status = 0;

	BattleInitItemEffect(GetUnit(gActionData.subjectIndex),
		gActionData.itemSlotIndex);

	BattleInitItemEffectTarget(GetUnit(gActionData.targetIndex));

#if CHAX
	switch (GetItemIndex(GetItemFromSlot(GetUnit(gActionData.subjectIndex), gActionData.itemSlotIndex))) {
	case ITEM_FILLAS_MIGHT:
		status = UNIT_STATUS_ATTACK;
		break;
	case ITEM_NINISS_GRACE:
		status = UNIT_STATUS_DEFENSE;
		break;
	case ITEM_THORS_IRE:
		status = UNIT_STATUS_CRIT;
		break;
	case ITEM_SETS_LITANY:
		status = UNIT_STATUS_AVOID;
		break;
	}

	SetUnitStatusExt(GetUnit(gActionData.targetIndex), status, 1);
#else
	switch (GetItemIndex(GetUnit(gActionData.subjectIndex)->items[gActionData.itemSlotIndex])) {
	case ITEM_FILLAS_MIGHT:
		status = UNIT_STAT_BUFF_RING_ATK;
		break;
	case ITEM_NINISS_GRACE:
		status = UNIT_STAT_BUFF_RING_DEF;
		break;
	case ITEM_THORS_IRE:
		status = UNIT_STAT_BUFF_RING_CRT;
		break;
	case ITEM_SETS_LITANY:
		status = UNIT_STAT_BUFF_RING_AVO;
		break;
	}

	SetUnitStatDebuff(GetUnit(gActionData.targetIndex), status);
#endif

	gBattleStats.config = BATTLE_CONFIG_DANCERING;

	BattleApplyItemEffect(proc);
	BeginBattleAnimations();
}
