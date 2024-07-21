#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "constants/texts.h"

const struct MenuItemDef gSkillMenuInfos[MAX_SKILL_NUM + 1] = {
#if (defined(SID_Dance) && COMMON_SKILL_VALID(SID_Dance))
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
#endif

#if (defined(SID_Steal) && COMMON_SKILL_VALID(SID_Steal))
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
#endif

#if (defined(SID_Summon) && COMMON_SKILL_VALID(SID_Summon))
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
#endif

#if (defined(SID_LockTouch) && COMMON_SKILL_VALID(SID_LockTouch))
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
#endif

#if (defined(SID_HealingFocus) && COMMON_SKILL_VALID(SID_HealingFocus))
    [SID_HealingFocus] = {
        .name = "　瞑想",
        .nameMsgId = MSG_MenuSkill_HealingFocusName,
        .helpMsgId = MSG_SKILL_HealingFocus,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = HealingFocus_Usability,
        .onDraw = NULL,
        .onSelected = HealingFocus_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Teleportation) && COMMON_SKILL_VALID(SID_Teleportation))
    [SID_Teleportation] = {
        .name = "　転移",
        .nameMsgId = MSG_SkillEffect_Teleportation,
        .helpMsgId = MSG_MenuSkill__Teleportation_Rtext,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = Teleportation_Usability,
        .onDraw = NULL,
        .onSelected = Teleportation_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_LightRune) && COMMON_SKILL_VALID(SID_LightRune))
    [SID_LightRune] = {
        .name = "　光の結界",
        .nameMsgId = MSG_MenuSkill_LightRuneName,
        .helpMsgId = MSG_SKILL_LightRune,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = LightRune_Usability,
        .onDraw = NULL,
        .onSelected = LightRune_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_Mine) && COMMON_SKILL_VALID(SID_Mine))
    [SID_Mine] = {
        .name = "　地雷",
        .nameMsgId = MSG_MenuSkill_MineName,
        .helpMsgId = MSG_SKILL_Mine,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = Mine_Usability,
        .onDraw = NULL,
        .onSelected = Mine_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif
};
