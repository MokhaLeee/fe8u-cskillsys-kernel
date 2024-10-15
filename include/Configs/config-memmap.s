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
_kernel_malloc sSkillList, 0x40 * 3
_kernel_malloc sSkillFastList, 0x100
_kernel_malloc sLearnedSkillPLists, 0x46 * 0x20
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
_kernel_malloc sStatDebuffStatusEnemy, 50 * 16
_kernel_malloc sStatDebuffStatusNpc, 8 * 16
_kernel_malloc sStatDebuffStatusBattleUnit, 2 * 16
_kernel_malloc sStatDebuffMsgBuf, 0x2C * 7
_kernel_malloc sStatDebuffMsgBufNext, 4
_kernel_malloc sExpaConvoyItemCount, 4
_kernel_malloc sExpaConvoyItemArray, 2 * 300

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
_kernel_malloc_overlay0 sBattleSkillActivateBuf, 4

/**
 * Usage of memory on IWRAM for arm-functions
 *
 * part     function name       start           end             max size    real size
 * [a]      ARM_SkillTester     0x03003CAC      0x03003DE4      0x138       0x138
 * [a]      ARM_MapFloodCoreRe  0x03003DE4      0x030040CC      0x2E8       0x2E8
 * [a]      ARM_MapTask         0x030040CC      0x03004128      0x05C       0x05C
 * [a]      __free__            0x03004128      0x03004150      0x028       ---
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
dat 0x03003CAC, ARM_SkillTester
dat 0x03003DE4, ARM_SkillTesterEnd
dat 0x03003DE4, ARM_MapFloodCoreRe
dat 0x030040CC, ARM_MapFloodCoreReEnd
dat 0x030040CC, ARM_MapTask
dat 0x03004128, ARM_MapTaskEnd

dat 0x0300428C, ARM_UnitList
dat 0x03004378, ARM_UnitListEnd
dat 0x03004378, ARM_SkillList
dat 0x030043B4, ARM_SkillListEnd
