#include "common-chax.h"
#include "item-sys.h"

STATIC_DECLAR struct SpellAssoc *GetSpellAssocStructPtrVanilla(u16 item)
{
	struct SpellAssoc *it = pr_SpellAssocData;
	u16 iid = GetItemIndex(item);
	u16 item_;

	item_ = it->item;
	if (item_ != 0xFFFF) {
		while (item_ != iid) {
			item_ = (++it)->item;
			if (item_ == 0xFFFF)
				break;
		}
	}
	return it;
}

STATIC_DECLAR struct SpellAssoc *GetSpellAssocStructPtrExt(u16 item)
{
	struct SpellAssoc *it = ExtSpellAssocData;
	u16 iid = GetItemIndex(item);
	u16 item_;

	item_ = it->item;
	if (item_ != 0xFFFF) {
		while (item_ != iid) {
			item_ = (++it)->item;
			if (item_ == 0xFFFF)
				break;
		}
	}
	return it;
}

LYN_REPLACE_CHECK(GetSpellAssocStructPtr);
struct SpellAssoc *GetSpellAssocStructPtr(u16 item)
{
	struct SpellAssoc *ret;

	ret = GetSpellAssocStructPtrExt(item);
	if (ret && ret->item != 0xFFFF)
		return ret;

	return GetSpellAssocStructPtrVanilla(item);
}
