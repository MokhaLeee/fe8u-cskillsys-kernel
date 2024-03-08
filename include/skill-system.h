#pragma once

#include "global.h"
#include "bmunit.h"
#include "uimenu.h"

#include "common-chax.h"
#include "efx-anim.h"
#include "list-verify.h"

#define MAX_SKILL_NUM 0xFF
#define SKILL_VALID(sid) ((sid > 0) && (sid < MAX_SKILL_NUM))
#define SKILL_ICON(sid) ((1 << 8) + (sid))

#define UNIT_RAM_SKILLS_LEN 6
#define UNIT_RAM_SKILLS(unit) ((u8 *)((unit)->supports))

#define STAT_SKILL_NUM_MAX 8

struct SkillInfo {
    const u8 * icon;
    u16 name, desc;
};

extern const struct SkillInfo gSkillInfos[0x100];
extern struct SkillInfo const * const gpSkillInfos;

const u8 * GetSkillIcon(const u8 sid);
u16 GetSkillDescMsg(const u8 sid);
char * GetSkillDescStr(const u8 sid);
char * SkillDescToName(char * str);
char * GetSkillNameStrFormDesc(const u8 sid);
char * GetSkillNameStr(const u8 sid);

/* Judge list */
#define SKILL_LIST_MAX_AMT 15
struct SkillList {
    struct UnitListHeader header;
    u8 amt;
    u8 sid[SKILL_LIST_MAX_AMT];
};

struct SkillList * GetUnitSkillList(struct Unit * unit);
void ResetSkillLists(void);
void DisableUnitSkills(struct Unit * unit);

/* Skill tetsers */
bool SkillTesterBasic(struct Unit * unit, const u8 sid);
bool SkillTesterFast(struct Unit * unit, const u8 sid);
extern bool (* const SkillTester)(struct Unit * unit, const u8 sid);

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

extern const u8 gConstSkillPTable[0x100][2];
extern const u8 gConstSkillJTable[0x100][2];

extern u8 const * const gpConstSkillPTable;
extern u8 const * const gpConstSkillJTable;

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

extern const struct SkillAnimInfo gSkillAnimInfos[0x100];
extern struct SkillAnimInfo const * const gpSkillAnimInfos;

int GetEfxSkillIndex(const u8 sid);
int GetEfxSkillPriority(const u8 sid);
int GetEfxSkillSfx(const u8 sid);

/* Efx skill */
extern struct EfxAnimConf const * const gEfxSkillAnims[0x100];
extern struct EfxAnimConf const * const * const gpEfxSkillAnims;
const struct EfxAnimConf * GetEfxSkillConf(const u8 aid);

/* Miscs */
bool IsSkillLearned(struct Unit * unit, const u8 sid);
void LearnSkill(struct Unit * unit, const u8 sid);
void ForgetSkill(struct Unit * unit, const u8 sid);
void ResetUnitLearnedSkillLists(void);                      /* GameInitHook */
void SaveUnitLearnedSkillLists(u8 * dst, const u32 size);   /* SaveData */
void LoadUnitLearnedSkillLists(u8 * src, const u32 size);   /* LoadData */

void UnitAutoLoadSkills(struct Unit * unit);
bool CanRemoveSkill(struct Unit * unit, const u8 sid);
int RemoveSkill(struct Unit * unit, const u8 sid);
int AddSkill(struct Unit * unit, const u8 sid);
void TryAddSkillLvup(struct Unit * unit, int level);
void TryAddSkillPromotion(struct Unit * unit, int jid);

/**
 * External MiscSkillEffects
 */
u8 DanceCommandUsabilityRework(const struct MenuItemDef * def, int number);
u8 StealCommandUsabilityRework(const struct MenuItemDef * def, int number);
u8 SupplyUsabilityRework(const struct MenuItemDef * def, int number);
u8 PickCommandUsabilityRework(const struct MenuItemDef * def, int number);
u8 SummonCommandUsabilityRework(const struct MenuItemDef * def, int number);

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
int TryActivateLegendSkill(struct Unit * unit, const u8 sid);
bool SkillTesterLegendActivated(struct Unit * unit, const u8 sid);
void PhaseSwitchUpdateLengendSkillStatus(void);
void PreBattleCalcLegendSkills(struct BattleUnit * attacker, struct BattleUnit * defender);
int SpdGetterLegendSkills(int status, struct Unit * unit);
int DefGetterLegendSkills(int status, struct Unit * unit);
int ResGetterLegendSkills(int status, struct Unit * unit);
