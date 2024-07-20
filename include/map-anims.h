#pragma once

#include "common-chax.h"

void CallMapAnim_HeavyGravity(ProcPtr parent, int x, int y);
bool MapAnimHeavyGravityExists(void);

void CallMapAnim_ThunderStorm(ProcPtr parent, int x, int y);
bool MapAnimThunderStormExists(void);

void CallMapAnim_Heal(ProcPtr parent, struct Unit * unit, int heal);
bool MapAnimHealExists(void);
