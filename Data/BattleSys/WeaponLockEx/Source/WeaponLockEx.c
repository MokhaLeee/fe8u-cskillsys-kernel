#include "common-chax.h"
#include "weapon-lockex.h"

/**
 * This list has a very serious performance loss problem.
 * Although this feature has not been removed from kernel,
 * it is not recommended to abuse it.
 *
 * Call stack:
 * CanUnitUseWeapon
 * 	--> CheckWeaponLockEx
 */
const struct WeaponLockExConf gWeaponLockExConfs[] = {
	{}
};
