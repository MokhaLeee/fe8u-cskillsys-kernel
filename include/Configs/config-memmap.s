.macro SET_DATA name, value
    .global \name
    .type \name, object
    .set \name, \value
.endm

.macro dat value, name
    .global \name
    .type \name, object
    .set \name, \value
.endm


SET_DATA FreeRamSpaceTop,    0x02026E30
SET_DATA FreeRamSpaceBottom, 0x02028E58
SET_DATA UsedFreeRamSpaceTop, FreeRamSpaceBottom

.macro _kernel_malloc name, size
    .set UsedFreeRamSpaceTop, UsedFreeRamSpaceTop - \size
    SET_DATA \name, UsedFreeRamSpaceTop
.endm

SET_DATA FreeDemoRamSpaceTop,    0x0203F000
SET_DATA FreeDemoRamSpaceBottom, 0x02040000
SET_DATA UsedFreeDemoRamSpaceTop, FreeDemoRamSpaceBottom

.macro _kernel_malloc_demo name, size
    .set UsedFreeDemoRamSpaceTop, UsedFreeDemoRamSpaceTop - \size
    SET_DATA \name, UsedFreeDemoRamSpaceTop
.endm

SET_DATA FreeRamSpace2Top,    0x0203AAA4
SET_DATA FreeRamSpace2Bottom, 0x0203DDE0
SET_DATA UsedFreeRamSpace2Top, FreeRamSpace2Bottom

.macro _kernel_malloc2 name, size
    .set UsedFreeRamSpace2Top, UsedFreeRamSpace2Top - \size
    SET_DATA \name, UsedFreeRamSpace2Top
.endm

SET_DATA EwramOverlay0_FreeRamSpaceTop,    0x201F200
SET_DATA EwramOverlay0_FreeRamSpaceBottom, 0x2020188
SET_DATA EwramOverlay0_UsedFreeRamSpaceTop, EwramOverlay0_FreeRamSpaceBottom

.macro _kernel_malloc_overlay0 name, size
    .set EwramOverlay0_UsedFreeRamSpaceTop, EwramOverlay0_UsedFreeRamSpaceTop - \size
    SET_DATA \name, EwramOverlay0_UsedFreeRamSpaceTop
.endm

/* From the bottom to the top */
_kernel_malloc sSkillList, 0x50 * 3
_kernel_malloc sSkillFastList, 0x100
_kernel_malloc sLearnedSkillPLists, 51 * 0x20
_kernel_malloc sEfxSkillRoundData, 8 * 0x21
_kernel_malloc sEfxCombatArtRoundData,  0x30
_kernel_malloc gBattleActorGlobalFlag, 0x10
_kernel_malloc gBattleTargetGlobalFlag, 0x10
_kernel_malloc gCombatArtStatus, 0x10
_kernel_malloc sRandSeedsC, 8
_kernel_malloc gBanimSyncHandler, 8
_kernel_malloc gComboAtkList, 0x14
_kernel_malloc sKTutorialBits, 0xC
_kernel_malloc sKTutorialBitsHistory, 0xC
_kernel_malloc gBattleTargetPositionBackup, 0x4
_kernel_malloc gActionDataExpa, 0x10
_kernel_malloc sStatDebuffStatusAlly, 51 * 16
_kernel_malloc sStatDebuffStatusEnemy, 51 * 16
_kernel_malloc sStatDebuffStatusNpc, 8 * 16
_kernel_malloc sStatDebuffStatusBattleUnit, 2 * 16
_kernel_malloc sStatDebuffMsgBuf, 0x2C * 7
_kernel_malloc sExpaConvoyItemCount, 4
_kernel_malloc sExpaConvoyItemArray, 2 * 300
_kernel_malloc sGaidenMagicListObj, 0x24
_kernel_malloc gpActorShileInfo, 4
_kernel_malloc gpTargetShileInfo, 4
_kernel_malloc sShileldInfoCache, 0x14 * 4
_kernel_malloc sPopupSkillStack, 0x10
_kernel_malloc gPlayStExpa, 0x10
_kernel_malloc sShileldInfoNext, 1
_kernel_malloc GenericBufferUsedFlag, 1
_kernel_malloc sKernelHookSkippingFlag, 1
_kernel_malloc sAnimNumberSlot, 1
_kernel_malloc sStatDebuffMsgBufNext, 1
_kernel_malloc gKonamiComboStep, 1

_kernel_malloc _kernel_malloc_align4_pad, 2

/**
 * These part of space is allocated from `ewram_overlay_0`
 * For more space, place refer to decomp ldscript.
 *
 * Note that since this space is in a time-sharing
 * relationship with banim, it is risky to use this area rashly.
 */
_kernel_malloc_overlay0 sPrepEquipSkillList, 0x120
_kernel_malloc_overlay0 UnitMenuSkills, 16
_kernel_malloc_overlay0 sEfxSkillQueue, 32
_kernel_malloc_overlay0 gBattleTemporaryFlag, 0x10
_kernel_malloc_overlay0 sCombatArtList, 0x20
_kernel_malloc_overlay0 sSelectedComatArtIndex, 4
_kernel_malloc_overlay0 sCombatArtBKSELfxTimer, 4
_kernel_malloc_overlay0 sHelpBoxType, 4
_kernel_malloc_overlay0 BattleOrderSkills, 8
_kernel_malloc_overlay0 gComboMapAnimBattleUnit, 0x80
_kernel_malloc_overlay0 KernelMoveMapFlags, 4
_kernel_malloc_overlay0 KernelExtMoveBarrierMap, 4
_kernel_malloc_overlay0 KernelExtMovePioneerMap, 4
_kernel_malloc_overlay0 MapTaskVec, 4
_kernel_malloc_overlay0 gStatScreenStExpa, 4
_kernel_malloc_overlay0 BattleRoundInfoBak, 0x100
_kernel_malloc_overlay0 gDmg, 40
_kernel_malloc_overlay0 BattleSysBattleStatusBackup, 32
_kernel_malloc_overlay0 sAiSimuSlotBuf, 0x100
_kernel_malloc_overlay0 gItemPageList, 0x28
_kernel_malloc_overlay0 gPostActionCommonBuffer, 0x20
_kernel_malloc_overlay0 sTmpMovCostTable, 0x44

/**
 * Usage of memory on IWRAM for arm-functions
 *
 * part     function name       start           end             max size    real size
 * [a]      ARM_MapFloodCoreRe  0x03003CAC      0x03003F94      0x2E8       0x2E8
 * [a]      ARM_MapTask         0x03003F94      0x03003FF0      0x05C       0x05C
 * [a]      ARM_SkillTester     0x03003FF0      0x03004150      0x138       0x160
 *
 * [b]      ARM_UnitList        0x0300428C      0x03004378      0x0EC       0x0EC
 * [b]      ARM_SkillList       0x03004378      0x030043B4      0x03C       0x03C
 * [b]      __free__            0x030043B4      0x03004960      0x5A0       ---
 *
 * Note on part[a]:
 * In vanilla, RAM func left a ram space at: 0x03003F48 - 0x03004150
 * But since MovementSkill has rewriten function: MapFloodCore/MapFloodCoreStep
 * So we get antother space as 0x03003CAC - 0x03003F48
 * Now this part of free IWRAM space is: [0x03003CAC - 0x03004150]
 */
dat 0x03003CAC, ARM_MapFloodCoreRe
dat 0x03003F94, ARM_MapFloodCoreReEnd
dat 0x03003F94, ARM_MapTask
dat 0x03003FF0, ARM_MapTaskEnd
dat 0x03003FF0, ARM_SkillTester
dat 0x03004150, ARM_SkillTesterEnd

dat 0x0300428C, ARM_UnitList
dat 0x03004378, ARM_UnitListEnd
dat 0x03004378, ARM_SkillList
dat 0x030043B4, ARM_SkillListEnd
