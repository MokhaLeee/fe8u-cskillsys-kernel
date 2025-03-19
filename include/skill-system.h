#pragma once

#include "common-chax.h"
#include "efx-anim.h"
#include "list-verify.h"
#include "kernel-lib.h"
#include "constants/event-cmds.h"

#ifndef MAX_SKILL_NUM
#define MAX_SKILL_NUM 0x3FF
#endif

#define MAX_GENERIC_SKILL_NUM 0xFF

enum SkillInfoListss {
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

#define RAM_SKILL_LEN_EXT ( \
	gpKernelDesigerConfig->max_equipable_skill < UNIT_RAM_SKILLS_LEN \
		? gpKernelDesigerConfig->max_equipable_skill \
		: UNIT_RAM_SKILLS_LEN)

extern u16 const *const gpConstSkillTable_Person;
extern u16 const *const gpConstSkillTable_Job;
extern u16 const *const gpConstSkillTable_Item;
extern u16 const *const gpConstSkillTable_Weapon;

struct SkillInfo {
	const u8 *icon;
	u16 name, desc;
};

extern struct SkillInfo const *const gpSkillInfos;

const u8 *GetSkillIcon1(const u8 sid);
const u8 *GetSkillIcon2(const u8 sid);
const u8 *GetSkillIcon3(const u8 sid);
const u8 *GetSkillIcon4(const u8 sid);

u16 GetSkillDescMsg(const u16 sid);
u16 GetSkillNameMsg(const u16 sid);
char *GetSkillDescStr(const u16 sid);
char *SkillDescToName(char *str);
char *GetSkillNameStrFormDesc(const u16 sid);
char *GetSkillNameStr(const u16 sid);

/**
 * 7 generic skill
 * 3 person skill
 * 3 job skill
 * 10 item skill
 *
 * (maybe todo) 2 weapon skill
 */
struct SkillList {
	struct UnitListHeader header;
	u8 amt;
	u8 _pad_;
	u16 sid[31];
};
struct SkillList *GetUnitSkillList(struct Unit *unit);

void SetupBattleSkillList(void);
void GenerateSkillListExt(struct Unit *unit, struct SkillList *list);
void ForceUpdateUnitSkillList(struct Unit *unit);
void DisableUnitSkilLList(struct Unit *unit);
void ResetSkillLists(void);
void AppendBattleUnitSkillList(struct BattleUnit *bu, u16 skill);

/* Skill tetsers */
// see: ../docs/SkillSys.md
bool SkillTester(struct Unit *unit, const u16 sid);
bool SkillListTester(struct Unit *unit, const u16 sid);
bool BattleFastSkillTester(struct BattleUnit *bu, const u16 sid);

bool CheckSkillActivate(struct Unit *unit, int sid, int rate);
bool CheckActiveUnitSkillActivate(int sid, int rate);
bool CheckBattleSkillActivate(struct BattleUnit *actor, struct BattleUnit *target, int sid, int rate);

/**
 * FEB list
 *
 * This is aligned to old asm skillsystem config, see:
 * https://feuniverse.us/t/the-skill-system-and-you-maximizing-your-usage-of-fe8s-most-prolific-bundle-of-wizardry/8232/5
 */
struct FebListEnt {
	u8 level;
	u8 sid;
};

#define JOB_SKILL_LEVEL_PROMOTE 0xFF
#define JOB_SKILL_LEVEL_CONFIG(__level, __config) (((__level) & 0x1F) | (__config))
#define LOAD_JOB_SKILL_CONFIG(__level_config)  ((__level_config) & 0xE0)
#define LOAD_JOB_SKILL_LEVEL(__level_config)   ((__level_config) & 0x1F)
#define LOAD_JOB_SKILL_CONFIG_NONE           (0 << 5)
#define LOAD_JOB_SKILL_CONFIG_PLAYER_ONLY    (1 << 5)
#define LOAD_JOB_SKILL_CONFIG_ENEMY_ONLY     (2 << 5)
#define LOAD_JOB_SKILL_CONFIG_WO_TUTORIAL    (3 << 5)
#define LOAD_JOB_SKILL_CONFIG_HARD_MODE_ONLY (4 << 5)

// extern struct FebListEnt const *const CharLevelUpSkillTable[0x100];
// extern struct FebListEnt const *const ClassLevelUpSkillTable[0x100];

extern struct FebListEnt const *const *const gpCharLevelUpSkillTable;
extern struct FebListEnt const *const *const gpClassLevelUpSkillTable;

#define DEFAULT_LEVEL_SKILLS_BUF_MAX_LEN 16

int GetUnitLevelSkills(struct Unit *unit, int level_from, int level_to, u8 *out_buffer, int max_len);
int GetInitialSkillList(struct Unit *unit, u8 *out_buffer, int max_len);
int GetLevelUpSkillList(struct Unit *unit, int level, u8 *out_buffer, int max_len);
int GetPromotionSkillList(struct Unit *unit, u8 *out_buffer, int max_len);

void FebList_LoadUnitSkill(struct Unit *unit);
void FebList_LvupAddSkill(struct Unit *unit, int level);
void FebList_PromotionAddSkill(struct Unit *unit);

/* Prep equip skill list */
struct PrepEquipSkillList {
	struct UnitListHeader header;
	u8 amt;
	u8 sid[0xFE];
};

void ResetPrepEquipSkillList(void);
struct PrepEquipSkillList *GetPrepEquipSkillList(struct Unit *unit);

/* Game data */
#define SKILL_ROM_DATA_AMT 5 /* Unit can learn 5 skills on lv0/5/10/15/20 */

struct SkillPreloadJConf {
	u8 skills[SKILL_ROM_DATA_AMT * (UNIT_LEVEL_MAX_RE / 5 + 1)];
};

struct SkillPreloadPConf {
	u8 skills[SKILL_ROM_DATA_AMT * (UNIT_RECORDED_LEVEL_MAX / 5 + 1)];
};

extern const struct SkillPreloadJConf gSkillPreloadJData[0x100];
extern const struct SkillPreloadPConf gSkillPreloadPData[0x100];

extern struct SkillPreloadJConf const *const gpSkillPreloadJData;
extern struct SkillPreloadPConf const *const gpSkillPreloadPData;

enum SkillAnimPriorityConfig {
	EFX_PRIORITY_LOW = 0x1,
	EFX_PRIORITY_NORMAL,
	EFX_PRIORITY_HIGH,
	EFX_PRIORITY_HIGHHIGH,
};

extern u8 const *const gpEfxSkillAnimPriority;
extern struct EfxAnimConf const *const *const gpEfxSkillAnims;

static inline int GetEfxSkillPriority(const u16 sid)
{
	return gpEfxSkillAnimPriority[sid];
}

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

extern u8 const *const gpImg_MapAnimSKILL;
extern const struct ProcCmd ProcScr_MapAnimSkillfx[];

void NewSkillMapAnimMini(int x, int y, u16 sid, ProcPtr parent);
bool SkillMapAnimMiniExists(void);

void NewMuSkillAnimOnActiveUnit(u16 sid, void (*callback1)(ProcPtr proc), void (*callback2)(ProcPtr proc));
bool MuSkillAnimExists(void);
void NewMuSkillAnimOnActiveUnitWithDeamon(ProcPtr parent, u16 sid, void (*callback1)(ProcPtr proc), void (*callback2)(ProcPtr proc));
void RemoveMuForActiveUnit(int delay);

extern const EventScr EventScr_MuSkillAnim[];

/**
 * Skill menu
 */
#define UNIT_MENU_SKILL_AMOUNT 4
extern u16 UnitMenuSkills[UNIT_MENU_SKILL_AMOUNT];
extern struct MenuItemDef const *const gpSkillMenuInfos;
#define GetSkillMenuInfo(sid) (&gpSkillMenuInfos[sid])

#ifdef CONFIG_MENU_SKILL_NOT_IN_UPPER
u8 UpperMenuSkill_Usability(const struct MenuItemDef *self, int number);
u8 UpperMenuSkill_OnSelected(struct MenuProc *menu, struct MenuItemProc *item);
#endif /* MENU_SKILL_NOT_IN_UPPER */

u8 MenuSkills_OnHelpBox(struct MenuProc *menu, struct MenuItemProc *item);
u8 MenuSkills_Usability(const struct MenuItemDef *self, int number);
int MenuSkills_OnDraw(struct MenuProc *menu, struct MenuItemProc *item);
u8 MenuSkills_OnSelected(struct MenuProc *menu, struct MenuItemProc *item);
u8 MenuSkills_Idle(struct MenuProc *menu, struct MenuItemProc *item);
int MenuSkills_Hover(struct MenuProc *menu, struct MenuItemProc *item);
int MenuSkills_Unhover(struct MenuProc *menu, struct MenuItemProc *item);

#define MENU_SKILL_INDEX(menu_item) ((menu_item)->helpMsgId) /* We use rtext msg as index */
#define IS_SKILL_MENU_ITEM(menu_item) ((menu_item)->isAvailable == MenuSkills_Usability)

/**
 * Skill action
 */
typedef bool (*SkillActionFunc_t)(ProcPtr proc);
extern SkillActionFunc_t const *const gpSkillActionFuncTable;

/**
 * Skill scroll
 */
char *GetSkillScrollItemName(int item);
int GetSkillScrollItemDescId(int item);
int GetSkillScrollItemUseDescId(int item);
int GetSkillScrollItemIconId(int item);

extern const struct MenuDef RemoveSkillMenuDef;

/**
 * Miscs
 */

struct SkillExtraInfo {
	s8 priv[4];
};
extern struct SkillExtraInfo const *const gpSkillExtraInfo;
#define SKILL_EFF0(sid) (gpSkillExtraInfo[sid].priv[0])
#define SKILL_EFF1(sid) (gpSkillExtraInfo[sid].priv[1])
#define SKILL_EFF2(sid) (gpSkillExtraInfo[sid].priv[2])
#define SKILL_EFF3(sid) (gpSkillExtraInfo[sid].priv[3])

bool IsSkillLearned(struct Unit *unit, const u16 sid);
void LearnSkill(struct Unit *unit, const u16 sid);
void ForgetSkill(struct Unit *unit, const u16 sid);
void ResetUnitLearnedSkillLists(void);					 /* GameInitHook */
void SaveUnitLearnedSkillLists(u8 *dst, const u32 size);   /* SaveData */
void LoadUnitLearnedSkillLists(u8 *src, const u32 size);   /* LoadData */

void UnitAutoLoadSkills(struct Unit *unit);
int GetSkillSlot(struct Unit *unit, int sid);
int GetFreeSkillSlot(struct Unit *unit);
bool CanRemoveSkill(struct Unit *unit, const u16 sid);
int RemoveSkill(struct Unit *unit, const u16 sid);
int AddSkill(struct Unit *unit, const u16 sid);
void TryAddSkillLvup(struct Unit *unit, int level);
void TryAddSkillPromotion(struct Unit *unit, int jid);

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
bool GetTeleportationRandomPosition(struct Unit *unit, struct Vec2 *out);

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
extern u16 const *const gpLegendSkillPool;
int TryActivateLegendSkill(struct Unit *unit, const u16 sid);
bool SkillTesterLegendActivated(struct Unit *unit, const u16 sid);
void PreBattleCalcLegendSkills(struct BattleUnit *attacker, struct BattleUnit *defender);
int SpdGetterLegendSkills(int status, struct Unit *unit);
int DefGetterLegendSkills(int status, struct Unit *unit);
int ResGetterLegendSkills(int status, struct Unit *unit);

/* Menu skills */
u8 HealingFocus_Usability(const struct MenuItemDef *def, int number);
u8 HealingFocus_OnSelected(struct MenuProc *menu, struct MenuItemProc *item);
u8 Teleportation_Usability(const struct MenuItemDef *def, int number);
u8 Teleportation_OnSelected(struct MenuProc *menu, struct MenuItemProc *item);
u8 LightRune_Usability(const struct MenuItemDef *def, int number);
u8 LightRune_OnSelected(struct MenuProc *menu, struct MenuItemProc *item);
u8 Mine_Usability(const struct MenuItemDef *def, int number);
u8 Mine_OnSelected(struct MenuProc *menu, struct MenuItemProc *item);
int Rally_Hover(struct MenuProc *menu, struct MenuItemProc *item);
int Rally_Unhover(struct MenuProc *menu, struct MenuItemProc *menuItem);
u8 Rally_Usability(const struct MenuItemDef *def, int number);
u8 Rally_OnSelected(struct MenuProc *menu, struct MenuItemProc *item);
int GoddessDance_Hover(struct MenuProc *menu, struct MenuItemProc *item);
int GoddessDance_Unhover(struct MenuProc *menu, struct MenuItemProc *menuItem);
u8 GoddessDance_Usability(const struct MenuItemDef *def, int number);
u8 GoddessDance_OnSelected(struct MenuProc *menu, struct MenuItemProc *item);
int Stride_Hover(struct MenuProc *menu, struct MenuItemProc *item);
int Stride_Unhover(struct MenuProc *menu, struct MenuItemProc *menuItem);
u8 Stride_Usability(const struct MenuItemDef *def, int number);
u8 Stride_OnSelected(struct MenuProc *menu, struct MenuItemProc *item);
u8 Swarp_Usability(const struct MenuItemDef *def, int number);
u8 Swarp_OnSelected(struct MenuProc *menu, struct MenuItemProc *item);
u8 Blacksmith_Usability(const struct MenuItemDef *def, int number);
u8 Blacksmith_OnSelected(struct MenuProc *menu, struct MenuItemProc *item);
u8 DivineReprieve_Usability(const struct MenuItemDef *def, int number);
u8 DivineReprieve_OnSelected(struct MenuProc *menu, struct MenuItemProc *item);
bool Action_DivineReprieve(ProcPtr parent);

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
