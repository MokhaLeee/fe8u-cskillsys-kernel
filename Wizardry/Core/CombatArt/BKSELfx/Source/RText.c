#include "global.h"
#include "bksel.h"
#include "fontgrp.h"
#include "statscreen.h"

#include "common-chax.h"
#include "stat-screen.h"
#include "battle-system.h"
#include "help-box.h"
#include "combat-art.h"

static _DECL_INFO RText_S_0544, RText_S_0571, RText_S_055D, RText_S_055F, RText_S_CombatArt;
static _DECL_INFO RText_E_0544, RText_E_055C, RText_E_0573, RText_E_055D, RText_E_055F, RText_E_0572, RText_E_CombatArt;

_DECL_INFO * const RTextCombatBkselStandard = &RText_S_CombatArt;
_DECL_INFO * const RTextCombatBkselExtended = &RText_E_CombatArt;

_DECL_INFO * const RTextCombatBkselStandardNoArts = &RText_S_0544;
_DECL_INFO * const RTextCombatBkselExtendedNoArts = &RText_E_0544;

STATIC_DECLAR void HbPopuplate_CombatArtBKSEL(struct HelpBoxProc * proc)
{
    int cid = GetCombatArtInForce(gActiveUnit);
    proc->mid = GetCombatArtDesc(cid);

    sHelpBoxType = NEW_HB_COMBAT_ART_BKSEL;
}

STATIC_DECLAR void HbPopuplate_NotCombatArtBKSEL(struct HelpBoxProc * proc)
{
    sHelpBoxType = 0;
}

STATIC_DECLAR void HbRedirect_CombatArtBKSEL(struct HelpBoxProc * proc)
{
    int cid = GetCombatArtInForce(gActiveUnit);
    if (COMBART_VALID(cid))
        return;

    switch (proc->moveKey) {
    case DPAD_DOWN:
        TryRelocateHbDown(proc);
        break;

    case DPAD_UP:
        TryRelocateHbUp(proc);
        break;
    }
}

/* Standard */
static _DECL_INFO RText_S_0544 = {
    &RText_S_CombatArt, &RText_S_0571, NULL, NULL,
    28, 24, 0x544,
    NULL, HbPopuplate_NotCombatArtBKSEL
};

static _DECL_INFO RText_S_0571 = {
    &RText_S_0544, &RText_S_055D, NULL, NULL,
    28, 40, 0x571,
    NULL, HbPopuplate_NotCombatArtBKSEL
};

static _DECL_INFO RText_S_055D = {
    &RText_S_0571, &RText_S_055F, NULL, NULL,
    28, 56, 0x55D,
    NULL, HbPopuplate_NotCombatArtBKSEL
};

static _DECL_INFO RText_S_055F = {
    &RText_S_055D, &RText_S_CombatArt, NULL, NULL,
    28, 72, 0x55F,
    NULL, HbPopuplate_NotCombatArtBKSEL
};

static _DECL_INFO RText_S_CombatArt = {
    &RText_S_055F, &RText_S_0544, NULL, NULL,
    28, 104, 0,
    HbRedirect_CombatArtBKSEL, HbPopuplate_CombatArtBKSEL
};

/* Extended */
static _DECL_INFO RText_E_0544 = {
    &RText_E_CombatArt, &RText_E_055C, NULL, NULL,
    28, 24, 0x544,
    NULL, HbPopuplate_NotCombatArtBKSEL
};

static _DECL_INFO RText_E_055C = {
    &RText_E_0544, &RText_E_0573, NULL, NULL,
    28, 40, 0x55C,
    NULL, HbPopuplate_NotCombatArtBKSEL
};

static _DECL_INFO RText_E_0573 = {
    &RText_E_055C, &RText_E_055D, NULL, NULL,
    28, 56, 0x573,
    NULL, HbPopuplate_NotCombatArtBKSEL
};

static _DECL_INFO RText_E_055D = {
    &RText_E_0573, &RText_E_055F, NULL, NULL,
    28, 72, 0x55D,
    NULL, HbPopuplate_NotCombatArtBKSEL
};

static _DECL_INFO RText_E_055F = {
    &RText_E_055D, &RText_E_0572, NULL, NULL,
    28, 88, 0x55F,
    NULL, HbPopuplate_NotCombatArtBKSEL
};

static _DECL_INFO RText_E_0572 = {
    &RText_E_055F, &RText_E_CombatArt, NULL, NULL,
    28, 104, 0x572,
    NULL, HbPopuplate_NotCombatArtBKSEL
};

static _DECL_INFO RText_E_CombatArt = {
    &RText_E_0572, &RText_E_0544, NULL, NULL,
    28, 136, 0,
    HbRedirect_CombatArtBKSEL, HbPopuplate_CombatArtBKSEL
};
