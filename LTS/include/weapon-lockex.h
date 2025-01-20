#pragma once

#include "common-chax.h"

/**
 *  1: Force unlock weapon
 * -1: Force lock weapon
 *  0: Normal check
 */
int CheckWeaponLockEx(struct Unit *unit, u16 item);
