#include "global.h"
#include "hardware.h"
#include "bmlib.h"
#include "ctc.h"
#include "ap.h"
#include "icon.h"
#include "fontgrp.h"
#include "statscreen.h"

#include "common-chax.h"
#include "skill-system.h"
#include "prep-skill.h"
#include "constants/texts.h"

STATIC_DECLAR void ReloadPrepSkill2IconGfx(struct ProcPrepSkillObj * proc)
{
    int x, y;
    struct ProcPrepSkill2 * pproc = proc->proc_parent;
    struct PrepEquipSkillList * rlist = GetPrepEquipSkillList(proc->unit);

    for (y = 0; y < PREP_SRLIST_HEIGHT; y++)
    {
        for (x = 0; x < PREP_SRLIST_LENGTH; x++)
        {
            int off  = PREP_SRLIST_OFFSET(x, y);
            int real = PREP_SRLIST_OFFSET(x, pproc->right_line + y);
            u8 sid = rlist->sid[real];
            u8 * dst = off < 0x10
                     ? OBJ_VRAM0 + VOBJ_SKILL_ICONS + 0x40 * off
                     : OBJ_VRAM0 + VOBJ_SKILL_ICONS + 0x40 * (off + 0x10);

            if (real >= rlist->amt)
                break;

            /* Copy gfx to ObjTile */
            Copy2dChr(GetSkillIcon(sid), dst, 2, 2);
        }
    }
    proc->reload = false;
}

STATIC_DECLAR void PutPrepSkill2IconSprits(struct ProcPrepSkillObj * proc)
{
    int x, y;
    struct ProcPrepSkill2 * pproc = proc->proc_parent;
    struct PrepEquipSkillList * rlist = GetPrepEquipSkillList(proc->unit);

    for (y = 0; y < PREP_SRLIST_HEIGHT; y++)
    {
        for (x = 0; x < PREP_SRLIST_LENGTH; x++)
        {
            int off  = PREP_SRLIST_OFFSET(x, y);
            int real = PREP_SRLIST_OFFSET(x, pproc->right_line + y);
            u8 * src = off < 0x10
                     ? OBJ_VRAM0 + VOBJ_SKILL_ICONS + 0x40 * off
                     : OBJ_VRAM0 + VOBJ_SKILL_ICONS + 0x40 * (off + 0x10);

            int ix = 0x72 + 0x10 * x;
            int iy = 0x1E + 0x10 * y;

            int oam2 = OAM2_PAL(SKILLOBJ_PAL) +
                       OAM2_LAYER(0b01) +
                       OAM2_CHR((u32)src / 0x20);

            if (real >= rlist->amt)
                break;

            PutSprite(5, ix, iy, gObject_16x16, oam2);
        }
    }
}

STATIC_DECLAR void PutPrepSkill2Suffix(struct ProcPrepSkillObj * proc)
{
    int x, y;
    struct ProcPrepSkill2 * pproc = proc->proc_parent;
    struct SkillList * llist = GetUnitSkillList(proc->unit);
    struct PrepEquipSkillList * rlist = GetPrepEquipSkillList(proc->unit);
    struct Unit * unit = pproc->unit;

    /* Draw R-list suffix */
    for (y = 0; y < PREP_SRLIST_HEIGHT; y++)
    {
        for (x = 0; x < PREP_SRLIST_LENGTH; x++)
        {
            int real = PREP_SRLIST_OFFSET(x, pproc->right_line + y);
            u8 sid = rlist->sid[real];

            if (real >= rlist->amt)
                break;

            if (SkillTester(unit, sid))
            {
                int ix = 0x72 + 0x10 * x + 8;
                int iy = 0x1E + 0x10 * y + 0;
                int oam2 = OAM2_PAL(SKILLOBJ_PAL) +
                           OAM2_LAYER(0b01) +
                           OAM2_CHR(SKILLOBJ_VOBJ / 0x20);

                if (CanRemoveSkill(unit, sid))
                    PutSprite(5, ix, iy, gObject_8x16, oam2);
                else
                    PutSprite(5, ix, iy, gObject_8x16, oam2 + 1);
            }
        }
    }

    /* Draw L-list suffix */
    for (y = 0; y < PREP_SLLIST_HEIGHT; y++)
    {
        for (x = 0; x < PREP_SLLIST_LENGTH; x++)
        {
            int real = PREP_SLLIST_OFFSET(x, pproc->left_line + y);
            u8 sid = llist->sid[real];

            if (real >= llist->amt)
                break;

            if (SkillTester(unit, sid))
            {
                int ix = 0x10 + 0x10 * x + 8;
                int iy = 0x30 + 0x10 * y + 0;
                int oam2 = OAM2_PAL(SKILLOBJ_PAL) +
                           OAM2_LAYER(0b01) +
                           OAM2_CHR(SKILLOBJ_VOBJ / 0x20);

                if (CanRemoveSkill(unit, sid))
                    PutSprite(5, ix, iy, gObject_8x16, oam2);
                else
                    PutSprite(5, ix, iy, gObject_8x16, oam2 + 1);
            }
        }
    }
}

STATIC_DECLAR void PrepSkillObj_OnInit(struct ProcPrepSkillObj * proc)
{
    Decompress(Gfx_ObjSkill, OBJ_VRAM0 + SKILLOBJ_VOBJ);
    ApplyPalette(Pal_ObjSkill, SKILLOBJ_PAL + 0x10);

    Decompress(Gfx_ObjWindow, OBJ_VRAM0 + OBJWINDOW_VOBJ);
    ApplyPalette(Pal_ObjWindow, OBJWINDOW_PAL + 0x10);
}

STATIC_DECLAR void PrepSkillObjMain(struct ProcPrepSkillObj * proc)
{
    if (proc->reload)
        ReloadPrepSkill2IconGfx(proc);

    if (!proc->active)
        return;

    PutPrepSkill2IconSprits(proc);

    if(GetGameClock() & (1 << 5))
        PutPrepSkill2Suffix(proc);
}

STATIC_DECLAR const struct ProcCmd ProcScr_PrepSkillObj[] = {
    PROC_NAME("PrepSkillObj"),
    PROC_CALL(PrepSkillObj_OnInit),
    PROC_REPEAT(PrepSkillObjMain),
    PROC_END
};

void NewPrepSkillObj(struct ProcPrepSkill2 * pproc)
{
    struct ProcPrepSkillObj * proc;

    proc = Proc_Find(ProcScr_PrepSkillObj);

    if (!proc)
        proc = Proc_Start(ProcScr_PrepSkillObj, pproc);

    proc->unit = pproc->unit;
    proc->reload = true;
    proc->active = true;
}

void EndPrepSkillObj(void)
{
    Proc_EndEach(ProcScr_PrepSkillObj);
}

void RegisterPrepSkillObjReload(void)
{
    struct ProcPrepSkillObj * proc;
    proc = Proc_Find(ProcScr_PrepSkillObj);

    if (proc)
        proc->reload = true;
}

void EnablePrepSkillObj(void)
{
    struct ProcPrepSkillObj * proc;
    proc = Proc_Find(ProcScr_PrepSkillObj);

    if (proc)
        proc->active = true;
}

void DisablePrepSkillObj(void)
{
    struct ProcPrepSkillObj * proc;
    proc = Proc_Find(ProcScr_PrepSkillObj);

    if (proc)
        proc->active = false;
}

void PutPrepSkill2PopupBox(int x, int y, int w, int h, int priority)
{
    PrepItemDrawPopupBox(x, y, w, h, OAM2_PAL(OBJWINDOW_PAL) + OAM2_LAYER(priority) + OAM2_CHR(OBJWINDOW_VOBJ / 0x20));
}

void PrepSkill2_DrawLeftSkillIcon(struct ProcPrepSkill2 * proc)
{
    int x, y;
    struct Unit * unit = proc->unit;
    struct SkillList * list = GetUnitSkillList(unit);
    ResetIconGraphics_();
    TileMap_FillRect(TILEMAP_LOCATED(gBG2TilemapBuffer, 1, 6), 0xA, 0x6, 0);

    for (y = 0; y < PREP_SLLIST_HEIGHT; y++)
    {
        for (x = 0; x < PREP_SLLIST_LENGTH; x++)
        {
            int count = x + y * PREP_SLLIST_LENGTH;
            if (count >= list->amt)
                break;

            DrawIcon(
                TILEMAP_LOCATED(gBG2TilemapBuffer, 2 + x * 2, 6 + y * 2),
                SKILL_ICON(list->sid[count]), 
                TILEREF(0, STATSCREEN_BGPAL_ITEMICONS));
        }
    }
    BG_EnableSyncByMask(BG2_SYNC_BIT);
}

/* Texts */
void PrepSkill2_InitTexts(void)
{
    ResetText();

    /* Skill desc */
    InitText(&gPrepUnitTexts[0x00], 0x15);
    InitText(&gPrepUnitTexts[0x01], 0x15);
    InitText(&gPrepUnitTexts[0x02], 0x15);

    /* Failed to add skill */
    InitText(&gPrepUnitTexts[0x03], 0x12);
    InitText(&gPrepUnitTexts[0x04], 0x12);

    /* Don't touch: Left unit name & Right top bar*/
    InitText(&gPrepUnitTexts[0x13], 7);
    InitText(&gPrepUnitTexts[0x14], 10);
    InitText(&gPrepUnitTexts[0x15], 5);
}

/* Skill desc */
void PrepSkill2_DrawDrawSkillDesc(struct ProcPrepSkill2 * proc)
{
    u8 sid;
    int i;
    const char * str;

    const int X = 0x5;
    const int Y = 0xD;

    if (proc->hand_pos == POS_L)
    {
        struct SkillList * llist = GetUnitSkillList(proc->unit);
        int index = PREP_SLLIST_OFFSET(proc->hand_x, proc->left_line + proc->hand_y);
        sid = llist->sid[index];
    }
    else
    {
        struct PrepEquipSkillList * rlist = GetPrepEquipSkillList(proc->unit);
        int index = PREP_SRLIST_OFFSET(proc->hand_x, proc->right_line + proc->hand_y);
        sid = rlist->sid[index];
    }

    TileMap_FillRect(TILEMAP_LOCATED(gBG0TilemapBuffer, X, Y), 0x10, 0x6, 0);

    str = GetSkillDescStr(sid);

    for (i = 0; i < 3 && '\0' != *str; i++)
    {
        struct Text * text = &gPrepUnitTexts[0x00 + i];
        ClearText(text);
        PutDrawText(
            text,
            TILEMAP_LOCATED(gBG0TilemapBuffer, X, Y + 2 * i),
            TEXT_COLOR_SYSTEM_WHITE, 0, 0, str
        );

        while ('\1' != *str++)
            if ('\0' == *str)
                break;
    }
    BG_EnableSyncByMask(BG0_SYNC_BIT);
}

void PrepSkill2_DrawRightTopBar(struct ProcPrepSkill2 * proc)
{
    struct Text * text = &gPrepUnitTexts[0x15];
    struct Unit * unit = proc->unit;
    int color = AddSkill(unit, 0) == 0
              ? TEXT_COLOR_SYSTEM_WHITE
              : TEXT_COLOR_SYSTEM_GREEN;
    struct SkillList * llist = GetUnitSkillList(unit);

    ClearText(text);
    TileMap_FillRect(TILEMAP_LOCATED(gBG0TilemapBuffer, 0x10, 0x1), 0xA, 0x1, 0);

    PutNumber(
        TILEMAP_LOCATED(gBG0TilemapBuffer, 0x12, 0x1),
        color,
        llist->amt
    );

    PutDrawText(
        text,
        TILEMAP_LOCATED(gBG0TilemapBuffer, 0x16, 0x1),
        TEXT_COLOR_SYSTEM_WHITE, 0, 0,
        GetStringFromIndex(MSG_PREPSKILL_RightTopBar)
    );
}
