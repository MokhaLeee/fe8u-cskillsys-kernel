#include "common-chax.h"
#include "kernel-lib.h"

const struct KernelDesigerConfig gKernelDesigerConfig = {
    .equip_skill_en = true,
    .max_equipable_skill = 7,

#ifdef CONFIG_REMOVE_MOVE_PATH
    .remove_move_path = true,
#endif

#ifdef CONFIG_USE_UTF8_GLYPH
    .use_chinese_character = true,
#endif

    .kernel_tutorial_level = CONFIG_KTUT_LEVEL,

#ifdef CONFIG_USE_COMBO_ATTACK
    .combo_attack_en = true,
#endif

#ifdef CONFIG_BATTLE_SURROUND
    .battle_surrend_en = true,
#endif

    .hit_decrease_on_range = true,

#ifdef CONFIG_DEBUG_UNIT_LOAD_SKILL
    .debug_autoload_skills = true,
#endif

    .guaranteed_lvup = false,
    .lvup_mode_tutorial = CONFIG_LVUP_MODE_TUTORIAL,
    .lvup_mode_normal   = CONFIG_LVUP_MODE_NORMAL,
    .lvup_mode_hard     = CONFIG_LVUP_MODE_HARD,

    .unit_page_style = CONFIG_PAGE1_WITH_LEADERSHIP,
    .skil_page_style = CONFIG_PAGE4_MOKHA_PLAN_A,
};
