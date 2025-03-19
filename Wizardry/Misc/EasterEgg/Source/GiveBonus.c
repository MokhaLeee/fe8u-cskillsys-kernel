#include <common-chax.h>

#include "Bonus.h"

#define MOKHA_BONUS_CONTENT_MSG "Mokha bonus\0"

static const u8 iid_bonus[] = {
	ITEM_BOOSTER_HP,
	ITEM_BOOSTER_POW,
	ITEM_BOOSTER_SKL,
	ITEM_BOOSTER_SPD,
	ITEM_BOOSTER_LCK,
	ITEM_BOOSTER_DEF,
	ITEM_BOOSTER_RES,
	ITEM_BOOSTER_MOV,
	ITEM_BOOSTER_CON,
};

void GiveKernelBonus(void)
{
	int i, ret;
	int bonus_item = iid_bonus[NextRN_N(ARRAY_COUNT(iid_bonus)) - 1];
	struct BonusClaimEnt *buf = (void *)gGenericBuffer;

	WARN_GENERIC_BUF_USED;

	ret = LoadBonusContentData(buf);
	if (ret == false)
		return;

	for (i = 0; i < 0x10; i++) {
		if (buf[i].unseen != true)
			continue;

		if (strcmp(buf[i].str, MOKHA_BONUS_CONTENT_MSG))
			return;
	}

	for (i = 0; i < 0x10; i++) {
		if (buf[i].unseen == true)
			continue;

		buf[i].unseen = true;
		buf[i].kind = BONUSKIND_ITEM0;
		buf[i].itemId = bonus_item;
		strcpy(buf[i].str, MOKHA_BONUS_CONTENT_MSG);
		break;
	}
	SaveBonusContentData(buf);
	WARN_GENERIC_BUF_RELEASED;
}

LYN_REPLACE_CHECK(LoadSavedEid8A);
bool LoadSavedEid8A(int slot)
{
	/**
	 * Check can exec bonus-claim
	 */
	return true;
}
