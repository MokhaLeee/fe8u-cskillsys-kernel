#pragma once

#include "common-chax.h"

struct WeaponLockExConf {
	u8 iid, pid, jid, level;
	u16 evt_flag;
	u16 _pad_;
};

extern struct WeaponLockExConf const gWeaponLockExConfs[];
extern struct WeaponLockExConf const *const gpWeaponLockExConfs;
