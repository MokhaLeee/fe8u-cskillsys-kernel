#include "common-chax.h"
#include "debug-event.h"
#include "skill-system.h"
#include "event-rework.h"
#include "constants/skills.h"

/**
 * Udefs
 */
static const struct REDA REDA_Eirika[] = {
	{
		.x = 14,
		.y = 27,
		.b = -1,
	}
};

static const struct REDA REDA_Seth[] = {
	{
		.x = 15,
		.y = 27,
		.b = -1,
		.delayFrames = 8,
	}
};

static const struct REDA REDA_Myrrh[] = {
	{
		.x = 13,
		.y = 25,
		.b = -1,
		.delayFrames = 16,
	}
};

static const struct REDA REDA_Tana[] = {
	{
		.x = 16,
		.y = 21,
		.b = -1,
		.delayFrames = 32,
	}
};

static const struct REDA REDA_Vanessa[] = {
	{
		.x = 17,
		.y = 19,
		.b = -1,
		.delayFrames = 48,
	}
};

static const struct REDA REDA_Marisa[] = {
	{
		.x = 14,
		.y = 24,
		.b = -1,
		.delayFrames = 48,
	}
};

static const struct REDA REDA_Ephraim[] = {
	{
		.x = 13,
		.y = 27,
		.b = -1,
	}
};

static const struct REDA REDA_Saleh[] = {
	{
		.x = 14,
		.y = 26,
		.b = -1,
		.delayFrames = 64,
	}
};

static const struct REDA REDA_Tethys[] = {
	{
		.x = 12,
		.y = 27,
		.b = -1,
		.delayFrames = 64,
	}
};

static const struct REDA REDA_Knoll[] = {
	{
		.x = 17,
		.y = 28,
		.b = -1,
		.delayFrames = 64,
	}
};

static const struct UnitDefinition UnitDef_Ally1[] = {
	{
		.charIndex = CHARACTER_EIRIKA,
		.classIndex = CLASS_EIRIKA_LORD,
		.allegiance = FACTION_ID_BLUE,
		.autolevel = true,
		.level = 10,
		.xPosition = 16,
		.yPosition = 29,
		.redaCount = 1,
		.redas = REDA_Eirika,
		.items = {
			ITEM_SWORD_RAPIER,
			ITEM_VULNERARY,
		},
	},
	{
		.charIndex = CHARACTER_SETH,
		.classIndex = CLASS_PALADIN,
		.autolevel = true,
		.level = 10,
		.xPosition = 16,
		.yPosition = 29,
		.redaCount = 1,
		.redas = REDA_Seth,
		.items = {
			ITEM_SWORD_SILVER,
			ITEM_LANCE_SILVER,
			ITEM_ELIXIR
		},
	},
	{
		.charIndex = CHARACTER_MYRRH,
		.classIndex = CLASS_MANAKETE_MYRRH,
		.allegiance = FACTION_ID_BLUE,
		.autolevel = true,
		.level = 15,
		.xPosition = 16,
		.yPosition = 29,
		.redaCount = 1,
		.redas = REDA_Myrrh,
		.items = {
			ITEM_DIVINESTONE,
			ITEM_VULNERARY,
		},
	},
	{
		.charIndex = CHARACTER_TANA,
		.classIndex = CLASS_PEGASUS_KNIGHT,
		.autolevel = true,
		.level = 10,
		.xPosition = 19,
		.yPosition = 15,
		.redaCount = 1,
		.redas = REDA_Tana,
		.items = {
			ITEM_LANCE_SLIM,
			ITEM_VULNERARY
		},
	},
	{
		.charIndex = CHARACTER_VANESSA,
		.classIndex = CLASS_PEGASUS_KNIGHT,
		.autolevel = true,
		.level = 10,
		.xPosition = 19,
		.yPosition = 15,
		.redaCount = 1,
		.redas = REDA_Vanessa,
		.items = {
			ITEM_LANCE_SLIM,
			ITEM_VULNERARY
		},
	},
	{
		.charIndex = CHARACTER_MARISA,
		.classIndex = CLASS_MYRMIDON_F,
		.autolevel = true,
		.level = 10,
		.xPosition = 16,
		.yPosition = 29,
		.redaCount = 1,
		.redas = REDA_Marisa,
		.items = {
			ITEM_SWORD_SLIM,
			CONFIG_ITEM_INDEX_MAG_BOOSTER
		},
	},
	{
		.charIndex = CHARACTER_EPHRAIM,
		.classIndex = CLASS_EPHRAIM_LORD,
		.autolevel = true,
		.level = 10,
		.xPosition = 16,
		.yPosition = 29,
		.redaCount = 1,
		.redas = REDA_Ephraim,
		.items = {
			ITEM_LANCE_IRON,
			ITEM_VULNERARY
		},
	},
	{
		.charIndex = CHARACTER_SALEH,
		.classIndex = CLASS_SAGE,
		.autolevel = true,
		.level = 10,
		.xPosition = 11,
		.yPosition = 29,
		.redaCount = 1,
		.redas = REDA_Saleh,
		.items = {
			ITEM_ANIMA_BOLTING,
			ITEM_ANIMA_ELFIRE,
			ITEM_VULNERARY
		},
	},
	{
		.charIndex = CHARACTER_TETHYS,
		.classIndex = CLASS_DANCER,
		.autolevel = true,
		.level = 10,
		.xPosition = 12,
		.yPosition = 29,
		.redaCount = 1,
		.redas = REDA_Tethys,
		.items = {
			ITEM_VULNERARY
		},
	},
	{
		.charIndex = CHARACTER_KNOLL,
		.classIndex = CLASS_SUMMONER,
		.autolevel = true,
		.level = 10,
		.xPosition = 12,
		.yPosition = 29,
		.redaCount = 1,
		.redas = REDA_Knoll,
		.items = {
			ITEM_DARK_FLUX,
			ITEM_VULNERARY
		},
	},
	{}
};

static const struct REDA REDA_ONeill[] = {
	{
		.x = 16,
		.y = 19,
		.b = -1,
	}
};

static const struct REDA REDA_Saar[] = {
	{
		.x = 10,
		.y = 25,
		.b = -1,
	}
};

static const struct REDA REDA_Bazba[] = {
	{
		.x = 8,
		.y = 7,
		.b = -1,
	}
};

static const struct REDA REDA_Vigarde[] = {
	{
		.x = 13,
		.y = 23,
		.b = -1,
	},
};

static const struct UnitDefinition UnitDef_Enemy1[] = {
	{
		.charIndex = CHARACTER_ONEILL,
		.classIndex = CLASS_FIGHTER,
		.autolevel = true,
		.allegiance = FACTION_ID_RED,
		.level = 4,
		.xPosition = 12,
		.yPosition = 21,
		.redaCount = 1,
		.redas = REDA_ONeill,
		.items = {
			ITEM_AXE_IRON,
		},
		.ai = { 0, 4, 9, 0 },
	},
	{
		.charIndex = CHARACTER_SAAR,
		.classIndex = CLASS_ARMOR_KNIGHT,
		.autolevel = true,
		.allegiance = FACTION_ID_RED,
		.level = 10,
		.xPosition = 4,
		.yPosition = 22,
		.redaCount = 1,
		.redas = REDA_Saar,
		.items = {
			ITEM_LANCE_IRON,
		},
		.ai = { 0, 4, 9, 0 },
	},
	{
		.charIndex = CHARACTER_BAZBA,
		.classIndex = CLASS_MERCENARY,
		.autolevel = true,
		.allegiance = FACTION_ID_RED,
		.level = 10,
		.xPosition = 10,
		.yPosition = 9,
		.redaCount = 1,
		.redas = REDA_Bazba,
		.items = {
			ITEM_SWORD_IRON,
		},
		.ai = { 0, 4, 9, 0 },
	},
	{
		.charIndex = CHARACTER_VIGARDE,
		.classIndex = CLASS_GENERAL,
		.autolevel = true,
		.allegiance = FACTION_ID_RED,
		.level = 10,
		.xPosition = 12,
		.yPosition = 21,
		.redaCount = 1,
		.redas = REDA_Vigarde,
		.items = {
			ITEM_AXE_IRON,
			ITEM_LANCE_SILVER,
			ITEM_VULNERARY,
		},
		.ai = { NeverMoveAI },
	},
	{}
};

/**
 * Main events
 */
static const EventScr EventScr_Beginning[] = {
	LOAD1(0x1, UnitDef_Ally1)
	ENUN
	LOAD1(0x1, UnitDef_Enemy1)
	ENUN

#if defined(SID_Obstruct) && (COMMON_SKILL_VALID(SID_Obstruct))
	Evt_AddSkill(SID_Obstruct, CHARACTER_SAAR)
#endif

#if defined(SID_Nihil) && (COMMON_SKILL_VALID(SID_Nihil))
	Evt_AddSkill(SID_Nihil, CHARACTER_SAAR)
#endif

#if defined(SID_Teleportation) && (COMMON_SKILL_VALID(SID_Teleportation))
	Evt_AddSkill(SID_Teleportation, CHARACTER_BAZBA)
#endif

#if defined(SID_Aerobatics) && (COMMON_SKILL_VALID(SID_Aerobatics))
	Evt_AddSkill(SID_Aerobatics, CHARACTER_MYRRH)
#endif

#if defined(SID_FlierGuidance) && (COMMON_SKILL_VALID(SID_FlierGuidance))
	Evt_AddSkill(SID_FlierGuidance, CHARACTER_TANA)
#endif

#if defined(SID_FlierGuidance) && (COMMON_SKILL_VALID(SID_FlierGuidance))
	Evt_AddSkill(SID_FlierGuidance, CHARACTER_VANESSA)
#endif

#if defined(SID_MovBonus) && (COMMON_SKILL_VALID(SID_MovBonus))
	Evt_AddSkill(SID_MovBonus, CHARACTER_EPHRAIM)
#endif

#if defined(SID_Pass) && (COMMON_SKILL_VALID(SID_Pass))
	Evt_AddSkill(SID_Pass, CHARACTER_EPHRAIM)
#endif

#if defined(SID_Teleportation) && (COMMON_SKILL_VALID(SID_Teleportation))
	Evt_AddSkill(SID_Teleportation, CHARACTER_EPHRAIM)
#endif

#if defined(SID_MagicEye) && (COMMON_SKILL_VALID(SID_MagicEye))
	Evt_AddSkill(SID_MagicEye, CHARACTER_SALEH)
#endif

#if defined(SID_MovHone) && (COMMON_SKILL_VALID(SID_MovHone))
	Evt_AddSkill(SID_MovHone, CHARACTER_SALEH)
#endif

#if defined(SID_HealingFocus) && (COMMON_SKILL_VALID(SID_HealingFocus))
	Evt_AddSkill(SID_HealingFocus, CHARACTER_KNOLL)
#endif

#if defined(SID_ChillPow) && (COMMON_SKILL_VALID(SID_ChillPow))
	Evt_AddSkill(SID_ChillPow, CHARACTER_TANA)
#endif

#if defined(SID_ChillSpd) && (COMMON_SKILL_VALID(SID_ChillSpd))
	Evt_AddSkill(SID_ChillSpd, CHARACTER_VANESSA)
#endif

#if defined(SID_RightfulArch) && (COMMON_SKILL_VALID(SID_RightfulArch))
	Evt_AddSkill(SID_RightfulArch, CHARACTER_VANESSA)
#endif

#if defined(SID_Sol) && (COMMON_SKILL_VALID(SID_Sol))
	Evt_AddSkill(SID_Sol, CHARACTER_VANESSA)
#endif

#if defined(SID_Fury) && (COMMON_SKILL_VALID(SID_Fury))
	/* In debug mode, unit may automatically learn all skills */
	Evt_RemoveSkill(SID_Fury, CHARACTER_EPHRAIM)
#endif

	ASMC(PrologueCallBack)

	// PREP
	CALL(EventScr_08591FD8)

	SVAL(EVT_SLOT_1, 12)
	SET_HP(CHARACTER_KNOLL)

	SVAL(EVT_SLOT_1, 10)
	SET_HP(CHARACTER_VANESSA)

	SVAL(EVT_SLOT_1, 10)
	SET_HP(CHARACTER_TANA)

	NoFade
	ENDA
};

static const EventScr EventScr_Ending[] = {
	MNC2(1)
	ENDA
};

/**
 * Misc events
 */
static const EventListScr EventScr_Talk_EirikaSeth[] = {
	ConvoEvent(0xB32)
};

/**
 * Event list
 */
static const EventListScr EventListScr_Turn[] = {
	END_MAIN
};

static const EventListScr EventListScr_Character[] = {
	CharacterEventBothWays(EVFLAG_TMP(7), EventScr_Talk_EirikaSeth, CHARACTER_EIRIKA, CHARACTER_SETH)
	END_MAIN
};

static const EventListScr EventListScr_Location[] = {
	END_MAIN
};

static const EventListScr EventListScr_Misc[] = {
	DefeatAll(EventScr_Ending)
	CauseGameOverIfLordDies
	END_MAIN
};

static const EventListScr EventListScr_SelectUnit[] = {
	END_MAIN
};

static const EventListScr EventListScr_SelectDestination[] = {
	END_MAIN
};

static const EventListScr EventListScr_UnitMove[] = {
	END_MAIN
};

static void const *const EventListScr_Tutorial[] = {
	NULL
};

static const u8 TrapData_ThisEvent[] = {
	TRAP_NONE
};

static const u8 TrapData_ThisEventHard[] = {
	TRAP_NONE
};

const struct ChapterEventGroup ThisEvent = {
	.turnBasedEvents			   = EventListScr_Turn,
	.characterBasedEvents		  = EventListScr_Character,
	.locationBasedEvents		   = EventListScr_Location,
	.miscBasedEvents			   = EventListScr_Misc,
	.specialEventsWhenUnitSelected = EventListScr_SelectUnit,
	.specialEventsWhenDestSelected = EventListScr_SelectDestination,
	.specialEventsAfterUnitMoved   = EventListScr_UnitMove,
	.tutorialEvents				= EventListScr_Tutorial,

	.traps			= TrapData_ThisEvent,
	.extraTrapsInHard = TrapData_ThisEventHard,

	.playerUnitsInNormal = UnitDef_Ally1,
	.playerUnitsInHard   = UnitDef_Ally1,

	.playerUnitsChoice1InEncounter = NULL,
	.playerUnitsChoice2InEncounter = NULL,
	.playerUnitsChoice3InEncounter = NULL,

	.enemyUnitsChoice1InEncounter = NULL,
	.enemyUnitsChoice2InEncounter = NULL,
	.enemyUnitsChoice3InEncounter = NULL,

	.beginningSceneEvents = EventScr_Beginning,
	.endingSceneEvents	= EventScr_Ending,
};
