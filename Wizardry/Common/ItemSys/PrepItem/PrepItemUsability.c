#include "global.h"
#include "bmunit.h"
#include "prepscreen.h"
#include "bmitemuse.h"
#include "bmunit.h"
#include "bmitem.h"
#include "constants/items.h"

#include "common-chax.h"

STATIC_DECLAR bool CanUnitUseMetiStome(struct Unit * unit, int item);

typedef bool (* PrepItemUsabilityFunc_t)(struct Unit * unit, int item);
PrepItemUsabilityFunc_t const gPrepItemUsabilityFuncs[0x100] = {
    [ITEM_BOOSTER_HP] =  CanUnitUseStatGainItem,
    [ITEM_BOOSTER_POW] = CanUnitUseStatGainItem,
    [ITEM_BOOSTER_SKL] = CanUnitUseStatGainItem,
    [ITEM_BOOSTER_SPD] = CanUnitUseStatGainItem,
    [ITEM_BOOSTER_LCK] = CanUnitUseStatGainItem,
    [ITEM_BOOSTER_DEF] = CanUnitUseStatGainItem,
    [ITEM_BOOSTER_RES] = CanUnitUseStatGainItem,
    [ITEM_BOOSTER_MOV] = CanUnitUseStatGainItem,
    [ITEM_BOOSTER_CON] = CanUnitUseStatGainItem,

    [ITEM_HEROCREST] =   CanUnitUsePromotionItem,
	[ITEM_KNIGHTCREST] = CanUnitUsePromotionItem,
	[ITEM_ORIONSBOLT] =  CanUnitUsePromotionItem,
	[ITEM_ELYSIANWHIP] = CanUnitUsePromotionItem,
	[ITEM_GUIDINGRING] = CanUnitUsePromotionItem,
	[ITEM_MASTERSEAL] =  CanUnitUsePromotionItem,
	[ITEM_HEAVENSEAL] =  CanUnitUsePromotionItem,
	[ITEM_OCEANSEAL] =   CanUnitUsePromotionItem,
	[ITEM_LUNARBRACE] =  CanUnitUsePromotionItem,
	[ITEM_SOLARBRACE] =  CanUnitUsePromotionItem,
	[ITEM_UNK_C1] =      CanUnitUsePromotionItem,

    [ITEM_METISSTOME] = CanUnitUseMetiStome,
    [ITEM_JUNAFRUIT] = (void *)CanUnitUseFruitItem,
};

/* LynJump */
bool CanUnitUseItemPrepScreen(struct Unit* unit, int item)
{
    PrepItemUsabilityFunc_t it = gPrepItemUsabilityFuncs[ITEM_INDEX(item)];

    if (it)
        return it(unit, item);

    return false;
}

STATIC_DECLAR bool CanUnitUseMetiStome(struct Unit * unit, int item)
{
    if (unit->state & US_GROWTH_BOOST)
        return false;

    return true;
}
