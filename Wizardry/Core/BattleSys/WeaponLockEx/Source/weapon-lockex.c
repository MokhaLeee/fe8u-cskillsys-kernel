#include "common-chax.h"
#include "weapon-lockex.h"
#include "wpnlockex-internal.h"

int CheckWeaponLockEx(struct Unit *unit, u16 item)
{
	int ret = 0;
	const struct WeaponLockExConf *it;

	for (it = gpWeaponLockExConfs; it->iid != 0; it++) {
		if (ITEM_INDEX(item) == it->iid) {
			if (it->pid != 0) {
				if (it->pid != UNIT_CHAR_ID(unit)) {
					ret = -1;
					continue;
				}
			}

			if (it->jid != 0) {
				if (it->jid != UNIT_CLASS_ID(unit)) {
					ret = -1;
					continue;
				}
			}

			if (it->level != 0) {
				if (unit->level < it->level) {
					ret = -1;
					continue;
				}
			}

			if (it->evt_flag != 0) {
				if (CheckFlag(it->evt_flag) == false) {
					ret = -1;
					continue;
				}
			}
			return 1;
		}
	}
	if (ret == -1)
		return -1;

	/* This weapon is not configured in wpnlockex */
	return 0;
}
