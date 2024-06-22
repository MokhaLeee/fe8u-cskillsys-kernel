#include "common-chax.h"
#include "efx-anim.h"
#include "constants/efx-skills.h"

struct EfxAnimConf const * const gEfxSkillAnims[0x100] = {
    [EFX_SKILL_DEFAULT] = &EfxSkillVanilla,
    [EFX_COMBAT_DEFAULT] = &EfxSkillCombat,
    [EFX_SKILL_MOKHA] = &EfxSkillMokha,
    [EFX_SKILL_AETHER] = &EfxSkillAether,
    [EFX_SKILL_DRAGON_FANG] = &EfxSkillDragonFang,
};
