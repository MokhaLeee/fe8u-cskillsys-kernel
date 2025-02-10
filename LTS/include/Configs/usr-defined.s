.include "macros.inc"

/* common */
dat 0x08017d64, gpCharacterData
dat 0x030005D0, gPopupInst
dat 0x030005F0, gpPopupUnit
dat 0x030005F4, gPopupItem
dat 0x030005F8, gPopupNumber

/* IconDisplay */
dat 0x02026A90, gIconReSts @ DrawnIconLookupTable
/**
 * Fall through.
 * Since there are enough space at here,
 * so we use such space for battle rather than FreeRAMSpaceEntry lol
 */
dat 0x02026AD0, gBattleHitArrayRe   /* 4 * 0x21 */
dat 0x02026B58, gExtBattleHitArray  /* 4 * 0x21 */
dat 0x02026BE0, gAnimRoundDataRe    /* 2 * 0x21 */
dat 0x02026C28, gEfxHpLutRe         /* 2 * 0x61 */

/* 0x02026CF0 - 0x02026E30 is free */
