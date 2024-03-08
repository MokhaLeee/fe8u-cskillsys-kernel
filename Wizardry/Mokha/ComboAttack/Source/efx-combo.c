#include "common-chax.h"
#include "chax-glb.h"
#include "banim-sync.h"
#include "efx-skill.h"
#include "combo-attack.h"

/**
 * SwitchAISFrameDataFromBARoundType:
 *  After caught hit, change it to battle anim
 * 
 * another side may block at C01
 */
struct BanimSyncHandler * GetBanimSyncHandler_ComboAtk(struct Anim * anim)
{
    struct Unit * unit;
    int round;

    if (!IsAttackerAnim(anim))
        return NULL;

    round = anim->nextRoundId - 1;
    if (!(round < COMBO_ATK_MAX))
        return NULL;

    if (gComboAtkList[round].uid == COMBO_ATK_UID_INVALID)
        return NULL;

    unit = GetUnit(gComboAtkList[round].uid);
    if (!UNIT_IS_VALID(unit))
        return NULL;

    gBanimSyncHandler.pid = UNIT_CHAR_ID(unit);
    gBanimSyncHandler.jid = UNIT_CLASS_ID(unit);
    gBanimSyncHandler.weapon = ITEM_INDEX(gComboAtkList[round].weapon);
    gBanimSyncHandler.animdef = unit->pClassData->pBattleAnimDef;
    return &gBanimSyncHandler;
}
