#pragma once

#include "common-chax.h"
#include "skill-system.h"

#define COMBART_VALID(cid) (((cid) > 0) && ((cid) < 0xFF))
#define COMBART_ICON(cid) ((1 << 8) + (cid))

#define CA_WTYPE_ANY ((u8)(-1))

struct CombatArtInfo {
    const u8 * icon;
    u16 name, desc;

    u8 wtype;
    s8 range_bonus;
    s16 cost;

    struct {
        s8 atk, def, hit, avo, crit, silencer, dodge, _pad_;
    } battle_status;

    bool double_attack;
    bool magic_attack;
    u8 effectiveness;

    /* debuffs */
    u8 debuff;
    bool aoe_debuff;

    u8 _pad_[7];
};

enum combat_art_effectiveness {
    /* CombatArtInfo::effectiveness */
    COMBART_EFF_NONE,
    COMBART_EFF_ALL,
    COMBART_EFF_ARMOR,
    COMBART_EFF_CAVALRY,
    COMBART_EFF_FLIER,
    COMBART_EFF_DRAGON,
    COMBART_EFF_MONSTER,
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
int CombatArtActionCommandOnDarw(struct MenuProc * menu, struct MenuItemProc * item);
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
