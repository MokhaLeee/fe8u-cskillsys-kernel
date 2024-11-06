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

/* If comment, CHAX may not verify on FESKILL magic work in SRAM */
#define CONFIG_VERIFY_SKILLSYS_SRAM

/* If uncomment, use utf-8 characters */
// #define CONFIG_USE_UTF8_GLYPH

/* Engage style combo attack */
/* #define CONFIG_USE_COMBO_ATTACK */

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
 * Install FEB patch: SetHPClassLimit
 */
#define CONFIG_UNLOCK_ALLY_MHP_LIMIT

/**
 * By: Vesly
 * Allow another line for all R button text
 * Installed in: Wizardry->Common->ItemSys->ItemDesc->ItemDesc.event
 * 
 * CURRENTLY BROKEN as I had to disable line 69/70/71 in order for Vesly's debugger to work
 */
// #define CONFIG_EXTEND_R_TEXT_DESC

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
#define CONFIG_INSTALL_NO_WAIT_AFTER_TRADING

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
 * By Snakey1:
 * Allow units to have base conversations Tellius style
 * IN PROGRESS - Requires input from creator, so do not enable yet
 */
//#define CONFIG_INSTALL_BASE_CONVERSATIONS

/**
 * By Tequila:
 * Reworks the text engine to allow, among other things;
 * 3 line text boxes, multiple colors, variable text,
 * speed, flipped portraits and more!
 * 
 * Now has NarrowFont from Scraiza integrated thanks to Jester, usable via [Font_NarrowText] and [Font_NarrowMenu]
 * 
 * Installed in: Wizardry/External/TextEngineRework/Installer.event
 * Source: https://feuniverse.us/t/teqs-minor-assembly-shenanigans/1655/118
 */
#define CONFIG_TEXT_ENGINE_REWORK

/**
 * By Jester
 * When two units can have a talk event, show a talk icon on the map
 * Keep track of WarningHpSheet.png that has the icons
 */
#define CONFIG_TALK_MAP_ICON

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
 * By Contro
 * Allows user to flip sprites by allegiance (would be nice to add class/character options too)
 * Installed in: Wizardry/External/MapMirrorSprites/Installer.event
 * Source: https://feuniverse.us/t/contros-code-collection/13514/42
 */
// #define CONFIG_FLIPPED_SPRITES


/**
 * By Zane | GORGON-EGG
 * Allows users to modify the minimug box in the top left corner when hovering over a unit
 * Installed in: Wizardry/External/GorgonEgg/output/Installer.event
 * Source: https://feuniverse.us/t/gorgon-egg/25690
 * 
 * W-A-R-N-I-N-G - Worked the first time I compiled now it's broken as well and I don't have
 * the energy to fix it at the time of writing
 */
// #define CONFIG_MODULAR_STAT_SCREEN_GORGON_EGG

/**
 * By Jester
 * Installed in: Wizardry/Core/SkillSys/MiscSkillEffects/MapSkills/MapSkills.c
 */
// #define CONFIG_AUTO_REPAIR_WEAPONS

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
 * Page1 styles
 */
#define CONFIG_PAGE1_WITH_BWL 0
#define CONFIG_PAGE1_WITH_LEADERSHIP 1

/**
 * Page4 styles
 */
#define CONFIG_PAGE4_MOKHA_PLAN_A 1
#define CONFIG_PAGE4_MOKHA_PLAN_B 0

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
