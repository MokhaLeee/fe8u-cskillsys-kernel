#include "common-chax.h"
#include "skill-system.h"

const struct SkillMiscConf gSkillMiscConf = {
    .Bonus_Fury = 3,
    .Bonus_FuryPlus = 4,
};

const u8 gStatusConf_DefiantSkills[SKILL_US_MAX] = {
    [SKILL_US_POW] = 7,
    [SKILL_US_MAG] = 7,
    [SKILL_US_SKL] = 7,
    [SKILL_US_SPD] = 7,
    [SKILL_US_LCK] = 7,
    [SKILL_US_DEF] = 7,
    [SKILL_US_RES] = 7,
};

const u8 gStatusConf_BonusSkills[SKILL_US_MAX] = {
    [SKILL_US_POW] = 2,
    [SKILL_US_MAG] = 2,
    [SKILL_US_SKL] = 2,
    [SKILL_US_SPD] = 2,
    [SKILL_US_LCK] = 2,
    [SKILL_US_DEF] = 2,
    [SKILL_US_RES] = 2,
};

const u8 gStatusConf_luckySevenSkill[SKILL_US_MAX] = {
    [SKILL_US_POW] = 7,
    [SKILL_US_MAG] = 7,
    [SKILL_US_SKL] = 7,
    [SKILL_US_SPD] = 7,
    [SKILL_US_LCK] = 7,
    [SKILL_US_DEF] = 7,
    [SKILL_US_RES] = 7,
};

const u8 gStatusConf_PushSkills[SKILL_US_MAX] = {
    [SKILL_US_POW] = 5,
    [SKILL_US_MAG] = 5,
    [SKILL_US_SKL] = 5,
    [SKILL_US_SPD] = 5,
    [SKILL_US_LCK] = 5,
    [SKILL_US_DEF] = 5,
    [SKILL_US_RES] = 5,
    [SKILL_US_MOV] = 5,
};
