#ifndef CONFIG_INSTALLED
#define CONFIG_INSTALLED

#include "config-debug.h"
#include "config-memmap.h"

/* Memmap */

/* If uncomment, use zh-cn language */
// #define CONFIG_LANG_CHINESE

/* Icon sheet amount */
#define ICON_SHEET_AMT 4

/* If comment, CHAX may not verify on FESKILL magic work in SRAM */
#define CONFIG_VERIFY_SKILLSYS_SRAM

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

#endif /* CONFIG_INSTALLED */
