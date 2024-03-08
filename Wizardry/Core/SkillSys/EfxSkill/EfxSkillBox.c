#include "global.h"
#include "icon.h"
#include "ctc.h"
#include "anime.h"
#include "hardware.h"
#include "fontgrp.h"
#include "bmlib.h"
#include "ekrbattle.h"

#include "common-chax.h"
#include "efx-skill.h"
#include "icon-rework.h"
#include "skill-system.h"
#include "combat-art.h"

struct ProcEfxskillbox {
    PROC_HEADER;
    u8 sid;
    int timer;
    int msg;
    const u8 * icon;
    struct Anim * anim;
    struct Anim * anim_icon, * anim_text, * anim_bg;
    int skill_or_combatart;
    struct Text text;
};

#define ICON_OBJ_CHR 0x40
#define ICON_OBJ_PAL 0x2

#define TEXT_OBJ_CHR 0x42
#define TEXT_OBJ_PAL 0xF

#define TEXT_BG_OBJ_CHR 0x00

STATIC_DECLAR void DrawEfxSkillName(struct ProcEfxskillbox * proc)
{
    struct Font _font;
    struct Text * text = &proc->text;
    struct Font * font = &_font;
    char * str;

    ApplyPalette((u16 *)Pal_Text, 0x10 + TEXT_OBJ_PAL);

    InitSpriteTextFont(font, OBJ_VRAM0 + TEXT_OBJ_CHR * 0x20, TEXT_OBJ_PAL);
    InitSpriteText(text);
    SetTextFont(font);
    SetTextFontGlyphs(TEXT_GLYPHS_SYSTEM);
    SpriteText_Clear(text);

    if (proc->skill_or_combatart == EFX_SKILL_BOX_SKILL)
        str = GetSkillNameStr(proc->sid);
    else
        str = GetStringFromIndex(GetCombatArtName(proc->sid));

    Text_InsertDrawString(
        text,
        GetStringTextCenteredPos(0x30, str),
        TEXT_COLOR_SYSTEM_WHITE,
        str);

    SetTextFont(NULL);
}

STATIC_DECLAR void EfxSkillBoxOnDraw(struct ProcEfxskillbox * proc)
{
    struct Anim * anim;

    LoadIconPalette(0, 0x10 + ICON_OBJ_PAL);
    Copy2dChr(proc->icon, OBJ_VRAM0 + ICON_OBJ_CHR * 0x20, 2, 2);

    /* Icon */
    if (GetAnimPosition(proc->anim) == EKR_POS_L)
    {
        anim = AnimCreate(AnimScr_EfxSkillBoxIconLeft, 0x95);
        anim->xPosition = 0x30;
    }
    else
    {
        anim = AnimCreate(AnimScr_EfxSkillBoxIconRight, 0x95);
        anim->xPosition = 0xF0;
    }
    anim->oam2Base = OAM2_PAL(ICON_OBJ_PAL) + OAM2_LAYER(0b01) + OAM2_CHR(ICON_OBJ_CHR);
    anim->yPosition = 0x60 - 0x3;
    proc->anim_icon = anim;

    /* Name */
    DrawEfxSkillName(proc);

    if (GetAnimPosition(proc->anim) == EKR_POS_L)
    {
        anim = AnimCreate(AnimScr_EfxSkillBoxNameLeft, 0x95);
        anim->xPosition = 0x00;
    }
    else
    {
        anim = AnimCreate(AnimScr_EfxSkillBoxNameRight, 0x95);
        anim->xPosition = 0x100;
    }
    anim->oam2Base = OAM2_PAL(TEXT_OBJ_PAL) + OAM2_LAYER(0b01) + OAM2_CHR(TEXT_OBJ_CHR);
    anim->yPosition = 0x60 - 0x3;
    proc->anim_text = anim;

    /* Text BG */
    Decompress(Img_EfxSkillBoxBG, gGenericBuffer);
    Copy2dChr(gGenericBuffer, OBJ_VRAM0 + TEXT_BG_OBJ_CHR * 0x20, 6, 2);

    if (GetAnimPosition(proc->anim) == EKR_POS_L)
    {
        anim = AnimCreate(AnimScr_EfxSkillBoxNameLeft, 0x95);
        anim->xPosition = 0x00;
    }
    else
    {
        anim = AnimCreate(AnimScr_EfxSkillBoxNameRight, 0x95);
        anim->xPosition = 0x100;
    }
    anim->oam2Base = OAM2_PAL(ICON_OBJ_PAL) + OAM2_LAYER(0b01) + OAM2_CHR(TEXT_BG_OBJ_CHR);
    anim->yPosition = 0x60 - 0x3;
    proc->anim_bg = anim;
}

STATIC_DECLAR void EfxSkillBoxIdle(struct ProcEfxskillbox * proc)
{
    if (proc->anim_text && proc->anim_text->state != 0)
        return;

    if (proc->anim_icon && proc->anim_icon->state != 0)
        return;

    if (proc->anim_bg   && proc->anim_bg->state   != 0)
        return;

    Proc_Break(proc);
}

STATIC_DECLAR const struct ProcCmd ProcScr_EfxSkillBox[] = {
    PROC_NAME("EfxSkillBox"),
    PROC_YIELD,
    PROC_CALL(EfxSkillBoxOnDraw),
    PROC_REPEAT(EfxSkillBoxIdle),
    PROC_END
};

void NewEfxSkillBox(struct Anim * anim, int msg, const u8 * icon, u8 sid, int skill_or_combatart)
{
    struct ProcEfxskillbox * proc;
    proc = Proc_Start(ProcScr_EfxSkillBox, PROC_TREE_3);
    proc->timer = 0;
    proc->anim = anim;
    proc->sid = sid;
    proc->skill_or_combatart = skill_or_combatart;
    proc->msg = msg;
    proc->icon = icon;

    if (proc->icon == NULL)
    {
        if (skill_or_combatart == EFX_SKILL_BOX_SKILL)
            proc->icon = GetSkillIcon(sid);
        else
            proc->icon = GetCombatArtIcon(sid);
    }
}

bool EfxSkillBoxExists(void)
{
    return !!Proc_Find(ProcScr_EfxSkillBox);
}
