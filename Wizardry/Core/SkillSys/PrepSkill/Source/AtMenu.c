#include "common-chax.h"
#include "prep-skill.h"
#include "kernel-lib.h"
#include "./jester_headers/procs.h"

struct ChapterStatusProc {
    /* 00 */ PROC_HEADER;

    /* 29 */ u8 restoreStateOnExit;
    /* 2A */ u8 focusUnitOnExit;
    /* 2B */ u8 timesCompleted;
    /* 2C */ u8 numObjectiveTextLines;
    /* 2D */ u8 unk_2d;
    /* 2E */ u8 unitIndex;
    /* 2F */ u8 unk_2f;
    /* 30 */ u8 numAllyUnits;
    /* 31 */ u8 numEnemyUnits;

    /* 34 */ struct Unit* units[2];

    /* 3C */ u16 unk_3c;
    /* 3E */ u8 helpTextActive;
    /* 3F */ u8 unk_3f;
    /* 40 */ u8 unk_40;

    /* 41 */ u8 _pad[0x64-0x41];

    /* 64 */ u16 unk_64;
};

void sub_8095C2C(struct ProcAtMenu * proc);

void PrepScreenMenu_OnEquip(struct ProcAtMenu * proc)
{
    if (gpKernelDesigerConfig->equip_skill_en == false)
    {
        /* Check map */
        Proc_Goto(proc, 0x5);
        return;
    }

    proc->state = 6;
    Proc_Goto(proc, 0xA);
};

LYN_REPLACE_CHECK(AtMenu_StartSubmenu);
void AtMenu_StartSubmenu(struct ProcAtMenu * proc)
{
    sub_8095C2C(proc);

    switch (proc->state) {
    case 5: /* Chapter Status */
#ifdef CONFIG_INSTALL_PREPS_AUGURY
        Proc_StartBlocking(PREEXT_Procs_Augury, proc);
#else
        sub_808E79C(proc);
#endif
        break;

    case 2: /* Items */
        StartPrepItemScreen(proc);
        break;

    case 1: /* Pick Units */
        Proc_StartBlocking(ProcScr_PrepUnitScreen, proc);
        break;

    case 4: /* Support */
        StartFortuneSubMenu(2, proc);
        break;

    case 3: /* Save */
        StartPrepSaveScreen(proc);
        break;

    case 6: /* Equip */
        StartPrepEquipScreen(proc);
        break;

    default:
        break;
    }

    Proc_Break(proc);
};