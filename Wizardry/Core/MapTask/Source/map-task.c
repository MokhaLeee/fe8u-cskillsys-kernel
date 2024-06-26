#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"

/**
 * Hpbar
 */
static inline int GetUnitHpBarIndex(struct Unit * unit)
{
    return Div(unit->curHP * 10, unit->maxHP);
}

static inline void PutUnitHpBar(struct Unit * unit, int ix, int iy)
{
    static const int chrs[] = {
              0x74, 0x74,
        0x56, 0x54, 0x52,
        0x36, 0x34, 0x32,
        0x16, 0x14, 0x12
    };

    u32 oam2 = OAM2_PAL(0) +
               OAM2_LAYER(2) +
               OAM2_CHR(chrs[GetUnitHpBarIndex(unit)]);

    CallARM_PushToSecondaryOAM(
        OAM1_X(0x200 + ix),
        OAM0_Y(0x100 + iy + 10),
        gObject_16x8,
        oam2);
}

/**
 * Debuff
 */
static inline void PutUnitStatusBuffIcon(struct Unit * unit, int ix, int iy)
{
    CallARM_PushToSecondaryOAM(
        OAM1_X(0x200 + ix - 1),
        OAM0_Y(0x100 + iy),
        sSprite_SysUpArrowA,
        OAM2_LAYER(2));
}

static inline void PutUnitStatusDebuffIcon(struct Unit * unit, int ix, int iy)
{
    CallARM_PushToSecondaryOAM(
        OAM1_X(0x200 + ix - 1),
        OAM0_Y(0x100 + iy),
        sSprite_SysDownArrowA,
        OAM2_LAYER(2));
}

static inline void PutUnitStatusDefaultIcon(struct Unit * unit, int type, int ix, int iy)
{
    switch (type) {
    case STATUS_DEBUFF_NEGATIVE:
        PutUnitStatusDebuffIcon(unit, ix, iy);
        break;

    case STATUS_DEBUFF_POSITIVE:
        PutUnitStatusBuffIcon(unit, ix, iy);
        break;
    }
}

static inline void PutUnitDebuffIcon(struct Unit * unit, int ix, int iy)
{
    int debuff = GetUnitStatusIndex(unit);
    const struct DebuffInfo * debuf_info = &gpDebuffInfos[debuff];
    if (debuff != UNIT_STATUS_NONE)
    {
        if (debuf_info->on_draw)
            debuf_info->on_draw(unit, ix, iy);
        else
            PutUnitStatusDefaultIcon(unit, debuf_info->positive_type, ix, iy);
    }
    else
        PutUnitStatusDefaultIcon(unit, SimulateStatDebuffPositiveType(unit), ix, iy);
}

/**
 * Event
 */
static inline void PutRescuingIcon(struct Unit * unit, int ix, int iy)
{
    static const u16 pal_idx[] = {
        0xC, 0xE, 0xD,
    };

    int faction = unit->rescue >> 6;

    CallARM_PushToSecondaryOAM(
        OAM1_X(0x200 + ix + 9),
        OAM0_Y(0x100 + iy + 7),
        gObject_8x8,
        OAM2_PAL(pal_idx[faction]) + OAM2_LAYER(2) + OAM2_CHR(0x60 / 0x20));
}

static inline void PutBossIcon(struct Unit * unit, int ix, int iy)
{
    CallARM_PushToSecondaryOAM(
        OAM1_X(0x200 + ix + 9),
        OAM0_Y(0x100 + iy + 7),
        gObject_8x8,
        OAM2_PAL(0) + OAM2_LAYER(2) + OAM2_CHR(0x200 / 0x20));
}

static inline void PutProtectIcon(struct Unit * unit, int ix, int iy)
{
    CallARM_PushToSecondaryOAM(
        OAM1_X(0x200 + ix + 9),
        OAM0_Y(0x100 + iy + 7),
        gObject_8x8,
        OAM2_PAL(0) + OAM2_LAYER(2) + OAM2_CHR(0x220 / 0x20));
}

static inline void PutUnitMiscIcon(struct Unit * unit, int ix, int iy)
{
    if (unit->state & US_RESCUING)
        PutRescuingIcon(unit, ix, iy);
    else if ((UNIT_FACTION(unit) != FACTION_BLUE) && (UNIT_CATTRIBUTES(unit) & CA_BOSS))
        PutBossIcon(unit, ix, iy);
    else if (GetBattleMapKind() != 2 && UNIT_CHAR_ID(unit) == GetROMChapterStruct(gPlaySt.chapterIndex)->protectCharacterIndex)
        PutProtectIcon(unit, ix, iy);
}

static void _PutUnitSpriteIconsOam(struct Unit * unit)
{
    int ix = unit->xPos * 16 - gBmSt.camera.x;
    int iy = unit->yPos * 16 - gBmSt.camera.y;

    if (ix < -16 || ix > DISPLAY_WIDTH)
        return;

    if (iy < -16 || iy > DISPLAY_HEIGHT)
        return;

    if (unit->curHP != unit->maxHP)
        PutUnitHpBar(unit, ix, iy);

    if (!((GetGameClock() & 0x3F) >= 0x28))
        PutUnitDebuffIcon(unit, ix, iy);
    else
        PutUnitMiscIcon(unit, ix, iy);
}

/* LynJump */
void PutUnitSpriteIconsOam(void)
{
    int i;

    if (CheckFlag(0x84) != 0)
        return;

    PutChapterMarkedTileIconOam();

    for (i = FACTION_BLUE + 1;  i <= (FACTION_BLUE  + CONFIG_UNIT_AMT_ALLY);  i++)
    {
        struct Unit * unit = GetUnit(i);
        if (!UNIT_IS_VALID(unit) || unit->state & US_HIDDEN)
            continue;

        // GetUnitSpriteHideFlag
        if (!unit->pMapSpriteHandle || (unit->pMapSpriteHandle->config & 0x80))
            continue;

        _PutUnitSpriteIconsOam(unit);
    }

    for (i = FACTION_RED + 1;   i <= (FACTION_RED   + CONFIG_UNIT_AMT_ENEMY); i++)
    {
        struct Unit * unit = GetUnit(i);
        if (!UNIT_IS_VALID(unit) || unit->state & US_HIDDEN)
            continue;

        // GetUnitSpriteHideFlag
        if (!unit->pMapSpriteHandle || (unit->pMapSpriteHandle->config & 0x80))
            continue;

        _PutUnitSpriteIconsOam(unit);
    }

    for (i = FACTION_GREEN + 1; i <= (FACTION_GREEN + CONFIG_UNIT_AMT_NPC);   i++)
    {
        struct Unit * unit = GetUnit(i);
        if (!UNIT_IS_VALID(unit) || unit->state & US_HIDDEN)
            continue;

        // GetUnitSpriteHideFlag
        if (!unit->pMapSpriteHandle || (unit->pMapSpriteHandle->config & 0x80))
            continue;

        _PutUnitSpriteIconsOam(unit);
    }
}
