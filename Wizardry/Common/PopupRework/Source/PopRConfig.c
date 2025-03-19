#include <common-chax.h>
#include <icon-rework.h>
#include <skill-system.h>
#include <popup-reowrk.h>

STATIC_DECLAR bool PopR_SetupBattleWeaponBroken(void)
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

STATIC_DECLAR bool PopR_SetupBattleWRankUp(void)
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

STATIC_DECLAR bool PopR_SetupPromotionWRankUpByWType(int wtype)
{
	if (UNIT_WRANK(&gpEkrBattleUnitRight->unit, wtype) == 0 && UNIT_WRANK(&gpEkrBattleUnitLeft->unit, wtype) != 0) {
		SetPopupItem(wtype);
		return true;
	}

#ifdef CONFIG_DEBUG_POPR
	SetPopupItem(wtype);
	return true;
#endif

	return false;
}

STATIC_DECLAR bool PopR_SetupPromotionWRankUpSword(void) { return PopR_SetupPromotionWRankUpByWType(ITYPE_SWORD); }
STATIC_DECLAR bool PopR_SetupPromotionWRankUpAxe(void)   { return PopR_SetupPromotionWRankUpByWType(ITYPE_AXE); }
STATIC_DECLAR bool PopR_SetupPromotionWRankUpLance(void) { return PopR_SetupPromotionWRankUpByWType(ITYPE_LANCE); }
STATIC_DECLAR bool PopR_SetupPromotionWRankUpBow(void)   { return PopR_SetupPromotionWRankUpByWType(ITYPE_BOW); }
STATIC_DECLAR bool PopR_SetupPromotionWRankUpAnima(void) { return PopR_SetupPromotionWRankUpByWType(ITYPE_ANIMA); }
STATIC_DECLAR bool PopR_SetupPromotionWRankUpLight(void) { return PopR_SetupPromotionWRankUpByWType(ITYPE_LIGHT); }
STATIC_DECLAR bool PopR_SetupPromotionWRankUpDark(void)  { return PopR_SetupPromotionWRankUpByWType(ITYPE_DARK); }
STATIC_DECLAR bool PopR_SetupPromotionWRankUpStaff(void) { return PopR_SetupPromotionWRankUpByWType(ITYPE_STAFF); }

struct PopupConfig const gBattlePopupConfigs[] = {
	{ PopR_SetupBattleWeaponBroken, PopupScr_WpnBroken, 0x60 },
	{ PopR_SetupBattleWRankUp, PopupScr_WRankUp, 0x60 },

	/**
	 * Learn new skills
	 */
	{ PopR_SetupLearnSkill, PopupScr_LearnSkill, 0x60 },
	{ PopR_SetupLearnSkill, PopupScr_LearnSkill, 0x60 },
	{ PopR_SetupLearnSkill, PopupScr_LearnSkill, 0x60 },
	{ PopR_SetupLearnSkill, PopupScr_LearnSkill, 0x60 },
	{ PopR_SetupLearnSkill, PopupScr_LearnSkill, 0x60 },
	{ PopR_SetupLearnSkill, PopupScr_LearnSkill, 0x60 },
	{ PopR_SetupLearnSkill, PopupScr_LearnSkill, 0x60 },
	{ PopR_SetupLearnSkill, PopupScr_LearnSkill, 0x60 },
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

	/**
	 * Learn new skills
	 */
	{ PopR_SetupLearnSkill, PopupScr_LearnSkill, 0x60 },
	{ PopR_SetupLearnSkill, PopupScr_LearnSkill, 0x60 },
	{ PopR_SetupLearnSkill, PopupScr_LearnSkill, 0x60 },
	{ PopR_SetupLearnSkill, PopupScr_LearnSkill, 0x60 },
	{ PopR_SetupLearnSkill, PopupScr_LearnSkill, 0x60 },
	{ PopR_SetupLearnSkill, PopupScr_LearnSkill, 0x60 },
	{ PopR_SetupLearnSkill, PopupScr_LearnSkill, 0x60 },
	{ PopR_SetupLearnSkill, PopupScr_LearnSkill, 0x60 },
	{}
};
