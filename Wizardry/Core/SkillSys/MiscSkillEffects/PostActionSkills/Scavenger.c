#include "common-chax.h"
#include "debuff.h"
#include "skill-system.h"
#include "map-anims.h"
#include "constants/skills.h"
#include "mu.h"

#if defined(SID_Scavenger) && (COMMON_SKILL_VALID(SID_Scavenger))

void percentageCheck() {
    int percentage = NextRN_100();

    gEventSlots[EVT_SLOT_7] = percentage;
    gEventSlots[EVT_SLOT_8] = gActiveUnit->skl;

    return;
}

void searchItem() {
    int scavengerID = NextRN_N(10);

    NoCashGBAPrintf("scavengerID is: %d", scavengerID);

    switch (scavengerID) 
    {
        case 0:
            gEventSlots[EVT_SLOT_3] = ITEM_VULNERARY;
            break;
        case 1:
            gEventSlots[EVT_SLOT_3] = ITEM_ANTITOXIN;
            break;
        case 2:
            gEventSlots[EVT_SLOT_3] = ITEM_DOORKEY;
            break;
        case 3:
            gEventSlots[EVT_SLOT_3] = ITEM_CHESTKEY;
            break;
        case 4:
            gEventSlots[EVT_SLOT_3] = ITEM_ELIXIR;
            break;
        case 5:
            gEventSlots[EVT_SLOT_3] = ITEM_MONSTER_STONE;
            break;
        case 6:
            gEventSlots[EVT_SLOT_3] = ITEM_200G;
            break;
        case 7:
            gEventSlots[EVT_SLOT_3] = ITEM_REDGEM;
            break;
        case 8:
            gEventSlots[EVT_SLOT_3] = ITEM_BOOSTER_LCK;
            break;
        case 9:
            gEventSlots[EVT_SLOT_3] = ITEM_METISSTOME;
            break;
        default:
            break;
    }

    return;
}

STATIC_DECLAR const EventScr EventScr_Scavenger[] = {
    ASMC(percentageCheck)
    BLE(0x1, EVT_SLOT_7, EVT_SLOT_8)
    NOFADE
    ENDA

LABEL(0x1)
    ASMC(searchItem)
    GIVEITEMTO(CHAR_EVT_ACTIVE_UNIT)
    NOFADE
    ENDA
};

bool PostActionScavenger(ProcPtr parent)
{

    if (!UNIT_ALIVE(gActiveUnit) || UNIT_STONED(gActiveUnit))
        return false;

    if (gPlaySt.chapterTurnNumber > 10)
        return false;

    if (!SkillTester(gActiveUnit, SID_Scavenger))
        return false;

    KernelCallEvent(EventScr_Scavenger, EV_EXEC_CUTSCENE, parent);
    return true;
}

#endif