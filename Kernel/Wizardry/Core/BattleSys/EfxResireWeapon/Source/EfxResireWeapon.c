#include "common-chax.h"
#include "item-sys.h"
#include "battle-system.h"

#define LOCAL_TRACE 0

extern SpellAnimFunc const *const pr_EfxMagicTable;

extern EWRAM_DATA u8 sEfxResireEfxIndexCacheMagic;
extern EWRAM_DATA u8 sEfxResireEfxIndexCacheData;

enum { EFX_RESIRE_EFX_INDEX_CACHE_MAGIC = 0x89 };

STATIC_DECLAR int GetEfxResireEfxIndex(void)
{
	if (sEfxResireEfxIndexCacheMagic != EFX_RESIRE_EFX_INDEX_CACHE_MAGIC) {
		const struct SpellAssoc *assoc_it;
		int i, max_efx;
		int efx, efx_ref1, efx_ref2;

		/**
		 * Check on spell-assoc list
		 */
		max_efx = -1;
		efx_ref1 = -1;
		for (assoc_it = pr_SpellAssocData; assoc_it->item != 0xFFFF; assoc_it++) {
			if (max_efx < assoc_it->efx)
				max_efx = assoc_it->efx;

			if (assoc_it->item == ITEM_DARK_NOSFERATU)
				efx_ref1 = assoc_it->efx;
		}

		/**
		 * Check on efxmagic table
		 */
		efx_ref2 = -1;

		for (i = 0; i < max_efx; i++) {
			if (pr_EfxMagicTable[i] == StartSpellAnimNosferatu) {
				efx_ref2 = i;
				break;
			}
		}

		LTRACEF("Global efx-resire ref: %d %d", efx_ref1, efx_ref2);

		if (efx_ref1 != -1)
			efx = efx_ref1;
		else if (efx_ref2 != -1)
			efx = efx_ref2;
		else
			efx = 0x1E; // well...

		sEfxResireEfxIndexCacheMagic = EFX_RESIRE_EFX_INDEX_CACHE_MAGIC;
		sEfxResireEfxIndexCacheData  = efx;
	}
	return sEfxResireEfxIndexCacheData;
}

bool CheckEfxMagicIsResire(int efx)
{
	if (efx == GetEfxResireEfxIndex())
		return true;

	return false;
}

bool CheckWeaponIsEfxResire(int weapon)
{
	int iid = ITEM_INDEX(weapon);
	const struct SpellAssoc *assoc_it;

	/* ext spellassoc */
	for (assoc_it = ExtSpellAssocData; assoc_it->item != 0xFFFF; assoc_it++) {
		if (assoc_it->item == iid) {
			if (assoc_it->efx == GetEfxResireEfxIndex())
				return true;
			else
				return false;
		}
	}

	for (assoc_it = pr_SpellAssocData; assoc_it->item != 0xFFFF; assoc_it++) {
		if (assoc_it->item == iid) {
			if (assoc_it->efx == GetEfxResireEfxIndex())
				return true;
			else
				return false;
		}
	}
	Errorf("Failed to find spell-asooc! item=0x%04X", weapon);
	return false;
}
