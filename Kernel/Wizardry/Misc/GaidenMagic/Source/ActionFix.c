#include "common-chax.h"

LYN_REPLACE_CHECK(HandlePostActionTraps);
bool HandlePostActionTraps(ProcPtr proc)
{
	if (GetUnitCurrentHp(gActiveUnit) <= 0)
		return true;

	if ((UNIT_CATTRIBUTES(gActiveUnit) & CA_CANTO) && !(gActiveUnit->state & US_CANTOING)) {
		switch (gActionData.unitActionType) {
		case UNIT_ACTION_WAIT:
		case UNIT_ACTION_COMBAT:
		case UNIT_ACTION_STAFF:
			break;

#if CHAX
		case CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat:
		case CONFIG_UNIT_ACTION_EXPA_GaidenMagicStaff:
			break;
#endif

		default:
			return true;
		}
	}

	if (!GetPickTrapType(gActiveUnit))
		return true;

	gActionData.suspendPointType = SUSPEND_POINT_DURINGACTION;
	gActionData.unitActionType = UNIT_ACTION_WAIT;
	WriteSuspendSave(SAVE_ID_SUSPEND);

	if (GetBattleAnimPreconfType() == PLAY_ANIMCONF_OFF)
		RefreshUnitSprites();

	return ExecTrap(proc, gActiveUnit, 0);
}

LYN_REPLACE_CHECK(ShouldCallBattleQuote);
bool ShouldCallBattleQuote(u8 pidA, u8 pidB)
{
	if (GetBattleMapKind() == BATTLEMAP_KIND_SKIRMISH)
		return false;

#if CHAX
	switch (gActionData.unitActionType) {
	case UNIT_ACTION_COMBAT:
	case CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat:
		break;

	default:
		return false;
	}
#else
	if (gActionData.unitActionType != UNIT_ACTION_COMBAT)
		return true;
#endif

	if (GetBattleQuoteEntry(pidA, pidB))
		return true;

	if (GetBattleQuoteEntry(pidA, 0))
		return true;

	if (GetBattleQuoteEntry(0, pidB))
		return true;

	return false;
}

LYN_REPLACE_CHECK(CallBattleQuoteEventsIfAny);
void CallBattleQuoteEventsIfAny(u8 pidA, u8 pidB)
{
	struct BattleTalkExtEnt *ent;

	if (GetBattleMapKind() == BATTLEMAP_KIND_SKIRMISH)
		return;

#if CHAX
	switch (gActionData.unitActionType) {
	case UNIT_ACTION_COMBAT:
	case CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat:
		break;

	default:
		return;
	}
#else
	if (gActionData.unitActionType != UNIT_ACTION_COMBAT)
		return;
#endif

	ent = GetBattleQuoteEntry(pidA, pidB);
	if (ent == NULL)
		ent = GetBattleQuoteEntry(pidA, 0);
	if (ent == NULL)
		ent = GetBattleQuoteEntry(0, pidB);

	if (ent != NULL) {
		if (ent->msg)
			CallBattleQuoteEventInBattle(ent->msg);
		else if (ent->event != 0)
			EventEngine_CreateBattle((u16 *)ent->event);

		SetFlag(ent->flag);
	}
}
