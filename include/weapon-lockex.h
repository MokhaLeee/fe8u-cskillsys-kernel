#pragma once

#include "common-chax.h"

struct WeaponLockExConf {
    u8 iid, pid, jid, level;
    u16 evt_flag;
    u16 _pad_;
};
extern const struct WeaponLockExConf gWeaponLockExConfs[];

/**
 *  1: Force unlock weapon
 * -1: Force lock weapon
 *  0: Normal check
 */
int CheckWeaponLockEx(struct Unit * unit, u16 item);
