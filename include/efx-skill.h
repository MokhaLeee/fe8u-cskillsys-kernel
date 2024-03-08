#pragma once

#include "global.h"
#include "proc.h"
#include "anime.h"

void NewEkrSkill(struct Anim * anim);
void NewEfxSkill(struct Anim * anim, int sid);
void NewEfxCombatArt(struct Anim * anim, int cid);
bool EfxSkillExists(void);

/* Efx skill box */
enum efx_skill_box_style {
    EFX_SKILL_BOX_SKILL = 0,
    EFX_SKILL_BOX_COMBATART = 1,
};

void NewEfxSkillBox(struct Anim * anim, int msg, const u8 * icon, u8 sid, int skill_or_combatart);
bool EfxSkillBoxExists(void);

/* Utils */
void InitEfxSkillRoundData(void);
void RegisterActorEfxSkill(int round, const u8 sid);
void RegisterTargetEfxSkill(int round, const u8 sid);
void RegisterEfxSkillCombatArt(int round, const u8 cid);
u8 GetActorEfxSkill(int round);
u8 GetTargetEfxSkill(int round);

extern AnimScr AnimScr_EfxSkillBoxIconRight[];
extern AnimScr AnimScr_EfxSkillBoxIconLeft[];
extern AnimScr AnimScr_EfxSkillBoxNameRight[];
extern AnimScr AnimScr_EfxSkillBoxNameLeft[];
extern const u8 Img_EfxSkillBoxBG[];
