#ifndef CONFIG_INSTALLED
#define CONFIG_INSTALLED

#include "config-debug.h"
#include "config-memmap.h"

/**
 * If uncomment, kernel may remove vanilla dislogue text
 * so that we can save ~470KByte ROM space in DEMO.
 *
 * Note that enabling this config will make FEB think there are something wrong in
 * ROM and report warnning. But it will be okay since this will not actually
 * introduce any bug.
 *
 * If you're planning to develop a new game instead of making minor
 * changes on vanilla, it is recommanded to uncomment this config to save space.
 */
// #define CONFIG_CROP_VANILLA_MSG

/* CommonProtection.event */
#define CONFIG_COMMON_PROTECTION_ENBALED

/* If comment, CHAX may not verify on FESKILL magic work in SRAM */
#define CONFIG_VERIFY_SKILLSYS_SRAM

/* If uncomment, use utf-8 characters */
#define CONFIG_USE_UTF8_GLYPH

/* Engage style combo attack */
#define CONFIG_USE_COMBO_ATTACK

/**
 * Surround:
 * When a unit is attacked and adjacent to the enemy,
 * each side with enemy may cause unit avoid -10%
 *
 * If completely surrounded (each side stands an enemy),
 * unit may cause def-5 additionally.
 *
 * Flyer in outdoor environments are not affected by this effect
 */
#define CONFIG_BATTLE_SURROUND

/**
 * 0: Not show any kernel tutorial
 * 1: Only show kernel tutorial in tutorial mode
 * 2: show kernel tutorial at anytime
 */
#define CONFIG_KTUT_LEVEL 2

/**
 * If uncommented, enable patch
 * FEB: Character Custom Animation
 */
#define CONFIG_USE_CHAR_CUSTOM_ANIM

/**
 * If uncommented, fasten function GenerateUnitCompleteAttackRange
 * with lower accuracy.
 */
#define CONFIG_FASTER_MAP_RANGE

/**
 * Hook on function: LoadUnit() to
 * optimize performance to avoid minor hitches when loading unit
 */
#define CONFIG_OPTIMIZE_UNIT_AUTOLEVEL

/**
 * Try narrow font on skill-name/skill-desc/menu-disp
 */
#define CONFIG_AUTO_NARROW_FONT

/**
 * Inject fake FEB old AnimNumber patch hooks.
 * This config may cause two functions suffer from ugly ASM hooks:
 *      - StartBattleAnimHitEffect
 *      - NewEfxHpBar
 */
#define CONFIG_FAKE_OLD_ANIMNUMBER_PATCH

/**
 * If uncommented, enable Gaiden style magic system
 *
 * Sub:
 * MUST_BE_MAGIC: if set as 0, then all weapons will be okay (rather than magic wepaon only)
 * REQUIRES_WRANK: if set as 0, then unit can directly use the weapon regardless vanilla judgement
 * AI_EN: if set as 1, AI can also use gaiden B.Mag
 * EXT_CONF_EN: enbale gGaidenChaxConfigs
 */
#define CONFIG_USE_GAIDEN_MAGIC

#ifdef CONFIG_USE_GAIDEN_MAGIC
	#define CONFIG_GAIDEN_MAGIC_MUST_BE_MAGIC 0 // 0: all weapons are okay
	#define CONFIG_GAIDEN_MAGIC_REQUIRES_WRANK 0 // 0: not requires unit ability to equip that weapon
	#define CONFIG_GAIDEN_MAGIC_AI_EN 1
	#define CONFIG_GAIDEN_EXT_CONF_EN 1
#endif /* USE_GAIDEN_MAGIC */

/**
 * Enable leader-ship features
 */
// #define CONFIG_LEADER_SHIP_EN

/**
 * If uncommented, this will disable move-path calculation and display.
 * Since move-path can display no more than 20 steps in vanilla,
 * with the increase of unit's mobility, this limitation may cause
 * risk of overflow error. Thus we directly remove the move-path to avoid overflow.
 */
// #define CONFIG_REMOVE_MOVE_PATH

/**
 * Install FEB patch: SetHPClassLimit
 */
#define CONFIG_UNLOCK_ALLY_MHP_LIMIT

/**
 * Append the same logic on loading skill as old skillsys, as:
 * https://feuniverse.us/t/the-skill-system-and-you-maximizing-your-usage-of-fe8s-most-prolific-bundle-of-wizardry/8232/5
 */
#define CONFIG_FIT_OLD_SKILLSYS_LIST

/**
 * Enable shield system
 */
#define CONFIG_INSTALL_KERNEL_SHIELD

/**
 * Unit menu skill item displayed in upper menu or standalone sub-menu.
 */
#define CONFIG_MENU_SKILL_NOT_IN_UPPER

/**
 * Levelup mode
 * 0: vanilla
 * 1: uncontrollable
 * 2: fixed
 * 3: 100%
 * 4: 0%
 */
#define CONFIG_LVUP_MODE_TUTORIAL 2
#define CONFIG_LVUP_MODE_NORMAL 0
#define CONFIG_LVUP_MODE_HARD 1

/**
 * Page1 styles
 */
#define CONFIG_PAGE1_WITH_BWL 0
#define CONFIG_PAGE1_WITH_LEADERSHIP 1

/**
 * Page4 styles
 */
#define CONFIG_PAGE4_MOKHA_PLAN_A 0
#define CONFIG_PAGE4_MOKHA_PLAN_B 1

/**
 * Install some performance hooks
 */
#define CONFIG_PERFORMANCE_OPTIMIZATION

/**
 * Install DrawMapActionAnimation
 */
// #define CONFIG_INSTALL_MAPACTIONANIM

/**
 * Install StatScreenfx
 */
// #define CONFIG_INSTALL_STATSCREENFX

/**
 * Install Bow2Decrease
 */
#define CONFIG_INSTALL_BOW2DECREASE

/**
 * Install none-kernel related patches (not necessary)
 */
#define CONFIG_INSTALL_NON_KERNEL_PATCH

/**
 * Install ConvoyExpa
 */
#define CONFIG_INSTALL_CONVOYEXPA
#define CONFIG_INSTALL_CONVOYEXPA_AMT 200 // better not to touch this since it will affact on size of gExpaConvoyItemArray

/**
 * If uncommented, suspend data may only saved in play-phase starting.
 * This may also overwrite PlaySt::disableTerrainDisplay config for player configuration in runtime.
 */
#define CONFIG_NO_SUS_IN_AI_PHASE

/**
 * If you press ↑↑↓↓←→←→BA at titlescreen, then...
 */
#define CONFIG_USE_KONAMI_CODE_BONUS

/**
 * Ai action expansion
 */
#define CONFIG_AI_ACTION_AMT 20
#define CONFIG_AI_ACTION_EXPA_Teleportation 14

/**
 * Unit action expansion
 */
#define CONFIG_UNIT_ACTION_AMT 0x30
#define CONFIG_UNIT_ACTION_EXPA_ExecSkill 0x23
#define CONFIG_UNIT_ACTION_EXPA_GaidenMagicCombat 0x24
#define CONFIG_UNIT_ACTION_EXPA_GaidenMagicStaff 0x25

/**
 * Icon config
 */
#define CONFIG_PR_ITEM_ICON  0x5926F4
#define CONFIG_ICON_INDEX_MAG_BOOSTER 0xCA
#define CONFIG_ICON_INDEX_STAR 0xCB

/**
 * Item config
 */
#define CONFIG_PR_ITEM_TABLE 0x809B10
#define CONFIG_ITEM_INDEX_MAG_BOOSTER 0xBC
#define CONFIG_ITEM_INDEX_SKILL_SCROLL 0xBD

/**
 * Unit amount, since it is hard to modify, it is recommanded not change this value
 */
#define CONFIG_UNIT_AMT_ALLY  51
#define CONFIG_UNIT_AMT_ENEMY 50
#define CONFIG_UNIT_AMT_NPC   8

#endif /* CONFIG_INSTALLED */
