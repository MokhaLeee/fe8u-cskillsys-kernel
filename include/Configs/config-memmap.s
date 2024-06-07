.macro SET_DATA name, value
    .global \name
    .type \name, object
    .set \name, \value
.endm

SET_DATA FreeRamSpaceTop, 0x02026E30
SET_DATA FreeRamSpaceBottom, 0x02028E58
SET_DATA gKernelUsedFreeRamSpaceTop, FreeRamSpaceBottom

.macro _kernel_malloc name, size
    /* Assert(gKernelUsedFreeRamSpaceTop > FreeRamSpaceTop) */
    .set gKernelUsedFreeRamSpaceTop, gKernelUsedFreeRamSpaceTop - \size
    SET_DATA \name, gKernelUsedFreeRamSpaceTop
.endm

/* From the bottom to the top */
_kernel_malloc sSkillListNext, 4
_kernel_malloc sSkillLists, 0x100
_kernel_malloc sPrepEquipSkillList, 0x120
_kernel_malloc sStatScreenPage1BarMax, 4
_kernel_malloc sLearnedSkillPLists, 0x46 * 0x20
_kernel_malloc sEfxSkillRoundData, 4 * 0x21
_kernel_malloc sEfxCombatArtRoundData,  0x30
_kernel_malloc gBattleTemporaryFlag, 8
_kernel_malloc gBattleActorGlobalFlag, 8
_kernel_malloc gBattleTargetGlobalFlag, 8
_kernel_malloc gCombatArtStatus, 0x10
_kernel_malloc sCombatArtList, 0x20
_kernel_malloc sSelectedComatArtIndex, 4
_kernel_malloc sCombatArtBKSELfxTimer, 4
_kernel_malloc sHelpBoxType, 4
_kernel_malloc sRandSeedsC, 8
_kernel_malloc sRandBackup, 8
_kernel_malloc gPostActionGaleforceFlag, 4
_kernel_malloc gBanimSyncHandler, 8
_kernel_malloc gComboAtkList, 0x14
_kernel_malloc sKTutorialBits, 0xC
_kernel_malloc sKTutorialBitsHistory, 0xC
_kernel_malloc gBattleTargetPositionBackup, 0x4
_kernel_malloc gActionDataExpa, 0x10
_kernel_malloc gComboMapAnimBattleUnit, 0x80
_kernel_malloc sStatDebuffStatusAlly, 51 * 4
_kernel_malloc sStatDebuffStatusEnemy, 50 * 4
_kernel_malloc sStatDebuffStatusNpc, 8 * 4
_kernel_malloc sStatDebuffMsgBuf, 0x1C * 6
_kernel_malloc sStatDebuffMsgBufNext, 4
_kernel_malloc sPrepareNextBattleRoundTimer, 4
