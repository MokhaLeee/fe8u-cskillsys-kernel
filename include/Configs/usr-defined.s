.include "macros.inc"

/* common */
dat 0x08017d64, gpCharacterData

/* IconDisplay */
dat 0x02026A90, gIconReSts @ DrawnIconLookupTable
/**
 * Fall through.
 * Since there are enough space at here,
 * so we use such space for battle rather than FreeRAMSpaceEntry lol
 */
dat 0x02026AD0, gBattleHitArrayRe   /* 4 * 0x21 */
dat 0x02026B54, gAnimRoundDataRe    /* 4 * 0x21 */
dat 0x02026BD8, gEfxHpLutRe         /* 4 * 0x21 */

/**
 * In vanilla, RAM func left a ram space at: 0x03003F48 - 0x03004150
 * But since MovementSkill has rewriten function: MapFloodCore/MapFloodCoreStep
 * So we get auther space as 0x03003CAC - 0x03003F48
 * Now this part of free IWRAM space is: [0x03003CAC - 0x03004150] (size=)
 */
dat 0x03003CAC, ARM_SkillTester
dat 0x03003E4C, ARM_SkillTesterEnd      @ size = 0x1A0
dat 0x03003E4C, ARM_MapFloodCoreRe
dat 0x03004150, ARM_MapFloodCoreReEnd   @ size = 0x304

dat 0x0300428C, ARM_UnitList
dat 0x0300438C, ARM_UnitListEnd
dat 0x0300438C, ARM_SkillList
dat 0x0300450C, ARM_SkillListEnd
