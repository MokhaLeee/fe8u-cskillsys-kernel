#include "common-chax.h"
#include "kernel-lib.h"
#include "feb-compat.h"
#include "banim-sync.h"
#include "efx-skill.h"
#include "combo-attack.h"

/**
 * SwitchAISFrameDataFromBARoundType:
 *  After caught hit, change it to battle anim
 * 
 * another side may block at C01
 */
struct BanimSyncHandler * GetBanimSyncHandler_ComboAtk(struct Anim *anim)
{
	struct Unit *unit;
	int round;

	/* User defined switch */
	if (!gpKernelDesigerConfig->combo_attack_en)
		return NULL;

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

	gBanimSyncHandler.animdef = NULL;

#ifdef CONFIG_USE_CHAR_CUSTOM_ANIM
	/* Port for Individual_animation_ea_2 */
	int i;
	const struct IndividualAnimConf *aconf = GetPrIndividualAnimConf();

	for (i = 0; aconf[i].pid != 0 || aconf[i].jid != 0; i++) {
		u8 pid = aconf[i].pid;
		u8 jid = aconf[i].jid;

		if (pid == UNIT_CHAR_ID(unit) && jid == UNIT_CLASS_ID(unit)) {
			gBanimSyncHandler.animdef = aconf[i].anim_conf;
			break;
		}
	}
#endif

	if (gBanimSyncHandler.animdef == NULL)
		gBanimSyncHandler.animdef = unit->pClassData->pBattleAnimDef;

	return &gBanimSyncHandler;
}
