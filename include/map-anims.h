#pragma once

#include "common-chax.h"

void CallMapAnim_HeavyGravity(ProcPtr parent, int x, int y);
bool MapAnimHeavyGravityExists(void);

void CallMapAnim_ThunderStorm(ProcPtr parent, int x, int y);
bool MapAnimThunderStormExists(void);

void CallMapAnim_HealExt(ProcPtr parent, struct Unit * unit, int heal, void (* callback1)(ProcPtr), void (* callback2)(ProcPtr));
void CallMapAnim_Heal(ProcPtr parent, struct Unit * unit, int heal);
bool MapAnimHealExists(void);

void CallMapAnim_HurtExt(ProcPtr parent, struct Unit * unit, int damage, void (* callback1)(ProcPtr), void (* callback2)(ProcPtr));
void CallMapAnim_Hurt(ProcPtr parent, struct Unit * unit, int damage);
bool MapAnimHurtExists(void);
