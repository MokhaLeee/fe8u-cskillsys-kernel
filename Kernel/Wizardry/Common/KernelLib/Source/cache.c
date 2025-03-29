#include "common-chax.h"
#include "kernel-lib.h"

#define LOCAL_TRACE 1

STATIC_DECLAR void CalculateItemDataLengthCache(void)
{
	int i;
	const struct ItemData *start = GetItemData(0);

	for (i = 0; ; i++) {
		const struct ItemData *it = start + i;

		if (!IsPointerOrNULL((uintptr_t)(it->pStatBonuses)) || !IsPointerOrNULL((uintptr_t)(it->pEffectiveness)))
			break;
	}

	ItemDataLengthCache = i;
	LTRACEF("Cache: ItemDataLengthCache=%d", ItemDataLengthCache);
}

void GameInit_InitCache(void)
{
	CalculateItemDataLengthCache();
}
