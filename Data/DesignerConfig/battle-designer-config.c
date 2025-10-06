#include <common-chax.h>
#include <kernel-lib.h>

const struct KernelBattleDesignerConfig gKernelBattleDesignerConfig = {
	.as_calc_atk_perc = 20,
	.nosferatu_hpdrain_perc = 50,
	.rider_debuff_indoor = 20,
	.crit_correction = 200,
	.critical_rate_bonus_cavalry = -10,
	.critical_rate_bonus_attr = 10,

	.combo_base_damage = 5,
	.combo_additional_damage_en = false,
	.combo_additional_damage_perc = 30,

	.battle_followup_speed_threshold = BATTLE_FOLLOWUP_SPEED_THRESHOLD,
};
