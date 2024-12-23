#include <common-chax.h>
#include <icon-rework.h>
#include <popup-reowrk.h>

static bool PopR_SetupBattleWeaponBroken(void)
{
	struct BattleUnit *bu = NULL;

#ifdef CONFIG_DEBUG_POPR
	SetPopupItem(ITEM_VULNERARY);
	return true;
#endif

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

#ifdef CONFIG_DEBUG_POPR
	SetPopupItem(ITYPE_SWORD);
	return true;
#endif

	if (BattleUnit_ShouldDisplayWRankUp(&gBattleActor))
		bu = &gBattleActor;
	else if (BattleUnit_ShouldDisplayWRankUp(&gBattleTarget))
		bu = &gBattleTarget;

	if (!bu)
		return false;

	SetPopupItem(bu->weaponType);
	return true;
}

static bool PopR_SetupPromotionWRankUpByWType(int wtype)
{
	if (gpEkrBattleUnitRight->unit.ranks[wtype] == 0 && gpEkrBattleUnitLeft->unit.ranks[wtype] != 0) {
		SetPopupItem(wtype);
		return true;
	}

#ifdef CONFIG_DEBUG_POPR
	SetPopupItem(wtype);
	return true;
#endif

	return false;
}

static bool PopR_SetupPromotionWRankUpSword(void) { return PopR_SetupPromotionWRankUpByWType(ITYPE_SWORD); }
static bool PopR_SetupPromotionWRankUpAxe(void)   { return PopR_SetupPromotionWRankUpByWType(ITYPE_AXE); }
static bool PopR_SetupPromotionWRankUpLance(void) { return PopR_SetupPromotionWRankUpByWType(ITYPE_LANCE); }
static bool PopR_SetupPromotionWRankUpBow(void)   { return PopR_SetupPromotionWRankUpByWType(ITYPE_BOW); }
static bool PopR_SetupPromotionWRankUpAnima(void) { return PopR_SetupPromotionWRankUpByWType(ITYPE_ANIMA); }
static bool PopR_SetupPromotionWRankUpLight(void) { return PopR_SetupPromotionWRankUpByWType(ITYPE_LIGHT); }
static bool PopR_SetupPromotionWRankUpDark(void)  { return PopR_SetupPromotionWRankUpByWType(ITYPE_DARK); }
static bool PopR_SetupPromotionWRankUpStaff(void) { return PopR_SetupPromotionWRankUpByWType(ITYPE_STAFF); }

struct PopupConfig const gBattlePopupConfigs[] = {
	{ PopR_SetupBattleWeaponBroken, PopupScr_WpnBroken, 0x60 },
	{ PopR_SetupBattleWRankUp, PopupScr_WRankUp, 0x60 },
	{}
};

struct PopupConfig const gPromotPopupConfigs[] = {
	{ PopR_SetupPromotionWRankUpSword, PopupScr_WRankUp, 0x60 },
	{ PopR_SetupPromotionWRankUpAxe,   PopupScr_WRankUp, 0x60 },
	{ PopR_SetupPromotionWRankUpLance, PopupScr_WRankUp, 0x60 },
	{ PopR_SetupPromotionWRankUpBow,   PopupScr_WRankUp, 0x60 },
	{ PopR_SetupPromotionWRankUpAnima, PopupScr_WRankUp, 0x60 },
	{ PopR_SetupPromotionWRankUpLight, PopupScr_WRankUp, 0x60 },
	{ PopR_SetupPromotionWRankUpDark,  PopupScr_WRankUp, 0x60 },
	{ PopR_SetupPromotionWRankUpStaff, PopupScr_WRankUp, 0x60 },
	{}
};
