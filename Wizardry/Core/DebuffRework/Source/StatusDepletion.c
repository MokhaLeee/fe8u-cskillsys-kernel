#include "global.h"
#include "bmusailment.h"
#include "bmunit.h"
#include "bmudisp.h"
#include "uiselecttarget.h"
#include "bmmind.h"

#include "common-chax.h"
#include "debuff.h"

struct UnknownBMUSAilmentProc {
    PROC_HEADER;

    /* 29 */ u8 _pad1[0x2C-0x29];
    /* 2C */ int unk_2C;
    /* 30 */ int _pad2;
    /* 34 */ int unk_34;
    /* 38 */ u8 _pad3[0x4C-0x38];

    /* 4C */ s16 unk_4C;
    /* 4E */ u8 _pad4[0x58-0x4E];

    /* 58 */ int unk_58;
};

void StartStatusHealEffect(struct Unit * unit, ProcPtr proc);

/* LynJump */
void StatusDecayDisplay_Display(struct UnknownBMUSAilmentProc * proc)
{
    struct SelectTarget * target = GetTarget(proc->unk_4C);
    struct Unit * unit = GetUnit(gActionData.subjectIndex);
    int status = GetUnitStatusIndex(unit);
    proc->unk_58 = status;

    SetUnitStatus(unit, 0);

    if (gpDebuffInfos[status].type == STATUS_INFO_TYPE_DEBUFF)
        StartStatusHealEffect(GetUnit(target->uid), proc);
}

/* LynJump */
void TickActiveFactionTurn(void)
{
    int i, displayMapChange = FALSE;

    InitTargets(0, 0);

    for (i = gPlaySt.faction + 1; i < gPlaySt.faction + 0x40; ++i)
    {
        struct Unit * unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->state & (US_UNAVAILABLE | US_RESCUED))
            continue;

        if (unit->barrierDuration != 0)
            unit->barrierDuration--;

        if (unit->torchDuration != 0)
        {
            unit->torchDuration--;
            displayMapChange = TRUE;
        }
    }

    if (displayMapChange)
    {
        RenderBmMapOnBg2();
        RefreshEntityBmMaps();
        RenderBmMap();
        NewBMXFADE(TRUE);
        RefreshUnitSprites();
    }

    #define DEC_STATUS(unit)                                        \
        int _status = GetUnitStatusIndex(unit);                     \
        int _duration = GetUnitStatusDuration(unit);                \
        if (0 != _status && _duration != 0)                         \
        {                                                           \
            if (_status != UNIT_STATUS_RECOVER)                     \
                TryTickUnitStatusDuration(unit);                    \
            if (GetUnitStatusDuration(unit) == 0)                   \
                AddTarget(unit->xPos, unit->yPos, unit->index, 0);  \
        }

    if (FACTION_BLUE == gPlaySt.faction)
    {
        /* Blue buff */
        for (i = FACTION_BLUE + 1; i < (FACTION_BLUE + 0x40); i++)
        {
            struct Unit * unit = GetUnit(i);

            if (gpDebuffInfos[GetUnitStatusIndex(unit)].type != STATUS_INFO_TYPE_BUFF)
                continue;

            DEC_STATUS(unit);
        }

        /* Red debuff */
        for (i = FACTION_RED + 1; i < (FACTION_RED + 0x40); i++)
        {
            struct Unit * unit = GetUnit(i);

            if (gpDebuffInfos[GetUnitStatusIndex(unit)].type != STATUS_INFO_TYPE_DEBUFF)
                continue;

            DEC_STATUS(unit);
        }
    }
    else if (FACTION_RED == gPlaySt.faction)
    {
        /* Red buff */
        for (i = FACTION_RED + 1; i < (FACTION_RED + 0x40); i++)
        {
            struct Unit * unit = GetUnit(i);

            if (gpDebuffInfos[GetUnitStatusIndex(unit)].type != STATUS_INFO_TYPE_BUFF)
                continue;

            DEC_STATUS(unit);
        }

        /* Blue debuff */
        for (i = FACTION_BLUE + 1; i < (FACTION_BLUE + 0x40); i++)
        {
            struct Unit * unit = GetUnit(i);

            if (gpDebuffInfos[GetUnitStatusIndex(unit)].type != STATUS_INFO_TYPE_DEBUFF)
                continue;

            DEC_STATUS(unit);
        }

        /* Green debuff */
        for (i = FACTION_GREEN + 1; i < (FACTION_GREEN + 0x40); i++)
        {
            struct Unit * unit = GetUnit(i);

            if (gpDebuffInfos[GetUnitStatusIndex(unit)].type != STATUS_INFO_TYPE_DEBUFF)
                continue;

            DEC_STATUS(unit);
        }
    }
    else if (FACTION_GREEN == gPlaySt.faction)
    {
        /* Green buff */
        for (i = FACTION_GREEN + 1; i < (FACTION_GREEN + 0x40); i++)
        {
            struct Unit * unit = GetUnit(i);

            if (gpDebuffInfos[GetUnitStatusIndex(unit)].type != STATUS_INFO_TYPE_BUFF)
                continue;

            DEC_STATUS(unit);
        }
    }
}
