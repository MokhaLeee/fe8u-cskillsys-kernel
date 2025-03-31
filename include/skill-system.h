#pragma once

#include "common-chax.h"
#include "efx-anim.h"
#include "list-verify.h"
#include "kernel-lib.h"
#include "constants/event-cmds.h"
#include "constants/skills.h"

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

/**
 * Generic skills
 */
#define UNIT_RAM_SKILLS_LEN 7
#define UNIT_RAM_SKILLS(unit) ((u8 *)((unit)->supports))

#define RAM_SKILL_LEN_EXT (                                             \
    gpKernelDesigerConfig->max_equipable_skill < UNIT_RAM_SKILLS_LEN    \
        ? gpKernelDesigerConfig->max_equipable_skill                    \
        : UNIT_RAM_SKILLS_LEN)

extern u16 const * const gpConstSkillTable_Person;
extern u16 const * const gpConstSkillTable_Job;
extern u16 const * const gpConstSkillTable_Item;
extern u16 const * const gpConstSkillTable_Weapon;

struct SkillInfo {
    const u8 * icon;
    u16 name, desc;
};

extern struct SkillInfo const * const gpSkillInfos;

const u8 * GetSkillIcon1(const u8 sid);
const u8 * GetSkillIcon2(const u8 sid);
const u8 * GetSkillIcon3(const u8 sid);
const u8 * GetSkillIcon4(const u8 sid);

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

struct SkillList *GetUnitSkillList(struct Unit *unit);

void SetupBattleSkillList(void);
void GenerateSkillListExt(struct Unit *unit, struct SkillList *list);
void ForceUpdateUnitSkillList(struct Unit *unit);
void DisableUnitSkilLList(struct Unit *unit);
void ResetSkillLists(void);
void AppendBattleUnitSkillList(struct BattleUnit *bu, u16 skill);

/* Skill testers */
extern bool (*_SkillTester)(struct Unit *unit, const u16 sid);
bool _SkillListTester(struct Unit *unit, const u16 sid);
// extern bool (*_BattleFastSkillTester)(struct BattleUnit *bu, const u16 sid);

// see: ../docs/SkillSys.md
#define SkillTester _SkillTester
#define SkillListTester(unit, sid) _SkillListTester(unit, sid)
// #define BattleFastSkillTester _BattleFastSkillTester

// Note this function can only exec for r0 = gBattleActor/gBattleTarget
extern bool (* _JudgeSkillViaList)(struct BattleUnit * unit, const u16 sid);
#define BattleSkillTester _JudgeSkillViaList
#define _BattleSkillTester(unit, sid) BattleSkillTester((struct BattleUnit *)(unit), sid)

bool CheckSkillActivate(struct Unit *unit, int sid, int rate);
bool CheckBattleSkillActivate(struct BattleUnit *actor, struct BattleUnit *target, int sid, int rate);


/* Prep equip skill list */
struct PrepEquipSkillList {
    struct UnitListHeader header;
    u8 amt;
    u8 sid[0xFE];
};

void ResetPrepEquipSkillList(void);
struct PrepEquipSkillList * GetPrepEquipSkillList(struct Unit * unit);


/* JESTER - I'm editing the number of learned skills from 5 to 7, preventing class and personals from being saved in the unit struct */
/* Game data */
#define SKILL_ROM_DATA_AMT 7 /* Unit can learn 7 skills on lv0/5/10/15/20 */
struct SkillPreloadJConf { u8 skills[SKILL_ROM_DATA_AMT * (UNIT_LEVEL_MAX_RE / 5 + 1)];};
struct SkillPreloadPConf { u8 skills[SKILL_ROM_DATA_AMT * (UNIT_RECORDED_LEVEL_MAX / 5 + 1)];};

extern const struct SkillPreloadJConf gSkillPreloadJData[0x100];
extern const struct SkillPreloadPConf gSkillPreloadPData[0x100];

extern struct SkillPreloadJConf const * const gpSkillPreloadJData;
extern struct SkillPreloadPConf const * const gpSkillPreloadPData;

enum SkillAnimPriorityConfig {
    EFX_PRIORITY_LOW = 0x1,
    EFX_PRIORITY_NORMAL,
    EFX_PRIORITY_HIGH,
    EFX_PRIORITY_HIGHHIGH,
};

extern u8 const * const gpEfxSkillAnimPriority;
extern struct EfxAnimConf const * const * const gpEfxSkillAnims;

int GetEfxSkillPriority(const u16 sid);

/**
 * Skill mapanim
 */
enum mapanimskillfx_idx {
    MAPANIMFX_CHR_L = 0x19C,
    MAPANIMFX_CHR_R = 0x19E,

    MAPANIMFX_PAL = 4,
};

struct ProcMapAnimSkillfx {
    PROC_HEADER;

    u8 pos;
    u32 icon_idx;
    int timer;
    int x, y;
};

extern u8 const * const gpImg_MapAnimSKILL;
extern const struct ProcCmd ProcScr_MapAnimSkillfx[];

void NewSkillMapAnimMini(int x, int y, u16 sid, ProcPtr parent);
bool SkillMapAnimMiniExists(void);

void NewMuSkillAnimOnActiveUnit(u16 sid, void (* callback1)(ProcPtr proc), void (* callback2)(ProcPtr proc));
bool MuSkillAnimExists(void);
void NewMuSkillAnimOnActiveUnitWithDeamon(ProcPtr parent, u16 sid, void (*callback1)(ProcPtr proc), void (*callback2)(ProcPtr proc));
void RemoveMuForActiveUnit(int delay);

extern const EventScr EventScr_MuSkillAnim[];

/**
 * Event scripts
 */
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
 * Skill scroll
 */
char * GetSkillScrollItemName(int item);
int GetSkillScrollItemDescId(int item);
int GetSkillScrollItemUseDescId(int item);
int GetSkillScrollItemIconId(int item);

extern const struct MenuDef RemoveSkillMenuDef;

/**
 * Catch Em All
 */
#if defined(SID_CatchEmAll) && (COMMON_SKILL_VALID(SID_CatchEmAll))
    extern const unsigned int gCatchEmAllId;
#endif

/**
 * Miscs
 */

struct SkillExtraInfo {
    s8 priv[10];
};
extern struct SkillExtraInfo const * const gpSkillExtraInfo;
#define SKILL_EFF0(sid) (gpSkillExtraInfo[sid].priv[0])
#define SKILL_EFF1(sid) (gpSkillExtraInfo[sid].priv[1])
#define SKILL_EFF2(sid) (gpSkillExtraInfo[sid].priv[2])
#define SKILL_EFF3(sid) (gpSkillExtraInfo[sid].priv[3])
#define SKILL_EFF4(sid) (gpSkillExtraInfo[sid].priv[4])
#define SKILL_EFF5(sid) (gpSkillExtraInfo[sid].priv[5])
#define SKILL_EFF6(sid) (gpSkillExtraInfo[sid].priv[6])
#define SKILL_EFF7(sid) (gpSkillExtraInfo[sid].priv[7])
#define SKILL_EFF8(sid) (gpSkillExtraInfo[sid].priv[8])
#define SKILL_EFF9(sid) (gpSkillExtraInfo[sid].priv[9])


bool IsSkillLearned(struct Unit * unit, const u16 sid);
void LearnSkill(struct Unit * unit, const u16 sid);
void ForgetSkill(struct Unit * unit, const u16 sid);
void ResetUnitLearnedSkillLists(void);                      /* GameInitHook */
void SaveUnitLearnedSkillLists(u8 * dst, const u32 size);   /* SaveData */
void LoadUnitLearnedSkillLists(u8 * src, const u32 size);   /* LoadData */

void UnitAutoLoadSkills(struct Unit * unit);
int GetSkillSlot(struct Unit * unit, int sid);
int GetFreeSkillSlot(struct Unit * unit);
bool CanRemoveSkill(struct Unit * unit, const u16 sid);
int RemoveSkill(struct Unit * unit, const u16 sid);
int AddSkill(struct Unit * unit, const u16 sid);
void TryAddSkillLvup(struct Unit * unit, int level);
void TryAddSkillPromotion(struct Unit * unit, int jid);

/**
 * Popups
 */
void ResetPopupSkillStack(void);
void PushSkillListStack(u16 sid);
int PopSkillListStack(void);

int PoprGetLen_SkillIcon(struct PopupProc *proc, const struct PopupInstruction *inst);
void PoprDisp_SkillIcon(struct Text *text, const struct PopupInstruction *inst);
int PoprGetLen_SkillName(struct PopupProc *proc, const struct PopupInstruction *inst);
void PoprDisp_SkillName(struct Text *text, const struct PopupInstruction *inst);
bool PopR_SetupLearnSkill(void);
extern const struct PopupInstruction PopupScr_LearnSkill[];

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
int Rally_Hover(struct MenuProc * menu, struct MenuItemProc * item);
int Rally_Unhover(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 Rally_Usability(const struct MenuItemDef * def, int number);
u8 Rally_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
int GoddessDance_Hover(struct MenuProc * menu, struct MenuItemProc * item);
int GoddessDance_Unhover(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 GoddessDance_Usability(const struct MenuItemDef * def, int number);
u8 GoddessDance_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
int Stride_Hover(struct MenuProc * menu, struct MenuItemProc * item);
int Stride_Unhover(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 Stride_Usability(const struct MenuItemDef * def, int number);
u8 Stride_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Swarp_Usability(const struct MenuItemDef * def, int number);
u8 Swarp_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Blacksmith_Usability(const struct MenuItemDef * def, int number);
u8 Blacksmith_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Swap_Usability(const struct MenuItemDef * def, int number);
u8 Swap_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Transform_Usability(const struct MenuItemDef * def, int number);
u8 Transform_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 DrawBack_Usability(const struct MenuItemDef * def, int number);
u8 DrawBack_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Pivot_Usability(const struct MenuItemDef * def, int number);
u8 Pivot_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Reposition_Usability(const struct MenuItemDef * def, int number);
u8 Reposition_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Shove_Usability(const struct MenuItemDef * def, int number);
u8 Shove_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Smite_Usability(const struct MenuItemDef * def, int number);
u8 Smite_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 StealPlus_Usability(const struct MenuItemDef * def, int number);
u8 StealPlus_Effect(struct MenuProc* menu, struct MenuItemProc* menuItem);
u8 ArdentSacrifice_Usability(const struct MenuItemDef * def, int number);
u8 ArdentSacrifice_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 ReciprocalAid_Usability(const struct MenuItemDef * def, int number);
u8 ReciprocalAid_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Sacrifice_Usability(const struct MenuItemDef * def, int number);
u8 Sacrifice_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Capture_Usability(const struct MenuItemDef * def, int number);
u8 Capture_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Doppleganger_Usability(const struct MenuItemDef * def, int number);
u8 Doppleganger_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Dismount_Usability(const struct MenuItemDef * def, int number);
u8 Dismount_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Entrepreneur_Usability(const struct MenuItemDef * def, int number);
u8 Entrepreneur_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 Arise_Usability(const struct MenuItemDef * def, int number);
u8 Arise_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 AssignDecoy_Usability(const struct MenuItemDef * def, int number);
u8 AssignDecoy_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 ShadowFlash_Usability(const struct MenuItemDef * def, int number);
u8 ShadowFlash_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
int ShadowFlash_Hover(struct MenuProc * menu, struct MenuItemProc * item);
int ShadowFlash_Unhover(struct MenuProc * menu, struct MenuItemProc * menuItem);
u8 Refuge_Usability(const struct MenuItemDef * def, int number);
u8 Refuge_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 DeathBlight_Usability(const struct MenuItemDef * def, int number);
u8 DeathBlight_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 DivineReprieve_Usability(const struct MenuItemDef *def, int number);
u8 DivineReprieve_OnSelected(struct MenuProc *menu, struct MenuItemProc *item);
u8 LoadstarRush_Usability(const struct MenuItemDef * def, int number);
u8 LoadstarRush_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);
u8 WyvernCrash_Usability(const struct MenuItemDef * def, int number);
u8 WyvernCrash_OnSelected(struct MenuProc * menu, struct MenuItemProc * item);

/* Skill actions */
bool Action_HealingFocus(ProcPtr proc);
bool Action_Teleportation(ProcPtr parent);
bool Action_LightRune(ProcPtr parent);
bool Action_Mine(ProcPtr parent);
bool Action_Rally(ProcPtr parent);
bool Action_GoddessDance(ProcPtr parent);
bool Action_Stride(ProcPtr parent);
bool Action_Swarp(ProcPtr parent);
bool Action_Blacksmith(ProcPtr parent);
bool Action_Swap(ProcPtr parent);
bool Action_Transform(ProcPtr parent);
bool Action_DrawBack(ProcPtr parent);
bool Action_Pivot(ProcPtr parent);
bool Action_Reposition(ProcPtr parent);
bool Action_Shove(ProcPtr parent);
bool Action_Smite(ProcPtr parent);
bool Action_StealPlus(ProcPtr parent);
bool Action_ArdentSacrifice(ProcPtr parent);
bool Action_ReciprocalAid(ProcPtr parent);
bool Action_Sacrifice(ProcPtr parent);
bool Action_Capture(ProcPtr parent);
bool Action_Doppleganger(ProcPtr parent);
bool Action_Dismount(ProcPtr parent);
bool Action_Entrepreneur(ProcPtr parent);
bool Action_Arise(ProcPtr parent);
bool Action_AssignDecoy(ProcPtr parent);
bool Action_ShadowFlash(ProcPtr parent);
bool Action_Refuge(ProcPtr parent);
bool Action_DeathBlight(ProcPtr parent);
bool Action_DivineReprieve(ProcPtr parent);
bool Action_LoadstarRush(ProcPtr parent);
bool Action_WyvernCrash(ProcPtr parent);