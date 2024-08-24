#pragma once

#include "common-chax.h"

struct EfxAnimConf {
    const s16 * frame_confs;
    u16 const * const * imgs;
    u16 const * const * tsas;
    u16 const * const * pals;
    u16 sfx;
    u8 _pad_[2];
};

extern const struct EfxAnimConf EfxSkillVanilla, EfxSkillCombat;
