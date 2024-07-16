#include "common-chax.h"
#include "kernel-lib.h"

const struct KernelDesigerConfig gKernelDesigerConfig = {
    .equip_skill_en = true,
    .max_equipable_skill = 7,

#ifdef CONFIG_REMOVE_MOVE_PATH
    .remove_move_path = true,
#endif

#ifdef CONFIG_USE_COMBO_ATTACK
    .combo_attack_en = true,
#endif

#ifdef CONFIG_REMOVE_MOVE_PATH
    .use_chinese_character = true,
#endif
};
