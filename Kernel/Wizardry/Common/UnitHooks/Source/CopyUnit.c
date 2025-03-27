#include "common-chax.h"

typedef int (*CopyUnitFunc_t)(struct Unit *from, struct Unit *to);
// extern const CopyUnitFunc_t gCopyUnitHooks[];
extern CopyUnitFunc_t const *const gpCopyUnitHooks;

LYN_REPLACE_CHECK(CopyUnit);
void CopyUnit(struct Unit *from, struct Unit *to)
{
	u8 id = to->index;

#if CHAX
	const CopyUnitFunc_t *it;

	for (it = gpCopyUnitHooks; *it; it++)
		(*it)(from, to);
#endif

	memcpy(to, from, sizeof(struct Unit));
	to->index = id;
}
