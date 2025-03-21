#pragma once
#include "common-chax.h"

bool isWeaponTriangleAdvantage(int attackerWeapon, int defenderWeapon); // weapon type
bool weaponHasSpecialEffect(int weaponAttributes);  // weaponID
int findMax(u8 *array, int size);  // find max value in array and return index

int RandSkill(int id, struct Unit * unit);