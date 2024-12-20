#include <common-chax.h>
#include <icon-rework.h>
#include <popup-reowrk.h>

static bool PopR_SetupBattleWeaponBroken(void)
{
	struct BattleUnit *bu = NULL;

	if (BattleUnit_ShouldDisplayWpnBroke(&gBattleActor))
		bu = &gBattleActor;
	else if (BattleUnit_ShouldDisplayWpnBroke(&gBattleTarget))
		bu = &gBattleTarget;

	if (!bu)
		return false;

	SetPopupItem(bu->weaponBefore);
	return true;
}

static bool PopR_SetupBattleWRankUp(void)
{
	struct BattleUnit *bu = NULL;

	if (BattleUnit_ShouldDisplayWRankUp(&gBattleActor))
		bu = &gBattleActor;
	else if (BattleUnit_ShouldDisplayWRankUp(&gBattleTarget))
		bu = &gBattleTarget;

	if (!bu)
		return false;

	SetPopupItem(bu->weaponType);
	return true;
}

struct PopupConfig const gBattlePopupConfigs[] = {
	{
		PopR_SetupBattleWeaponBroken,
		PopupScr_WpnBroken,
		0x60
	},
	{
		PopR_SetupBattleWRankUp,
		PopupScr_WRankUp,
		0x60
	},
	{}
};
