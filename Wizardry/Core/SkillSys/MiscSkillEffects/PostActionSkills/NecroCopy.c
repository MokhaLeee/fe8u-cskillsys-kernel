#include "common-chax.h"
#include "debuff.h"
#include "kernel-lib.h"
#include "skill-system.h"
#include "battle-system.h"
#include "event-rework.h"
#include "constants/skills.h"
#include "constants/texts.h"
#include "strmag.h"

STATIC_DECLAR void PostActionNecroCopySkipMenuIfNotAlly(struct EventEngineProc * proc)
{
    ShowUnitSprite(gActiveUnit);

    gEventSlots[EVT_SLOT_C] = false;
    if (UNIT_FACTION(gActiveUnit) == FACTION_BLUE)
        gEventSlots[EVT_SLOT_C] = true;
}

STATIC_DECLAR void PrepareNecroCopy(void)
{
    struct Unit * targetUnit = GetUnit(gBattleTarget.unit.index);

    gActiveUnit->pClassData = targetUnit->pClassData;
    gActiveUnit->maxHP = targetUnit->maxHP;
    gActiveUnit->curHP = targetUnit->maxHP;
    gActiveUnit->pow = targetUnit->pow;
    UNIT_MAG(gActiveUnit) = UNIT_MAG(targetUnit);
    gActiveUnit->skl = targetUnit->skl;
    gActiveUnit->spd = targetUnit->spd;
    gActiveUnit->lck = targetUnit->lck;
    gActiveUnit->def = targetUnit->def;
    gActiveUnit->res = targetUnit->res;

    gActiveUnit->ranks[ITYPE_ANIMA] = targetUnit->ranks[ITYPE_ANIMA];
    gActiveUnit->ranks[ITYPE_LIGHT] = targetUnit->ranks[ITYPE_LIGHT];
    gActiveUnit->ranks[ITYPE_DARK] = targetUnit->ranks[ITYPE_DARK];
    gActiveUnit->ranks[ITYPE_STAFF] = targetUnit->ranks[ITYPE_STAFF];
    gActiveUnit->ranks[ITYPE_SWORD] = targetUnit->ranks[ITYPE_SWORD];
    gActiveUnit->ranks[ITYPE_LANCE] = targetUnit->ranks[ITYPE_LANCE];
    gActiveUnit->ranks[ITYPE_AXE] = targetUnit->ranks[ITYPE_AXE];
    gActiveUnit->ranks[ITYPE_BOW] = targetUnit->ranks[ITYPE_BOW];

    gActiveUnit->items[0] = targetUnit->items[0];
    gActiveUnit->items[1] = targetUnit->items[1];
    gActiveUnit->items[2] = targetUnit->items[2];
    gActiveUnit->items[3] = targetUnit->items[3];
    gActiveUnit->items[4] = targetUnit->items[4];
}

STATIC_DECLAR const EventScr EventScr_PostActionNecroCopy[] = {
    EVBIT_MODIFY(0x4)
    ASMC(PostActionNecroCopySkipMenuIfNotAlly)
    BEQ(0, EVT_SLOT_C, EVT_SLOT_0)

    TUTORIALTEXTBOXSTART
    SVAL(EVT_SLOT_B, 0xffffffff)
    TEXTSHOW(MSG_SkillEffect_NecroCopy_Confirm)
    TEXTEND
    REMA
    SVAL(EVT_SLOT_7, 0x1)
    BNE(99, EVT_SLOT_C, EVT_SLOT_7)

LABEL(0)
#if defined(SID_NecroCopy) && (COMMON_SKILL_VALID(SID_NecroCopy))
    SVAL(EVT_SLOT_B, SID_NecroCopy)
    CALL(EventScr_MuSkillAnim)
#endif
    ASMC(PrepareNecroCopy)

LABEL(99)
    ASMC(MapAnim_CommonEnd)
    NOFADE
    ENDA
};

bool PostActionNecroCopy(ProcPtr proc)
{
    if (gActionData.unitActionType != UNIT_ACTION_COMBAT)
        return false;

    if (!UNIT_ALIVE(gActiveUnit) || UNIT_STONED(gActiveUnit))
        return false;

#if defined(SID_NecroCopy) && (COMMON_SKILL_VALID(SID_NecroCopy))
    if (!SkillTester(gActiveUnit, SID_NecroCopy))
#else
    if (1)
#endif
        return false;

    if (gBattleActorGlobalFlag.enimy_defeated == false)
        return false;

    KernelCallEvent(EventScr_PostActionNecroCopy, EV_EXEC_CUTSCENE, proc);

    return true;
}
