#pragma once

#include "common-chax.h"
#include "efx-anim.h"
#include "list-verify.h"
#include "constants/event-cmds.h"

#ifndef MAX_SKILL_NUM
#define MAX_SKILL_NUM 0x3FF
#endif

#define MAX_GENERIC_SKILL_NUM 0xFF

enum SkillInfoListss
{
    Skill_INFO_GENERIC,
    Skill_INFO_PERSON,
    Skill_INFO_JOB,
    Skill_INFO_ITEM,

    Skill_INFO_MAX
};

#define SKILL_INDEX_REAL(sid) ((sid) & 0xFF)
#define SKILL_INDEX_LIST(sid) (((sid) >> 8) & 0xFF)

#define EQUIPE_SKILL_VALID(sid) (sid > 0x000 && sid < 0x0FF)
#define COMMON_SKILL_VALID(sid) (sid > 0x000 && sid < 0x400)

#define SKILL_ICON(sid) ((2 << 8) + (sid))

/**
 * Generic skills
 */
#define UNIT_RAM_SKILLS_LEN 7
#define UNIT_RAM_SKILLS(unit) ((u8 *)((unit)->supports))

extern u16 const * const gpConstSkillTable_Person;
extern u16 const * const gpConstSkillTable_Job;
extern u16 const * const gpConstSkillTable_Item;
extern u16 const * const gpConstSkillTable_Weapon;

struct SkillInfo {
    const u8 * icon;
    u16 name, desc;
};

extern struct SkillInfo const * const gpSkillInfos;

const u8 * GetSkillIcon_Generic(const u8 sid);
const u8 * GetSkillIcon_Person(const u8 sid);
const u8 * GetSkillIcon_Job(const u8 sid);
const u8 * GetSkillIcon_Item(const u8 sid);

u16 GetSkillDescMsg(const u16 sid);
u16 GetSkillNameMsg(const u16 sid);
char * GetSkillDescStr(const u16 sid);
char * SkillDescToName(char * str);
char * GetSkillNameStrFormDesc(const u16 sid);
char * GetSkillNameStr(const u16 sid);

/**
 * 7 generic skill
 * 2 person skill
 * 2 job skill
 * 10 item skill
 * 
 * (maybe todo) 2 weapon skill
 */
struct SkillList {
    struct UnitListHeader header;
    u8 amt;
    u16 sid[23];
};
extern struct SkillList * (* _GetUnitSkillList)(struct Unit * unit);
#define GetUnitSkillList _GetUnitSkillList

void ResetSkillLists(void);

/* Skill tetsers */
extern bool (* _SkillTester)(struct Unit * unit, const u16 sid);
#define SkillTester _SkillTester

// Note this function can only exec for r0 = gBattleActor/gBattleTarget
extern bool (* _JudgeSkillViaList)(struct BattleUnit * unit, const u16 sid);
#define BattleSkillTester _JudgeSkillViaList
#define _BattleSkillTester(unit, sid) BattleSkillTester((struct BattleUnit *)(unit), sid)

/* Prep equip skill list */
struct PrepEquipSkillList {
    struct UnitListHeader header;
    u8 amt;
    u8 sid[0xFE];
};

void ResetPrepEquipSkillList(void);
struct PrepEquipSkillList * GetPrepEquipSkillList(struct Unit * unit);

/* Game data */
#define SKILL_ROM_DATA_AMT 5 /* Unit can learn 5 skills on lv0/5/10/15/20 */
struct SkillPreloadJConf { u8 skills[SKILL_ROM_DATA_AMT * (UNIT_LEVEL_MAX_RE / 5 + 1)];};
struct SkillPreloadPConf { u8 skills[SKILL_ROM_DATA_AMT * (UNIT_RECORDED_LEVEL_MAX / 5 + 1)];};

extern const struct SkillPreloadJConf gSkillPreloadJData[0x100];
extern const struct SkillPreloadPConf gSkillPreloadPData[0x100];

extern struct SkillPreloadJConf const * const gpSkillPreloadJData;
extern struct SkillPreloadPConf const * const gpSkillPreloadPData;

struct SkillAnimInfo {
    u8 aid;
    u8 priority;
    u16 sfx;
};

enum SkillAnimPriorityConfig {
    EFX_PRIORITY_LOW = 0x1,
    EFX_PRIORITY_NORMAL,
    EFX_PRIORITY_HIGH,
    EFX_PRIORITY_HIGHHIGH,
};

extern struct SkillAnimInfo const * const gpSkillAnimInfos;

int GetEfxSkillIndex(const u16 sid);
int GetEfxSkillPriority(const u16 sid);
int GetEfxSkillSfx(const u16 sid);

/* Efx skill */
extern struct EfxAnimConf const * const gEfxSkillAnims[0x100];
extern struct EfxAnimConf const * const * const gpEfxSkillAnims;
const struct EfxAnimConf * GetEfxSkillConf(const u8 aid);

/**
 * Skill mapanim
 */
struct ProcMapAnimSkillfx {
    PROC_HEADER;

    u8 pos;
    u32 icon_idx;
    int timer;
    int x, y;
};

extern const struct ProcCmd ProcScr_MapAnimSkillfx[];

/**
 * Event scripts
 */
enum EventSkillSubOps {
    EVSUBCMD_ADD_SKILL = 1,
    EVSUBCMD_ADD_SKILL_AT,
    EVSUBCMD_ADD_SKILL_SC,

    EVSUBCMD_REMOVE_SKILL,
    EVSUBCMD_REMOVE_SKILL_AT,
    EVSUBCMD_REMOVE_SKILL_SC,
};

#define Evt_AddSkill(sid, pid) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_ADD_SKILL, sid), _EvtParams2(pid, 0),
#define Evt_AddSkillAt(sid, x, y) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_ADD_SKILL_AT, sid), _EvtParams2(x, y),
#define Evt_AddSkillSC(sid) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_ADD_SKILL_SC, sid), _EvtParams2(0, 0),

#define Evt_RemoveSkill(sid, pid) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_REMOVE_SKILL, sid), _EvtParams2(pid, 0),
#define Evt_RemoveSkillAt(sid, x, y) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_REMOVE_SKILL_AT, sid), _EvtParams2(x, y),
#define Evt_RemoveSkillSC(sid) _EvtArg0(EVENT_CMD_SKILL, 4, EVSUBCMD_REMOVE_SKILL_SC, sid), _EvtParams2(0, 0),

/**
 * Skill menu
 */
#define UNIT_MENU_SKILL_AMOUNT 4
extern u16 UnitMenuSkills[UNIT_MENU_SKILL_AMOUNT];
extern struct MenuItemDef const * const gpSkillMenuInfos;
#define GetSkillMenuInfo(sid) (&gpSkillMenuInfos[sid])

u8 MenuSkills_OnHelpBox(struct MenuProc * menu, struct MenuItemProc * item);
u8 MenuSkills_Usability(const struct MenuItemDef * self, int number);
int MenuSkills_OnDraw(struct MenuProc * menu, struct MenuItemProc * item);
u8 MenuSkills_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 MenuSkills_Idle(struct MenuProc * menu, struct MenuItemProc * item);
int MenuSkills_Hover(struct MenuProc * menu, struct MenuItemProc * item);
int MenuSkills_Unhover(struct MenuProc * menu, struct MenuItemProc * item);

#define MENU_SKILL_INDEX(menu_item) ((menu_item)->helpMsgId) /* We use rtext msg as index */
#define IS_SKILL_MENU_ITEM(menu_item) ((menu_item)->isAvailable == MenuSkills_Usability)

/**
 * Skill action
 */
typedef bool (* SkillActionFunc_t)(ProcPtr);
extern SkillActionFunc_t const * const gpSkillActionFuncTable;

/**
 * Miscs
 */

struct SkillExtraInfo {
    s8 priv[4];
};
extern struct SkillExtraInfo const * const gpSkillExtraInfo;
#define SKILL_EFF0(sid) (gpSkillExtraInfo[sid].priv[0])
#define SKILL_EFF1(sid) (gpSkillExtraInfo[sid].priv[1])
#define SKILL_EFF2(sid) (gpSkillExtraInfo[sid].priv[2])
#define SKILL_EFF3(sid) (gpSkillExtraInfo[sid].priv[3])

bool IsSkillLearned(struct Unit * unit, const u16 sid);
void LearnSkill(struct Unit * unit, const u16 sid);
void ForgetSkill(struct Unit * unit, const u16 sid);
void ResetUnitLearnedSkillLists(void);                      /* GameInitHook */
void SaveUnitLearnedSkillLists(u8 * dst, const u32 size);   /* SaveData */
void LoadUnitLearnedSkillLists(u8 * src, const u32 size);   /* LoadData */

void UnitAutoLoadSkills(struct Unit * unit);
bool CanRemoveSkill(struct Unit * unit, const u16 sid);
int RemoveSkill(struct Unit * unit, const u16 sid);
int AddSkill(struct Unit * unit, const u16 sid);
void TryAddSkillLvup(struct Unit * unit, int level);
void TryAddSkillPromotion(struct Unit * unit, int jid);

/**
 * External MiscSkillEffects
 */
bool GetTeleportationRandomPosition(struct Unit * unit, struct Vec2 * out);

/* lucky 7 */
enum skill_lucky_seven_idx {
    LUCKY7_POW = 0,
    LUCKY7_MAG,
    LUCKY7_SKL,
    LUCKY7_SPD,
    LUCKY7_LCK,
    LUCKY7_DEF,
    LUCKY7_RES,
    LUCKY7_MOV,
};

/* Legendary skill */
extern u8 const * const gpLegendSkillPool;
int TryActivateLegendSkill(struct Unit * unit, const u16 sid);
bool SkillTesterLegendActivated(struct Unit * unit, const u16 sid);
void PreBattleCalcLegendSkills(struct BattleUnit * attacker, struct BattleUnit * defender);
int SpdGetterLegendSkills(int status, struct Unit * unit);
int DefGetterLegendSkills(int status, struct Unit * unit);
int ResGetterLegendSkills(int status, struct Unit * unit);

/* Menu skills */
u8 HealingFocus_Usability(const struct MenuItemDef * def, int number);
u8 HealingFocus_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Teleportation_Usability(const struct MenuItemDef * def, int number);
u8 Teleportation_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 LightRune_Usability(const struct MenuItemDef * def, int number);
u8 LightRune_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Mine_Usability(const struct MenuItemDef * def, int number);
u8 Mine_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Rally_Usability(const struct MenuItemDef * def, int number);
u8 Rally_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);

/* Skill actions */
bool Action_HealingFocus(ProcPtr proc);
bool Action_Teleportation(ProcPtr parent);
bool Action_LightRune(ProcPtr parent);
bool Action_Mine(ProcPtr parent);
bool Action_Rally(ProcPtr parent);
