#pragma once

#include "global.h"
#include "uimenu.h"
#include "uiselecttarget.h"

#include "common-chax.h"
#include "skill-system.h"

#define COMBART_VALID(cid) (((cid) > 0) && ((cid) < 0xFF))
#define COMBART_ICON(cid) ((2 << 8) + (cid))

struct CombatArtInfo {
    const u8 * icon;
    u16 name, desc;

    u8 wtype;
    s8 range_bonus;
    s16 cost;

    struct {
        s8 atk, def, hit, avo, crit, silencer, dodge;
    } battle_status;

    /* flags */
    u32 external_calc : 1;
    u32 magic_attack : 1;
    u32 effective_armor : 1;
    u32 effective_ride : 1;
    u32 effective_fly : 1;
    u32 effective_dragon : 1;
    u32 effective_monster : 1;
    u32 effective_all : 1;
    u32 double_attack : 1;
    u32 debuff_gravity : 1;
    u32 debuff_def : 1;
    u32 debuff_res : 1;
    u32 debuff_weaken : 1;
    u32 aoe_debuff : 1;
};

extern const struct CombatArtInfo gCombatArtInfos[0x100];
extern struct CombatArtInfo const * const gpCombatArtInfos;

static inline u16 GetCombatArtName(u8 cid)
{
    return gpCombatArtInfos[cid].name;
}

static inline u16 GetCombatArtDesc(u8 cid)
{
    if (0 == gpCombatArtInfos[cid].desc)
        return gpCombatArtInfos[cid].name;

    return gpCombatArtInfos[cid].desc;
}

/* Combat-art status */
struct CombatArtStatus {
    u8 cid;
    s8 uid;
    bool hitted;
    u8 x, y;
    u8 _pad_[0x10 - 0x8];
};

extern struct CombatArtStatus gCombatArtStatus;

u8 GetCombatArtInForce(struct Unit * unit);
void RegisterCombatArtStatus(struct Unit * unit, u8 cid);
void RegisterCombatArtHitted(void);
void RegisterCombatArtTargetPos(u8 x, u8 y);
bool IsCombatArtHitted(void);
void ResetCombatArtStatus(void);
void SaveCombatArtStatus(u8 * dst, const u32 size);
void LoadCombatArtStatus(u8 * src, const u32 size);

/* Combat-art list */
#define COMBART_LIST_MAX_AMT 6
struct CombatArtList {
    struct UnitListHeader ref;
    u8 wtype;
    u8 amt;
    u8 cid[COMBART_LIST_MAX_AMT];
};

struct CombatArtList * GetCombatArtList(struct Unit * unit, u8 wtype);
void ResetCombatArtList(void);

struct CombatArtRomTable {
    u8 cid_sword[8];
    u8 cid_lance[8];
    u8 cid_axe[8];
    u8 cid_bow[8];
};

extern const struct CombatArtRomTable gCombatArtRomPTable[0x100];
extern const struct CombatArtRomTable gCombatArtRomJTable[0x100];
extern const struct CombatArtRomTable gCombatArtDefaultTable;
extern const u8 gCombatArtWeaponTable[0x100];
extern const u8 gCombatArtSkillTable[0x100];

extern struct CombatArtRomTable const * const gpCombatArtRomPTable;
extern struct CombatArtRomTable const * const gpCombatArtRomJTable;
extern struct CombatArtRomTable const * const gpCombatArtDefaultTable;
extern u8 const * const gpCombatArtWeaponTable;
extern u8 const * const gpCombatArtSkillTable;

/* TargetSel & BKSEL */
ProcPtr NewTargetSelectionRework(const struct SelectInfo * selectInfo);
bool CombatArtSelectTargetExist(void);
u8 GetCombatArtByTargetSelIndex(void);

/* HelpBox related */
void DrawHelpBoxCombatArtBkselLabels(void);
void DrawHelpBoxCombatArtBkselStats(void);

/* EfxSkill */
extern const struct SkillAnimInfo gEfxCombatArtAnimInfos[0x100];
extern struct SkillAnimInfo const * const gpEfxCombatArtAnimInfos;

void InitEfxCombatArtRoundData(void);
int GetEfxCombatArtIndex(const u8 cid);
int GetEfxCombatArtPriority(const u8 cid);
int GetEfxCombatArtSfx(const u8 cid);
void RegisterEfxSkillCombatArt(int round, const u8 cid);
u8 GetEfxCombatArt(int round);

/* Menu */
u8 CombatArtActionCommandUsability(const struct MenuItemDef * def, int number);
u8 CombatArtActionCommandEffect(struct MenuProc * menu, struct MenuItemProc * menuItem);
int CombatArtActionCommandHover(struct MenuProc * menu, struct MenuItemProc * menuItem);
int CombatArtActionCommandUnhover(struct MenuProc * menu, struct MenuItemProc * menuItem);
extern const struct MenuDef gCombatArtItemMenuDef;
extern const struct SelectInfo gSelectInfoCombatArt;

/* Misc */
bool CanUnitPlayCombatArt(struct Unit * unit, u16 item);
u8 GetBestRangeBonusCid(struct Unit * unit, u16 item);
const u8 * GetCombatArtIcon(const u8 cid);
int WeaponRangeGetterCombatArt(int range, struct Unit * unit, u16 item);
void PreBattleCalcCombatArt(struct BattleUnit * bu, struct BattleUnit * defender);
bool PostActionCombatArtEffect(ProcPtr parent);
