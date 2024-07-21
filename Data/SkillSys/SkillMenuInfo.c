#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

const struct MenuItemDef gSkillMenuInfos[MAX_SKILL_NUM + 1] = {
    [SID_Dance] = {
        .name = "　踊る",
        .nameMsgId = 0x67E,
        .helpMsgId = 0x6C2,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = DanceCommandUsability,
        .onDraw = NULL,
        .onSelected = PlayCommandEffect,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },

    [SID_Steal] = {
        .name = "　盗む",
        .nameMsgId = 0x67F,
        .helpMsgId = 0x6C4,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = StealCommandUsability,
        .onDraw = NULL,
        .onSelected = StealCommandEffect,
        .onIdle = 0,
        .onSwitchIn = 0,
        .onSwitchOut = 0
    },

    [SID_Summon] = {
        .name = "　召喚",
        .nameMsgId = 0x693,
        .helpMsgId = 0x6DD,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = SummonCommandUsability,
        .onDraw = 0,
        .onSelected = SummonCommandEffect,
        .onIdle = 0,
        .onSwitchIn = 0,
        .onSwitchOut = 0
    },

    [SID_LockTouch] = {
        .name = "　かぎ開",
        .nameMsgId = 0x694,
        .helpMsgId = 0x6DE,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = PickCommandUsability,
        .onDraw = NULL,
        .onSelected = PickCommandEffect,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },

    [SID_HealingFocus] = {
        .name = "　瞑想",
        .nameMsgId = MSG_SkillEffect_HealingFocus,
        .helpMsgId = MSG_SKILL_HealingFocus,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = HealingFocus_Usability,
        .onDraw = NULL,
        .onSelected = HealingFocus_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },

    [SID_Teleportation] = {
        .name = "　転移",
        .nameMsgId = MSG_SkillEffect_Teleportation,
        .helpMsgId = MSG_SKILL_Teleportation,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = Teleportation_Usability,
        .onDraw = NULL,
        .onSelected = Teleportation_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
};
