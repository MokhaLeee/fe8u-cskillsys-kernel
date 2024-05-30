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

/* Free RAM space allocation */
dat 0x02026E30, FreeRamSpaceTop
dat 0x02028E58, FreeRamSpaceBottom

/* From the bottom to the top */
SET_DATA sSkillListNext, FreeRamSpaceBottom - 4
SET_DATA sSkillLists, sSkillListNext - 0x100
SET_DATA sPrepEquipSkillList, sSkillLists - 0x120
SET_DATA sStatScreenPage1BarMax, sPrepEquipSkillList - 4
SET_DATA sLearnedSkillPLists, sStatScreenPage1BarMax - 0x46 * 0x20
SET_DATA sEfxSkillRoundData, sLearnedSkillPLists - 4 * 0x21
SET_DATA sEfxCombatArtRoundData, sEfxSkillRoundData - 0x30
SET_DATA gBattleTemporaryFlag, sEfxCombatArtRoundData - 8
SET_DATA gBattleActorGlobalFlag, gBattleTemporaryFlag - 8
SET_DATA gBattleTargetGlobalFlag, gBattleActorGlobalFlag - 8
SET_DATA gCombatArtStatus, gBattleTargetGlobalFlag - 0x10
SET_DATA sCombatArtList, gCombatArtStatus - 0x20
SET_DATA sSelectedComatArtIndex, sCombatArtList - 1
SET_DATA sCombatArtBKSELfxTimer, sSelectedComatArtIndex - 3
SET_DATA sHelpBoxType, sCombatArtBKSELfxTimer - 4
SET_DATA sRandSeedsC, sHelpBoxType - 8
SET_DATA sRandBackup, sRandSeedsC - 8
SET_DATA gPostActionGaleforceFlag, sRandBackup - 4
SET_DATA gBanimSyncHandler, gPostActionGaleforceFlag - 8
SET_DATA gComboAtkList, gBanimSyncHandler - 0x14
SET_DATA sKTutorialBits, gComboAtkList - 0xC
SET_DATA sKTutorialBitsHistory, sKTutorialBits - 0xC
SET_DATA gBattleTargetPositionBackup, sKTutorialBitsHistory - 0x4
SET_DATA gActionDataExpa, gBattleTargetPositionBackup - 0x10
SET_DATA gComboMapAnimBattleUnit, gActionDataExpa - 0x80
SET_DATA sStatDebuffStatusAlly, gComboMapAnimBattleUnit - 51 * 4
SET_DATA sStatDebuffStatusEnemy, sStatDebuffStatusAlly - 50 * 4
SET_DATA sStatDebuffStatusNpc, sStatDebuffStatusEnemy - 8 * 4
SET_DATA sStatDebuffMsgBuf, sStatDebuffStatusNpc - 0x1C * 6
SET_DATA sStatDebuffMsgBufNext, sStatDebuffMsgBuf - 4
SET_DATA sPrepareNextBattleRoundTimer, sStatDebuffMsgBufNext - 4

SET_DATA gKernelUsedFreeRamSpaceTop, sPrepareNextBattleRoundTimer
/* Assert(gKernelUsedFreeRamSpaceTop > FreeRamSpaceTop) */
