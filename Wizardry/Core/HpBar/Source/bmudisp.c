#include "common-chax.h"
#include "debuff.h"

/* External */
void PutUnitHpBar(struct Unit * unit);

/* Vanilla port */
void PutChapterMarkedTileIconOam(void);

STATIC_DECLAR void PutSubIconCommon(int x, int y, int oam2)
{
    if (x < -16 || x > DISPLAY_WIDTH)
        return;

    if (y < -16 || y > DISPLAY_HEIGHT)
        return;

    if ((GetGameClock() & 0x3F) >= 0x28)
        return;

    CallARM_PushToSecondaryOAM(
        OAM1_X(0x200 + x + 9),
        OAM0_Y(0x100 + y + 7),
        gObject_8x8,
        oam2);
}

STATIC_DECLAR void PutRescuingIcon(struct Unit * unit)
{
    const u16 rescuePalLut[] = {
        0xC, 0xE, 0xD,
    };

    u8 faction = unit->rescue >> 6;

    PutSubIconCommon(
        unit->xPos * 16 - gBmSt.camera.x,
        unit->yPos * 16 - gBmSt.camera.y,
        OAM2_PAL(rescuePalLut[faction]) + OAM2_LAYER(2) + OAM2_CHR(0x60 / 0x20)
    );
}

STATIC_DECLAR void PutBossIcon(struct Unit * unit)
{
    PutSubIconCommon(
        unit->xPos * 16 - gBmSt.camera.x,
        unit->yPos * 16 - gBmSt.camera.y,
        OAM2_PAL(0) + OAM2_LAYER(2) + OAM2_CHR(0x200 / 0x20)
    );
}

STATIC_DECLAR void PutProtectIcon(struct Unit * unit)
{
    PutSubIconCommon(
        unit->xPos * 16 - gBmSt.camera.x,
        unit->yPos * 16 - gBmSt.camera.y,
        OAM2_PAL(0) + OAM2_LAYER(2) + OAM2_CHR(0x220 / 0x20)
    );
}

static inline _PutUnitSpriteIconsOam(struct Unit * unit)
{
    if (!UNIT_IS_VALID(unit))
        return;

    if (unit->state & US_HIDDEN)
        return;

    if (GetUnitSpriteHideFlag(unit) != 0)
        return;

    /* Hp bar */
    PutUnitHpBar(unit);

    /* In DebuffRework */
    PutUnitStatusIcon(unit);

    if (unit->state & US_RESCUING)
        PutRescuingIcon(unit);
    else if ((UNIT_FACTION(unit) != FACTION_BLUE) && (UNIT_CATTRIBUTES(unit) & CA_BOSS))
        PutBossIcon(unit);
    else if (GetBattleMapKind() != 2 && UNIT_CHAR_ID(unit) == GetROMChapterStruct(gPlaySt.chapterIndex)->protectCharacterIndex)
        PutProtectIcon(unit);
}

/* LynJump */
void PutUnitSpriteIconsOam(void)
{
    int i;

    if (CheckFlag(0x84) != 0)
        return;

    PutChapterMarkedTileIconOam();

    for (i = FACTION_BLUE + 1;  i <= (FACTION_BLUE  + CONFIG_UNIT_AMT_ALLY);  i++) _PutUnitSpriteIconsOam(GetUnit(i));
    for (i = FACTION_RED + 1;   i <= (FACTION_RED   + CONFIG_UNIT_AMT_ENEMY); i++) _PutUnitSpriteIconsOam(GetUnit(i));
    for (i = FACTION_GREEN + 1; i <= (FACTION_GREEN + CONFIG_UNIT_AMT_NPC);   i++) _PutUnitSpriteIconsOam(GetUnit(i));
}
