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
