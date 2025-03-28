#include "common-chax.h"
#include "weapon-lockex.h"

#define LOCAL_TRACE 0

int CheckWeaponLockEx(struct Unit *unit, u16 item)
{
	int ret = 0;
	const struct WeaponLockExConf *it = *pr_gpWeaponLockExConfs;

	for (; it->iid != 0; it++) {
		LTRACEF("iid=0x%02X, pid=0x%02X, jid=0x%02X, level=%d, evt_flag=0x%02X",
			it->iid, it->pid, it->jid, it->level, it->evt_flag);

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
