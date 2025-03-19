#include "common-chax.h"
#include "skill-system.h"
#include "debuff.h"
#include "event-rework.h"
#include "constants/skills.h"

extern const EventScr TestEvent_SetDebuff[];
extern const EventScr TestEvent_ClearDebuff[];

/**
 * Udefs
 */
static const struct UnitDefinition UnitDef_Ally1[] = {
	{
		.charIndex = CHARACTER_EIRIKA,
		.classIndex = CLASS_EIRIKA_LORD,
		.allegiance = FACTION_ID_BLUE,
		.autolevel = true,
		.level = 9,
		.xPosition = 16,
		.yPosition = 29,
		.redaCount = 1,
		.redas = (const struct REDA[]) {{
			.x = 14,
			.y = 27,
			.b = -1,
		}},
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
		.redas = (const struct REDA[]) {{
			.x = 15,
			.y = 27,
			.b = -1,
			.delayFrames = 8,
		}},
		.items = {
			ITEM_SWORD_SILVER,
			ITEM_LANCE_SILVER,
			ITEM_HOPLON_SHIELD,
			ITEM_ELIXIR
		},
	},
	{
		.charIndex = CHARACTER_MYRRH,
		.classIndex = CLASS_MANAKETE_MYRRH,
		.allegiance = FACTION_ID_BLUE,
		.autolevel = true,
		.level = 9,
		.xPosition = 16,
		.yPosition = 29,
		.redaCount = 1,
		.redas = (const struct REDA[]) {{
			.x = 13,
			.y = 25,
			.b = -1,
			.delayFrames = 16,
		}},
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
		.redas = (const struct REDA[]) {{
			.x = 16,
			.y = 21,
			.b = -1,
			.delayFrames = 32,
		}},
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
		.redas = (const struct REDA[]) {{
			.x = 17,
			.y = 19,
			.b = -1,
			.delayFrames = 48,
		}},
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
		.redas = (const struct REDA[]) {{
			.x = 14,
			.y = 24,
			.b = -1,
			.delayFrames = 48,
		}},
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
		.redas = (const struct REDA[]) {{
			.x = 13,
			.y = 27,
			.b = -1,
		}},
		.items = {
			ITEM_LANCE_IRON,
			ITEM_SOLARBRACE,
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
		.redas = (const struct REDA[]) {{
			.x = 14,
			.y = 26,
			.b = -1,
			.delayFrames = 64,
		}},
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
		.redas = (const struct REDA[]) {{
			.x = 12,
			.y = 27,
			.b = -1,
			.delayFrames = 64,
		}},
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
		.redas = (const struct REDA[]) {{
			.x = 17,
			.y = 28,
			.b = -1,
			.delayFrames = 64,
		}},
		.items = {
			ITEM_DARK_FLUX,
			ITEM_VULNERARY
		},
	},
	{
		.charIndex = CHARACTER_NATASHA,
		.classIndex = CLASS_CLERIC,
		.autolevel = true,
		.level = 1,
		.xPosition = 12,
		.yPosition = 29,
		.redaCount = 1,
		.redas = &(const struct REDA) {
			.x = 15,
			.y = 23,
			.b = -1,
			.delayFrames = 64,
		},
		.items = {
			ITEM_VULNERARY
		},
	},
	{}
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
		.redas = (const struct REDA[]) {{
			.x = 16,
			.y = 19,
			.b = -1,
		}},
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
		.redas = (const struct REDA[]) {{
			.x = 10,
			.y = 25,
			.b = -1,
		}},
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
		.redas = (const struct REDA[]) {{
			.x = 8,
			.y = 7,
			.b = -1,
		}},
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
		.redas = (const struct REDA[]) {{
			.x = 13,
			.y = 23,
			.b = -1,
		}},
		.items = {
			ITEM_AXE_IRON,
			ITEM_LANCE_SILVER,
			ITEM_VULNERARY,
		},
		.ai = { NeverMoveAI },
	},
	{
		.charIndex = CHARACTER_RIEV,
		.classIndex = CLASS_BISHOP,
		.autolevel = true,
		.allegiance = FACTION_ID_RED,
		.level = 10,
		.xPosition = 12,
		.yPosition = 21,
		.redaCount = 1,
		.redas = (const struct REDA []) {{
			.x = 12,
			.y = 23,
			.b = -1,
		}},
		.items = {
			ITEM_VULNERARY,
		},
		.ai = { DefaultAI },
	},
	{
		.charIndex = CHARACTER_SELENA,
		.classIndex = CLASS_MAGE_KNIGHT_F,
		.autolevel = true,
		.allegiance = FACTION_ID_RED,
		.level = 10,
		.xPosition = 12,
		.yPosition = 21,
		.redaCount = 1,
		.redas = (const struct REDA []) {{
			.x = 13,
			.y = 18,
			.b = -1,
		}},
		.items = {
			ITEM_VULNERARY,
		},
		.ai = { AI_A_00, AI_B_03, (1 | 8), 0 },
	},
	{
		.charIndex = CHARACTER_NOVALA,
		.classIndex = CLASS_SHAMAN,
		.autolevel = true,
		.allegiance = FACTION_ID_RED,
		.level = 5,
		.xPosition = 10,
		.yPosition = 9,
		.redaCount = 1,
		.redas = (const struct REDA []) {{
			.x = 9,
			.y = 7,
			.b = -1,
		}},
		.ai = { 0, 4, 9, 0 }
	},
{
		.charIndex = 0x80,
		.classIndex = CLASS_BRIGAND,
		.allegiance = FACTION_ID_RED,
		.autolevel = true,
		.level = 1,
		.xPosition = 10,
		.yPosition = 10,
	},
	{
		.charIndex = 0x80,
		.classIndex = CLASS_BRIGAND,
		.allegiance = FACTION_ID_RED,
		.autolevel = true,
		.level = 5,
		.xPosition = 10,
		.yPosition = 11,
	},
	{
		.charIndex = 0x80,
		.classIndex = CLASS_BRIGAND,
		.allegiance = FACTION_ID_RED,
		.autolevel = true,
		.level = 10,
		.xPosition = 10,
		.yPosition = 12,
	},
	{}
};

static void modify_unit_status(void)
{
	struct Unit *unit;

	unit = GetUnitFromCharId(CHARACTER_EPHRAIM);
	if (unit) {
#if defined(SID_Fury) && (EQUIPE_SKILL_VALID(SID_Fury))
		UnitAddItem(unit, (SID_Fury << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL);
#endif

		unit->items[0] = (10 << 8) | ITEM_LANCE_IRON;
		unit->ranks[ITYPE_LANCE] = WPN_EXP_B - 1;
		unit->level = 10;
	}

	unit = GetUnitFromCharId(CHARACTER_EIRIKA);
	if (unit) {
		unit->exp = 99;
	}

	unit = GetUnitFromCharId(CHARACTER_MYRRH);
	if (unit) {
		unit->exp = 99;
	}

	unit = GetUnitFromCharId(CHARACTER_NATASHA);
	if (unit) {
		unit->res = 20;
	}
}

static void modify_unit_status_post_prep(void)
{
	struct Unit *unit;

	unit = GetUnitFromCharId(CHARACTER_EPHRAIM);
	if (unit) {
		SetUnitStatus(unit, UNIT_STATUS_POISON);
	}

	unit = GetUnitFromCharId(CHARACTER_SELENA);
	if (unit) {
		SetUnitStatus(unit, UNIT_STATUS_SILENCED);
	}

	unit = GetUnitFromCharId(CHARACTER_NOVALA);
	if (unit) {
		SetUnitStatus(unit, UNIT_STATUS_BERSERK);
	}

	unit = GetUnitFromCharId(CHARACTER_RIEV);
	if (unit) {
		SetUnitStatus(unit, UNIT_STATUS_SLEEP);
	}
}

/**
 * Main events
 */
static const EventScr EventScr_Beginning[] = {
	LOAD1(0x1, UnitDef_Ally1)
	ENUN
	LOAD1(0x1, UnitDef_Enemy1)
	ENUN

#if defined(SID_Obstruct) && (EQUIPE_SKILL_VALID(SID_Obstruct))
	Evt_AddSkill(SID_Obstruct, CHARACTER_SAAR)
#endif

#if defined(SID_Nihil) && (EQUIPE_SKILL_VALID(SID_Nihil))
	Evt_AddSkill(SID_Nihil, CHARACTER_SAAR)
#endif

#if defined(SID_Teleportation) && (EQUIPE_SKILL_VALID(SID_Teleportation))
	Evt_AddSkill(SID_Teleportation, CHARACTER_BAZBA)
	Evt_AddSkill(SID_Teleportation, CHARACTER_NOVALA)
#endif

#if defined(SID_Aerobatics) && (EQUIPE_SKILL_VALID(SID_Aerobatics))
	Evt_AddSkill(SID_Aerobatics, CHARACTER_MYRRH)
#endif

#if defined(SID_FlierGuidance) && (EQUIPE_SKILL_VALID(SID_FlierGuidance))
	Evt_AddSkill(SID_FlierGuidance, CHARACTER_TANA)
#endif

#if defined(SID_FlierGuidance) && (EQUIPE_SKILL_VALID(SID_FlierGuidance))
	Evt_AddSkill(SID_FlierGuidance, CHARACTER_VANESSA)
#endif

#if defined(SID_HpBonus) && (EQUIPE_SKILL_VALID(SID_HpBonus))
	Evt_AddSkill(SID_HpBonus, CHARACTER_EPHRAIM)
#endif

#if defined(SID_Pass) && (EQUIPE_SKILL_VALID(SID_Pass))
	Evt_AddSkill(SID_Pass, CHARACTER_EPHRAIM)
#endif

#if defined(SID_Teleportation) && (EQUIPE_SKILL_VALID(SID_Teleportation))
	Evt_AddSkill(SID_Teleportation, CHARACTER_EPHRAIM)
#endif

#if defined(SID_MagicEye) && (EQUIPE_SKILL_VALID(SID_MagicEye))
	Evt_AddSkill(SID_MagicEye, CHARACTER_SALEH)
#endif

#if defined(SID_MovHone) && (EQUIPE_SKILL_VALID(SID_MovHone))
	Evt_AddSkill(SID_MovHone, CHARACTER_SALEH)
#endif

#if defined(SID_HealingFocus) && (EQUIPE_SKILL_VALID(SID_HealingFocus))
	Evt_AddSkill(SID_HealingFocus, CHARACTER_KNOLL)
#endif

#if defined(SID_ChillPow) && (EQUIPE_SKILL_VALID(SID_ChillPow))
	Evt_AddSkill(SID_ChillPow, CHARACTER_TANA)
#endif

#if defined(SID_BloodSurge) && (EQUIPE_SKILL_VALID(SID_BloodSurge))
	Evt_AddSkill(SID_BloodSurge, CHARACTER_TANA)
#endif

#if defined(SID_ChillSpd) && (EQUIPE_SKILL_VALID(SID_ChillSpd))
	Evt_AddSkill(SID_ChillSpd, CHARACTER_VANESSA)
#endif

#if defined(SID_RightfulArch) && (EQUIPE_SKILL_VALID(SID_RightfulArch))
	Evt_AddSkill(SID_RightfulArch, CHARACTER_VANESSA)
#endif

#if defined(SID_Sol) && (EQUIPE_SKILL_VALID(SID_Sol))
	Evt_AddSkill(SID_Sol, CHARACTER_VANESSA)
#endif

#if defined(SID_Aptitude) && (EQUIPE_SKILL_VALID(SID_Aptitude))
	Evt_AddSkill(SID_Aptitude, CHARACTER_VANESSA)
#endif

#if defined(SID_WingedShield) && (EQUIPE_SKILL_VALID(SID_WingedShield))
	Evt_AddSkill(SID_WingedShield, CHARACTER_VANESSA)
#endif

#if defined(SID_Fury) && (EQUIPE_SKILL_VALID(SID_Fury))
	/* In debug mode, unit may automatically learn all skills */
	Evt_RemoveSkill(SID_Fury, CHARACTER_EPHRAIM)
#endif

	ASMC(modify_unit_status)

	SVAL(EVT_SLOT_3, ITEM_VULNERARY)
	GIVEITEMTO(CHARACTER_EPHRAIM)

	SVAL(EVT_SLOT_3, 4000)
	GIVEITEMTOMAIN(0)

	// PREP
	CALL(EventScr_08591FD8)

	ASMC(modify_unit_status_post_prep)

	SVAL(EVT_SLOT_1, 12)
	SET_HP(CHARACTER_KNOLL)

	SVAL(EVT_SLOT_1, 10)
	SET_HP(CHARACTER_VANESSA)

	SVAL(EVT_SLOT_1, 10)
	SET_HP(CHARACTER_TANA)

	SVAL(EVT_SLOT_1, 40)
	SET_HP(CHARACTER_VIGARDE)

	SVAL(EVT_SLOT_1, 23)
	SET_HP(CHARACTER_EPHRAIM)

	CALL(TestEvent_SetDebuff)
	CALL(TestEvent_ClearDebuff)

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

static const void *const EventListScr_Tutorial[] = {
	NULL
};

static const u8 TrapData_ThisEvent[] = {
	TRAP_NONE
};

static const u8 TrapData_ThisEventHard[] = {
	TRAP_NONE
};

const struct ChapterEventGroup ThisEvent = {
	.turnBasedEvents               = EventListScr_Turn,
	.characterBasedEvents          = EventListScr_Character,
	.locationBasedEvents           = EventListScr_Location,
	.miscBasedEvents               = EventListScr_Misc,
	.specialEventsWhenUnitSelected = EventListScr_SelectUnit,
	.specialEventsWhenDestSelected = EventListScr_SelectDestination,
	.specialEventsAfterUnitMoved   = EventListScr_UnitMove,
	.tutorialEvents                = EventListScr_Tutorial,

	.traps            = TrapData_ThisEvent,
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
	.endingSceneEvents    = EventScr_Ending,
};
