#include "global.h"
#include "bmunit.h"
#include "prepscreen.h"
#include "hardware.h"
#include "fontgrp.h"
#include "uiutils.h"
#include "soundwrapper.h"
#include "mapanim.h"
#include "bmusemind.h"

#include "common-chax.h"
#include "strmag.h"
#include "status-getter.h"

/* LynJump */
void DrawPrepScreenItemUseStatLabels(struct Unit * unit)
{
    int i;
    char * str;
    struct Text * text = gPrepItemTexts;

    for (i = 0; i < 8; i++)
        ClearText(&text[i]);

    /* HP */
    PutDrawText(
        text++,
        TILEMAP_LOCATED(gBG2TilemapBuffer, 16, 3),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4E9)
    );

    /* Str[.] */
    PutDrawText(
        text++,
        TILEMAP_LOCATED(gBG2TilemapBuffer, 16, 5),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4FE)
    );

    /* Mag[.] */
    PutDrawText(
        text++,
        TILEMAP_LOCATED(gBG2TilemapBuffer, 16, 7),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4FF)
    );

    /* Luck */
    PutDrawText(
        text++,
        TILEMAP_LOCATED(gBG2TilemapBuffer, 16, 9),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4EE)
    );

    /* Skill[.] */
    PutDrawText(
        text++,
        TILEMAP_LOCATED(gBG2TilemapBuffer, 23, 3),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4EC)
    );

    /* Spd[.] */
    PutDrawText(
        text++,
        TILEMAP_LOCATED(gBG2TilemapBuffer, 23, 5),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4ED)
    );

    /* Def[.] */
    PutDrawText(
        text++,
        TILEMAP_LOCATED(gBG2TilemapBuffer, 23, 7),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4EF)
    );

    /* Res[.] */
    PutDrawText(
        text++,
        TILEMAP_LOCATED(gBG2TilemapBuffer, 23, 9),
        TEXT_COLOR_SYSTEM_GOLD,
        0, 0,
        GetStringFromIndex(0x4F0)
    );

    str = GetStringFromIndex(unit->pClassData->nameTextId);
    PutDrawText(
        text++,
        TILEMAP_LOCATED(gBG2TilemapBuffer, 21, 1),
        TEXT_COLOR_SYSTEM_WHITE,
        GetStringTextCenteredPos(0x40, str),
        0, str
    );

    /* LV */
    PutTwoSpecialChar(TILEMAP_LOCATED(gBG2TilemapBuffer, 17, 1), 3, 0x24, 0x25);
}

/* LynJump */
void DrawPrepScreenItemUseStatBars(struct Unit * unit, int mask)
{
    int ix, iy, stat_pack[8];
    UnpackUiBarPalette(2);

    stat_pack[0] = GetUnitCurrentHp(unit) * 24 / UNIT_MHP_MAX(unit);
    stat_pack[1] = PowGetter(unit) * 24 / UNIT_POW_MAX(unit);
    stat_pack[2] = MagGetter(unit) * 24 / GetUnitMaxMagic(unit);
    stat_pack[3] = LckGetter(unit) * 24 / UNIT_LCK_MAX(unit);
    stat_pack[4] = SklGetter(unit) * 24 / UNIT_SKL_MAX(unit);
    stat_pack[5] = SpdGetter(unit) * 24 / UNIT_SPD_MAX(unit);
	stat_pack[6] = DefGetter(unit) * 24 / UNIT_DEF_MAX(unit);
	stat_pack[7] = ResGetter(unit) * 24 / UNIT_RES_MAX(unit);

    for (iy = 0; iy < 4; iy++)
    {
        for (ix = 0; ix < 2; ix++)
        {
            int index = iy + 4 * ix; /* This is quite different */

            LogDebugf("%s: [index %d]=%d, at x=%d y=%d",
                    __func__, index, stat_pack[index], ix, iy);

            DrawStatBarGfx(
                0x380 + 0x8 * index, 4,
                TILEMAP_LOCATED(gBG0TilemapBuffer, 18 + 7 * ix, 4 + 2 * iy),
                mask & (1 << index)
                    ? TILEREF(0, 3)
                    : TILEREF(0, 2),
                24, stat_pack[index], 0);
        }
    }
    BG_EnableSyncByMask(BG0_SYNC_BIT);
}

/* LynJump */
void DrawPrepScreenItemUseStatValues(struct Unit* unit)
{
    // HP
    PutNumberOrBlank( 
        TILEMAP_LOCATED(gBG2TilemapBuffer, 20, 3),
        (GetUnitCurrentHp(unit) == UNIT_MHP_MAX(unit)) 
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        GetUnitCurrentHp(unit)
    );

    // POW
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 20, 5),
        (PowGetter(unit) == UNIT_POW_MAX(unit)) 
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        PowGetter(unit)
    );

    // MAG
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 20, 7),
        (MagGetter(unit) == GetUnitMaxMagic(unit)) 
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        MagGetter(unit)
    );

    // LCK
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 20, 9),
        (LckGetter(unit) == UNIT_LCK_MAX(unit)) 
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        LckGetter(unit)
    );

    // SKL
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 27, 3),
        (SklGetter(unit) == UNIT_SKL_MAX(unit)) 
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        SklGetter(unit)
    );

    // SPD
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 27, 5),
        (SpdGetter(unit) == UNIT_SPD_MAX(unit)) 
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        SpdGetter(unit)
    );

    // DEF
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 27, 7),
        (DefGetter(unit) == UNIT_DEF_MAX(unit)) 
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        DefGetter(unit)
    );

    // RES
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 27, 9),
        (ResGetter(unit) == UNIT_RES_MAX(unit)) 
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        ResGetter(unit)
    );

    TileMap_FillRect(TILEMAP_LOCATED(gBG2TilemapBuffer, 19, 1), 1, 1, 0);

    // LV
    PutNumberOrBlank(
        TILEMAP_LOCATED(gBG2TilemapBuffer, 20, 1),
        TEXT_COLOR_SYSTEM_BLUE,
        unit->level
    );
    BG_EnableSyncByMask(BG2_SYNC_BIT);
}

/* LynJump */
void PrepItemUseBooster_OnInit(struct ProcPrepItemUseBooster * proc)
{
    int i, item, msg;
    struct ProcPrepItemUse * parent = proc->proc_parent;

    StartManimLevelUpStatGainLabels(0x1C0, 3, 0, proc);

    proc->status_pre[0] = GetUnitCurrentHp(parent->unit);
    proc->status_pre[1] = PowGetter(parent->unit);
    proc->status_pre[2] = MagGetter(parent->unit);
    proc->status_pre[3] = LckGetter(parent->unit);
    proc->status_pre[4] = SklGetter(parent->unit);
    proc->status_pre[5] = SpdGetter(parent->unit);
    proc->status_pre[6] = DefGetter(parent->unit);
    proc->status_pre[7] = ResGetter(parent->unit);

    item = parent->unit->items[parent->slot];

    msg = ApplyStatBoostItem(parent->unit, parent->slot);
    
    DrawPrepScreenItemUseStatBars(parent->unit, 0);
    DrawPrepScreenItemUseStatValues(parent->unit);

    proc->status_pst[0] = GetUnitCurrentHp(parent->unit);
    proc->status_pst[1] = PowGetter(parent->unit);
    proc->status_pst[2] = MagGetter(parent->unit);
    proc->status_pst[3] = LckGetter(parent->unit);
    proc->status_pst[4] = SklGetter(parent->unit);
    proc->status_pst[5] = SpdGetter(parent->unit);
    proc->status_pst[6] = DefGetter(parent->unit);
    proc->status_pst[7] = ResGetter(parent->unit);

    PrepItemUseBooster_OnDraw(proc, 0xE, 0xE, msg, item);

    for (i = 0; i < EKRLVUP_STAT_MAX; i++) {
        if (proc->status_pre[i] == proc->status_pst[i])
            continue;

        StartPrepItemBoostStatGainLabelAnim(
            (i >> 2) * 56 + 0xB0,
            (i  & 3) * 16 + 0x2A,
            proc->status_pst[i] - proc->status_pre[i]
        );
    }

    proc->timer = 0x78;
    PlaySoundEffect(0x5A);
}
