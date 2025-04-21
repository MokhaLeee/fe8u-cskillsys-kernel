#pragma once

#include "common-chax.h"
#include "constants/event-cmds.h"

enum char_identifier_event_expa {
	CHAR_EVT_UID_AT_SLOT2 = -4,

	CHAR_EVT_K_MAX = -5,
};

/* input: [slot-2]=uid */
extern const EventListScr EventScr_UidWarpIN[];
extern const EventListScr EventScr_UidWarpOUT[];
extern const EventListScr EventScr_UidFlushingIN[];
extern const EventListScr EventScr_UidFlushingOUT[];

/**
 * The following part of definition may cause complain from coding-style check.
 * But this is a concession made by decomp when designing the event script to adapt to EAstdlib.
 * So let it go LOL
 *
 * 必要的牺牲识得唔识得啊 (■_■)
 */

/* Skill-sys kernel related event ops */
enum EventSkillSubOps {
	EVSUBCMD_ADD_SKILL = 1,
	EVSUBCMD_ADD_SKILL_ACTIVE,
	EVSUBCMD_ADD_SKILL_AT,
	EVSUBCMD_ADD_SKILL_SC,

	EVSUBCMD_REMOVE_SKILL,
	EVSUBCMD_REMOVE_SKILL_ACTIVE,
	EVSUBCMD_REMOVE_SKILL_AT,
	EVSUBCMD_REMOVE_SKILL_SC,
};

#define Evt_AddSkill(sid, pid) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_ADD_SKILL, sid), _EvtParams2(pid, 0),
#define Evt_AddSkillActive(sid) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_ADD_SKILL_ACTIVE, sid), _EvtParams2(0, 0),
#define Evt_AddSkillAt(sid, x, y) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_ADD_SKILL_AT, sid), _EvtParams2(x, y),
#define Evt_AddSkillSC(sid) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_ADD_SKILL_SC, sid), _EvtParams2(0, 0),

#define Evt_RemoveSkill(sid, pid) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_REMOVE_SKILL, sid), _EvtParams2(pid, 0),
#define Evt_RemoveSkillActive(sid) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_REMOVE_SKILL_ACTIVE, sid), _EvtParams2(0, 0),
#define Evt_RemoveSkillAt(sid, x, y) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_REMOVE_SKILL_AT, sid), _EvtParams2(x, y),
#define Evt_RemoveSkillSC(sid) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_REMOVE_SKILL_SC, sid), _EvtParams2(0, 0),
