#pragma once

#include "common-chax.h"

extern const u16 Img_HeavyGravity[];
extern const u16 Pal_NinianPrayfx[];
extern const u8 Img_NinianPrayfx[];
extern const u8 SpritAnim_NinianPrayfx[];
extern const struct EventSpriteAnimConf EventSpriteAnimConf_NinianPray;
extern struct BmBgxConf BmBgfxConf_EventThunder[];

void CallMapAnim_HeavyGravity(ProcPtr parent, int x, int y);
bool MapAnimHeavyGravityExists(void);

void CallMapAnim_ThunderStorm(ProcPtr parent, int x, int y);
bool MapAnimThunderStormExists(void);

void StartNinianPrayfx(ProcPtr proc, int x, int y);

void CallMapAnim_HealExt(ProcPtr parent, struct Unit *unit, int heal, void (*callback1)(ProcPtr proc), void (*callback2)(ProcPtr proc));
void CallMapAnim_Heal(ProcPtr parent, struct Unit *unit, int heal);
bool MapAnimHealExists(void);

void CallMapAnim_HurtExt(ProcPtr parent, struct Unit *unit, int damage, void (*callback1)(ProcPtr proc), void (*callback2)(ProcPtr proc));
void CallMapAnim_Hurt(ProcPtr parent, struct Unit *unit, int damage);
bool MapAnimHurtExists(void);
