#pragma once

#include "global.h"

struct EfxAnimConf {
    const s16 * frame_confs;
    u16 const * const * imgs;
    u16 const * const * tsas;
    u16 const * const * pals;
};

extern const struct EfxAnimConf EfxSkillVanilla, EfxSkillMokha, EfxSkillAether;
extern const struct EfxAnimConf EfxSkillCombat, EfxSkillDragonFang;
