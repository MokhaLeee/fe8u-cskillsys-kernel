#ifndef CONFIG_INSTALLED
#define CONFIG_INSTALLED

// #include "config-debug.h"
#include "config-memmap.h"

#define CONFIG_FE8_REWRITE

/**
 * If uncomment, kernel may remove vanilla dislogue text
 * so that we can save ~470KByte ROM space in DEMO.
 *
 * Note that enabling this config will make FEB think there are something wrong in
 * ROM and report warnning. But it will be okay since this will not actually
 * introduce any bug.
 *
 * If you're planning to develop a new game instead of making minor
 * changes on vanilla, it is recommended to uncomment this config to save space.
 */
#define CONFIG_CROP_VANILLA_MSG

/* If comment, CHAX may not verify on FESKILL magic work in SRAM */
#define CONFIG_VERIFY_SKILLSYS_SRAM

/* If uncomment, use utf-8 characters */
#define CONFIG_USE_UTF8_GLYPH

/* Engage style combo attack */
// #define CONFIG_USE_COMBO_ATTACK

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
// #define CONFIG_BATTLE_SURROUND

/**
 * 0: Not show any kernel tutorial
 * 1: Only show kernel tutorial in tutorial mode
 * 2: show kernel tutorial at anytime
 */
#define CONFIG_KTUT_LEVEL 0

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
 * If uncommented, enable Gaiden style magic system
 *
 * Sub:
 * MUST_BE_MAGIC: if set as 0, then all weapons will be okay (rather than magic wepaon only)
 * REQUIRES_WRANK: if set as 0, then unit can directly use the weapon regardless vanilla judgement
 * AI_EN: if set as 1, AI can also use gaiden B.Mag
 * EXT_CONF_EN: enbale gGaidenChaxConfigs
 */
// #define CONFIG_USE_GAIDEN_MAGIC

#ifdef CONFIG_USE_GAIDEN_MAGIC
	#define CONFIG_GAIDEN_MAGIC_MUST_BE_MAGIC 0 // 0: all weapons are okay
	#define CONFIG_GAIDEN_MAGIC_REQUIRES_WRANK 0 // 0: not requires unit ability to equip that weapon
	#define CONFIG_GAIDEN_MAGIC_AI_EN 1
	#define CONFIG_GAIDEN_EXT_CONF_EN 1
#endif /* USE_GAIDEN_MAGIC */

/**
 * Install FEB patch: SetHPClassLimit
 */
#define CONFIG_UNLOCK_ALLY_MHP_LIMIT

/**
 * Enable shield system
 */
// #define CONFIG_INSTALL_KERNEL_SHIELD

/**
 * By: Vesly
 * Set a number of frames to hold the targetting cursor for (default is 45), 60 frames = 1 second
 * Installed in: Wizardry->Common->UnitTarget->TargetCursor->TargetCursor.event
 */
#define CONFIG_TARGETTING_CURSOR_FRAMES

/**
 * By: Vesly
 * Set a number of frames for some battle anims (defaults are 30 30 30 10 30), 60 frames = 1 second
 * Installed in: Wizardry->Common->MapAnimBattleHook->MapAnimBattleFrames->MapAnimBattleFrames.event
 */
#define CONFIG_MAP_BATTLE_ANIM_FRAMES

/**
 * By: Contro
 * Send a user's inventory to the supply when they die
 * Installed in: Wizardry->External->SendInventoryOnDeath->Installer.event
 */
#define CONFIG_INSTALL_SEND_INVENTORY_ON_DEATH

/**
 * By: Contro
 * Display the hidden RES stat of some map tiles like thrones
 * Installed in: Wizardry->External->ResTerrainWindow->Installer.event
 */
#define CONFIG_INSTALL_RES_TERRAIN_WINDOW


/**
 * By Mokha:
 * Allow units to perform all their regular actions after trading
 */
// #define CONFIG_INSTALL_NO_WAIT_AFTER_TRADING

/**
 * By Vesly:
 * Speed up your rom hack testing with this tool kit of
 * menu options that lets you test everything from promotions to
 * chapter warps to arena battles all from the start.
 */
#define CONFIG_INSTALL_VESLY_DEBUGGER

/**
 * DO NOT ENABLE
 * Increase the caps of the stat bars for units
 * IN_PROGRESS - currently need to integrate https://github.com/FireEmblemUniverse/fireemblem8u/blob/12004dfd31c8f860804b811fc110394747dbcd12/src/data_classes.c#L1
 * to preprocess new stat caps for each class
 */
// #define CONFIG_INSTALL_EXTENDED_STAT_BARS

/**
 * Set the widths of the various popup menus (in pixels)
 * Popup widths defined in: Wizardry/External/PopupMenuWidths/PopupMenuWidths.h
 */
#define CONFIG_INSTALL_MENU_WIDTHS

/**
 * By Tequila:
 * Reworks the text engine to allow, among other things;
 * 3 line text boxes, multiple colors, variable text,
 * speed, flipped portraits and more!
 * 
 * Now has NarrowFont from Scraiza integrated thanks to Jester, usable via [Font_NarrowTalk] and [Font_NarrowMenu]
 * 
 * Installed in: Wizardry/External/TextEngineRework/Installer.event
 * Source: https://feuniverse.us/t/teqs-minor-assembly-shenanigans/1655/118
 */
// #define CONFIG_TEXT_ENGINE_REWORK

/**
 * By Jester
 * When two units can have a talk event, show a talk icon on the map
 * Keep track of WarningHpSheet.png that has the icons
 * 
 * WARNING - This causes a lot of lag due to having to run GetTalkee for
 * every unit on the map
 */
// #define CONFIG_TALK_MAP_ICON

/**
 * By Huichelaar
 * Add an Augury like FE7 has to give you special information
 * about the current chapter. Currently it doesn't cost anything
 * look into changing that
 * 
 * Installed in: Wizardry/External/Augury/Installer.event
 * Hooked into Wiardry/Core/SkillSys/PrepSkill/Source/AtMenu.c/AtMenu_StartSubMenu()
 * 
 * Source: https://feuniverse.us/t/huichelaars-assembly-adventures/8229
 */
#define CONFIG_INSTALL_PREPS_AUGURY

/**
 * By Tequila
 * Show heal amount for target units in mini box when using a staff
 * Installed in: Wizardry/External/ShowHealAmount/Installer.event
 * Source: https://feuniverse.us/t/teqs-minor-assembly-shenanigans/1655/22
 */
#define CONFIG_SHOW_HEAL_AMOUNT

/**
 * Decomped by Mokha 
 * Ported to FE8 by Eebit
 * Made into a handy C macro by Jester
 * Allows the use of the Quintessance steal effect as an event in FE8
 * Installed in: Wizardry/External/QuintessanceEffect/Installer.event
 * Macro in: include/jester_headers/macro.h
 */
#define CONFIG_QUINTESSANCE_EFFECT

/**
 * By Snakey1
 * Allows summoners to gain EXP from their summon's battles
 * Installed in: Wizardry/External/SummonerEXP/Installer.event
 * Source: https://feuniverse.us/t/snakes-pit-of-asm-and-other-stuff/3627/45
 * 
 * W-A-R-N-I-N-G : This uses a lyn event file for the compiled ASM, so it'll trigger
 * errors in make about a missing C file (as make expects to produce lyn files from C files)
 * The solution would be to rewrite SummonEXPMaster.asm in C, but until then you can ignores these errors
 */
// #define CONFIG_SUMMON_EXP

/**
 * By Contro
 * 
 * Shows different palette colors for stat screens depending on allegiance
 * 
 * Installed in: Wizardry/External/StatScreenAllegianceColors/Installer.event
 * Source: https://feuniverse.us/t/contros-code-collection/13514/12
 */

#define CONFIG_STAT_SCREEN_ALLEGIANCE_COLORS


/**
 * By Vesly
 * Source: https://github.com/Veslyquix/ASM/tree/main/SupportPostBattle
 */
#define CONFIG_VESLY_SUPPORT_POST_BATTLE

#ifdef CONFIG_VESLY_SUPPORT_POST_BATTLE
	#define SUPPORT_RATE_KILL 100
	#define SUPPORT_RATE_COMBAT 100
	#define SUPPORT_RATE_STAFF 100
	#define SUPPORT_RATE_DANCE 100
#endif

/**
 * By Contro
 * Allows user to flip sprites by allegiance (would be nice to add class/character options too)
 * Installed in: Wizardry/External/MapMirrorSprites/Installer.event
 * Source: https://feuniverse.us/t/contros-code-collection/13514/42
 */
// #define CONFIG_FLIPPED_SPRITES


/**
 * By Jester
 * Installed in: Wizardry/Core/SkillSys/MiscSkillEffects/MapSkills/MapSkills.c
 */
// #define CONFIG_AUTO_REPAIR_WEAPONS

// By Jester
#define CONFIG_STAT_SCREEN_TERRAIN_BONUS

// By Tequila
// #define CONFIG_INSTANT_LEVEL_UP

/* 
** By Jester
** Apparently this is a thing in the FE8J skill system
** Having an S rank in a wepaon type will mean you can ignore weapon weight
** when using weapons of that type
*/
#define CONFIG_S_RANK_NO_WEAPON_WEIGHT

/*
** By Vesly
** Takes over the Juniper Fruit to introduced in Fire Emblem - Fates
** that lets a unit change their class without resetting their level.
*/
// #define CONFIG_VESLY_RECLASS

/*
** By Jester 
** Originally by Vesly, but this has been semi ported to C
** Allows for up to 5 lines of text in a helpbox
** Currently it causes graphical glitches during the following:
** - When trading on a map,
** - When the subtitle field is shown during targetting
** - When seleecting the R button when looking at the map objectives
** - When looking at the supply list
** - When selecting the R button when looking at the use menu in the manage units panel
*/
// #define CONFIG_EXTENDED_HELPBOXES

/*
** By Stan
** This introduces a dynamic turn flow where control passes between
** different factions after each unit moves.
** Source: https://feuniverse.us/t/fe8u-stans-asm-stuff/2376/19
**
*/
// #define CONFIG_LAZBERIAN_TURN_FLOW

/* 
** By Contro
** This introduces a limitation on items you can
** buy in a shop.
**
** Source: https://feuniverse.us/t/contros-code-collection/13514/20
*/
// #define CONFIG_LIMITED_SHOP_STOCK

/*
** By Zeta
** Can set level and turn limits on the arena and
** make it so participation isn't lethal
**
** Source: https://feuniverse.us/t/zetas-asm-c-spellbook/4824
*/
// #define CONFIG_ARENA_LIMITS

/*
** By Huichelaar
** Uses the fog of war palette for a map to generate a map
** wide radius of reachable tiles by all enemy units.
** The palette for fog of war for a map tileset can be changed
** with the python tools bundled with the hack. Read the README for details
**
** Source: https://feuniverse.us/t/huichelaars-assembly-adventures/8229
*/
// #define CONFIG_DANGER_RADIUS

#define CONFIG_TELLIUS_CAPACITY_SYSTEM
#define CONFIG_TELLIUS_CAPACITY_BASE 50
#define CONFIG_TELLIUS_CAPACITY_PROMOTED 25

/*
** By Jester
*/
#define CONFIG_TALK_LEVEL_UP

/* 
** By Jester
*/
// #define CONFIG_RESTORE_HP_ON_LEVEL_UP

/*
** By Jester
*/
// #define CONFIG_DEATH_DANCE

/*
** By Jester
*/
// #define CONFIG_INFINITE_DURABILITY

/*
** By Jester
** NOT STARTED
*/
// #define CONFIG_FORGE

/*
** By Jester
** NOT WORKING, BEXP saves on suspend but not on power off and seems to be shared between all save files.
*/
// #define CONFIG_BEXP 1

/*
** By: Circleseverywhere
*/
#define CONFIG_CHAPTER_NAMES_AS_TEXT


/*
** By: Jester
** Use BWL stats as counters for additional skills
*/
#define CONFIG_RESET_BWL_STATS_EACH_CHAPTER


/**
 * Levelup mode
 * 0: vanilla
 * 1: uncontrollable
 * 2: fixed
 * 3: 100%
 * 4: 0%
 */
#define CONFIG_LVUP_MODE_TUTORIAL 0
#define CONFIG_LVUP_MODE_NORMAL 0
#define CONFIG_LVUP_MODE_HARD 0

/**
 * Page 1 styles
 */
#define CONFIG_PAGE1_WITH_BWL 1
#define CONFIG_PAGE1_WITH_LEADERSHIP 0

/**
 * Page 3 styles
 * 
 * Right now, there's just two styles (0 and 1)
 */
#define CONFIG_PAGE3_SKILL_LAYOUT 1

/**
 * Install some performance hooks
 */
#define CONFIG_PERFORMANCE_OPTIMIZATION

/**
 * Install DrawMapActionAnimation
 */
// #define CONFIG_INSTALL_MAPACTIONANIM

/**
 * Install FE6_STAT_SCREEN
 */
// #define CONFIG_INSTALL_FE6_STAT_SCREEN

/**
 * Install Bow2Decrease
 */
// #define CONFIG_INSTALL_BOW2DECREASE

// #define CONFIG_INSTALL_DECREASE_HIT_AT_RANGE

#define CONFIG_VESLY_AOE


/* 
This is my sorry ass attempt at fixing a problem with the arena that
has made enemy stats ballon to broken levels. Unfortunately, the base
arena formula is so fucking out of wack that even with these fixes
promoted enemies become a joke, and unpromoted are still fairly strong
but at least the arena is playable now
*/
#define CONFIG_ARENA_PARTIAL_FIX

/**
 * By Jester
 * If an enemy kills a unit and they haven't promoted already,
 * then promote them. Sort of like Shiren the Wanderer.
 */
#define CONFIG_PROMOTE_ENEMIES_IF_KILLED_UNIT

/**
 * By Jester:
 * Allow units to rescue themselves into allies following the standard CON < AID formula
 */
#define CONFIG_REFUGE_FEATURE

#define CONFIG_ESCAPE_EVENT

/**
 * By Jester
 * Tellius mechanic: Automatically display the pormotion screen for a character that reaches their max level
 */
/* BROKEN - DO NOT USE */
// #define CONFIG_AUTO_PROMOTION_ON_MAX_LEVEL

/**
 * Install ConvoyExpa
 */
#define CONFIG_INSTALL_CONVOYEXPA
#define CONFIG_INSTALL_CONVOYEXPA_AMT 200 // better not to touch this since it will affact on size of gExpaConvoyItemArray

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
#define CONFIG_ICON_INDEX_SKILL_STEALER 0xCC
#define CONFIG_ICON_INDEX_ARMS_SCROLL 0xCD

/**
 * Item config
 */
#define CONFIG_PR_ITEM_TABLE 0x809B10
#define CONFIG_ITEM_INDEX_MAG_BOOSTER 0xBC
#define CONFIG_ITEM_INDEX_SKILL_SCROLL 0xBD
#define CONFIG_ITEM_INDEX_SKILL_STEALER 0xBE
#define CONFIG_ITEM_INDEX_ARMS_SCROLL 0xBF

/**
 * Unit amount, since it is hard to modify, it is recommanded not change this value
 */
#define CONFIG_UNIT_AMT_ALLY  51
#define CONFIG_UNIT_AMT_ENEMY 50
#define CONFIG_UNIT_AMT_NPC   8

/**
 * Make skill names narrow by default
 */
#define CONFIG_AUTO_NARROW_FONT

/**
 * FE8 Rewritten exclusive configs
 */
#define SETH_INJURED
#define INJURED_TURN_COUNT 5

#endif /* CONFIG_INSTALLED */
