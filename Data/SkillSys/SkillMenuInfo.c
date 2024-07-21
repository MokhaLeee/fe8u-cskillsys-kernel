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

#if (defined(SID_RallyDefense) && COMMON_SKILL_VALID(SID_RallyDefense))
    [SID_RallyDefense] = {
        .name = "　ラリー防御",
        .nameMsgId = MSG_MenuSkill_RallyDefenseName,
        .helpMsgId = MSG_SKILL_RallyDefense,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = RallyDefense_Usability,
        .onDraw = NULL,
        .onSelected = RallyDefense_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_RallyLuck) && COMMON_SKILL_VALID(SID_RallyLuck))
    [SID_RallyLuck] = {
        .name = "　ラリー運",
        .nameMsgId = MSG_MenuSkill_RallyLuckName,
        .helpMsgId = MSG_SKILL_RallyLuck,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = RallyLuck_Usability,
        .onDraw = NULL,
        .onSelected = RallyLuck_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_RallyMagic) && COMMON_SKILL_VALID(SID_RallyMagic))
    [SID_RallyMagic] = {
        .name = "　ラリーマジック",
        .nameMsgId = MSG_MenuSkill_RallyMagicName,
        .helpMsgId = MSG_SKILL_RallyMagic,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = RallyMagic_Usability,
        .onDraw = NULL,
        .onSelected = RallyMagic_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_RallyMovement) && COMMON_SKILL_VALID(SID_RallyMovement))
    [SID_RallyMovement] = {
        .name = "　ラリーの動き",
        .nameMsgId = MSG_MenuSkill_RallyMovementName,
        .helpMsgId = MSG_SKILL_RallyMovement,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = RallyMovement_Usability,
        .onDraw = NULL,
        .onSelected = RallyMovement_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_RallyResistance) && COMMON_SKILL_VALID(SID_RallyResistance))
    [SID_RallyResistance] = {
        .name = "　ラリー耐性",
        .nameMsgId = MSG_MenuSkill_RallyResistanceName,
        .helpMsgId = MSG_SKILL_RallyResistance,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = RallyResistance_Usability,
        .onDraw = NULL,
        .onSelected = RallyResistance_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_RallySkill) && COMMON_SKILL_VALID(SID_RallySkill))
    [SID_RallySkill] = {
        .name = "　ラリースキル",
        .nameMsgId = MSG_MenuSkill_RallySkillName,
        .helpMsgId = MSG_SKILL_RallySkill,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = RallySkill_Usability,
        .onDraw = NULL,
        .onSelected = RallySkill_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_RallySpeed) && COMMON_SKILL_VALID(SID_RallySpeed))
    [SID_RallySpeed] = {
        .name = "　ラリースピード",
        .nameMsgId = MSG_MenuSkill_RallySpeedName,
        .helpMsgId = MSG_SKILL_RallySpeed,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = RallySpeed_Usability,
        .onDraw = NULL,
        .onSelected = RallySpeed_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_RallyStrength) && COMMON_SKILL_VALID(SID_RallyStrength))
    [SID_RallyStrength] = {
        .name = "　ラリーパワー",
        .nameMsgId = MSG_MenuSkill_RallyStrengthName,
        .helpMsgId = MSG_SKILL_RallyStrength,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = RallyStrength_Usability,
        .onDraw = NULL,
        .onSelected = RallyStrength_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif

#if (defined(SID_RallySpectrum) && COMMON_SKILL_VALID(SID_RallySpectrum))
    [SID_RallySpectrum] = {
        .name = "　ラリースペクトラム",
        .nameMsgId = MSG_MenuSkill_RallySpectrumName,
        .helpMsgId = MSG_SKILL_RallySpectrum,
        .color = TEXT_COLOR_SYSTEM_WHITE,
        .isAvailable = RallySpectrum_Usability,
        .onDraw = NULL,
        .onSelected = RallySpectrum_OnSelected,
        .onIdle = NULL,
        .onSwitchIn = NULL,
        .onSwitchOut = NULL,
    },
#endif
};
