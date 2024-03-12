.macro fun value, name
    .global \name
    .type \name, function
    .set \name, \value
.endm
.macro dat value, name
    .global \name
    .type \name, object
    .set \name, \value
.endm

.macro SET_FUNC name, value
    .global \name
    .type \name, function
    .set \name, \value
.endm

.macro SET_DATA name, value
    .global \name
    .type \name, object
    .set \name, \value
.endm

/* common */
dat 0x08017d64, gpCharacterData

/* Free RAM space allocation */
SET_DATA FreeRamSpaceEntry, 0x02026E30

SET_DATA sSkillListNext, FreeRamSpaceEntry + 0
SET_DATA sSkillLists, sSkillListNext + 4
SET_DATA sPrepEquipSkillList, sSkillLists + 0x100
SET_DATA sStatScreenPage1BarMax, sPrepEquipSkillList + 0x120
SET_DATA sLearnedSkillPLists, sStatScreenPage1BarMax + 0x4
SET_DATA sEfxSkillRoundData, sLearnedSkillPLists + 0x46 * 0x20
SET_DATA sEfxCombatArtRoundData, sEfxSkillRoundData + 4 * 0x21
SET_DATA gBattleTemporaryFlag, sEfxCombatArtRoundData + 0x40
SET_DATA gBattleActorGlobalFlag, gBattleTemporaryFlag + 8
SET_DATA gBattleTargetGlobalFlag, gBattleActorGlobalFlag + 8
SET_DATA gCombatArtStatus, gBattleTargetGlobalFlag + 8
SET_DATA sCombatArtList, gCombatArtStatus + 0x10
SET_DATA sSelectedComatArtIndex, sCombatArtList + 0x20
SET_DATA sCombatArtBKSELfxTimer, sSelectedComatArtIndex + 0x1
SET_DATA sHelpBoxType, sCombatArtBKSELfxTimer + 0x3
SET_DATA sRandSeedsC, sHelpBoxType + 0x4
SET_DATA sRandBackup, sRandSeedsC + 0x8
SET_DATA gPostActionGaleforceFlag, sRandBackup + 0x8
SET_DATA gBanimSyncHandler, gPostActionGaleforceFlag + 0x4
SET_DATA gComboAtkList, gBanimSyncHandler + 0x8

SET_DATA FreeRamSpaceTail, gComboAtkList + 0x14
SET_DATA FreeRamSpaceMax, FreeRamSpaceEntry + 0x2028

/* IconDisplay */
SET_DATA gIconReSts, 0x02026A90 @ DrawnIconLookupTable
/**
 * Fall through.
 * Since there are enough space at here,
 * so we use such space for battle rather than FreeRAMSpaceEntry lol
 */
SET_DATA gBattleHitArrayRe, 0x02026AD0   /* 4 * 0x21 */
SET_DATA gAnimRoundDataRe, 0x02026B54    /* 4 * 0x21 */
SET_DATA gEfxHpLutRe, 0x02026BD8         /* 4 * 0x21 */
