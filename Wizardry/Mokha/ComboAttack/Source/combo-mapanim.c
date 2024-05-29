#include "common-chax.h"
#include "battle-system.h"
#include "combo-attack.h"

#define COMBO_MAPA_ACTOR_IDX 2

extern struct BattleUnit gComboMapAnimBattleUnit;

STATIC_DECLAR void MapAnim_PrepareNextBattleRound_CleanPreRoundCombo(void)
{
    struct Unit * unit;
    int round = GetBattleHitRound(gManimSt.pCurrentRound);

    if (round != 0)
    {
        unit = GetMapAnimComboUnit(round - 1);
        if (!UNIT_IS_VALID(unit))
            return;

        Printf("actor %d, pid %#x at round %d",
                gManimSt.subjectActorId, UNIT_CHAR_ID(gManimSt.actor[COMBO_MAPA_ACTOR_IDX].unit), round);

        if (gManimSt.subjectActorId != COMBO_MAPA_ACTOR_IDX)
            return;

        if (gManimSt.actor[COMBO_MAPA_ACTOR_IDX].unit != unit)
            return;

        MU_End(gManimSt.actor[COMBO_MAPA_ACTOR_IDX].mu);

        gManimSt.actor[COMBO_MAPA_ACTOR_IDX].unit = NULL;
        gManimSt.actor[COMBO_MAPA_ACTOR_IDX].bu   = NULL;
        gManimSt.actor[COMBO_MAPA_ACTOR_IDX].mu = NULL;

        ShowUnitSprite(unit);
    }
}

STATIC_DECLAR void MapAnim_PrepareNextBattleRound_SetNewRoundCombo(void)
{
    struct Unit * unit;
    int round = GetBattleHitRound(gManimSt.pCurrentRound);

    unit = GetMapAnimComboUnit(round);
    if (!UNIT_IS_VALID(unit))
    {
        SetupBattleMOVEUNITs();
        return;
    }

    /* Now we get the combo unit */
    gComboMapAnimBattleUnit.unit = *unit;

    // SetupMapBattleAnim
    MakeBattleMOVEUNIT(COMBO_MAPA_ACTOR_IDX, &gComboMapAnimBattleUnit, unit);
    HideUnitSprite(unit);

    // SetupBattleMOVEUNITs
    gManimSt.actor[COMBO_MAPA_ACTOR_IDX].mu->pAPHandle->objLayer = gUnknown_08205714[COMBO_MAPA_ACTOR_IDX];

    // MapAnim_AdvanceBattleRound
    gManimSt.subjectActorId = COMBO_MAPA_ACTOR_IDX;

    SetBattleAnimFacing(COMBO_MAPA_ACTOR_IDX, 1, MA_FACING_OPPONENT);
    SetBattleAnimFacing(1, COMBO_MAPA_ACTOR_IDX, MA_FACING_OPPONENT);

    // MapAnim_PrepareNextBattleRound
    gManimSt.specialProcScr = GetSpellAssocAlt6CPointer(GetUnitEquippedWeapon(unit));
}

/**
 * Special combo map-anim effect
 */
extern const u8 Img_MapAnimCOMBO[];

STATIC_DECLAR void MapAnim_ShowComboSpecialEffect(ProcPtr proc)
{
    int round = GetBattleHitRound(gManimSt.pCurrentRound - 1);
    struct Unit * unit = GetMapAnimComboUnit(round);

    if (!UNIT_IS_VALID(unit) || unit != gManimSt.actor[COMBO_MAPA_ACTOR_IDX].unit)
    {
        Proc_Goto(proc, 99);
        return;
    }

    Decompress(
        Img_MapAnimCOMBO,
        OBJ_VRAM0 + 0x20 * BM_OBJCHR_BANIM_EFFECT);

    APProc_Create(
        Obj_MapAnimMISS,
        16 * (SCREEN_TILE_X(unit->xPos)) + 8,
        16 * (SCREEN_TILE_Y(unit->yPos)) + 16,
        TILEREF(BM_OBJCHR_BANIM_EFFECT, 0), 0, 2);

    PlaySeSpacial(0x2DA,
            gManimSt.actor[COMBO_MAPA_ACTOR_IDX].unit->xPos * 0x10 - gBmSt.camera.x);
}

STATIC_DECLAR void MapAnim_PlayComboSE(ProcPtr proc)
{
    PlaySeSpacial(0x2DA,
            gManimSt.actor[COMBO_MAPA_ACTOR_IDX].unit->xPos * 0x10 - gBmSt.camera.x);
}

STATIC_DECLAR const struct ProcCmd ProcScr_MapAnim_PrepareNextBattleHook[] = {
    PROC_YIELD,
    PROC_CALL(MapAnim_ShowComboSpecialEffect),
    PROC_SLEEP(5),
    PROC_CALL(MapAnim_PlayComboSE),
    PROC_SLEEP(20),

PROC_LABEL(99),
    PROC_END
};

static void _dummy_func(ProcPtr p)
{
    struct Proc * proc = p;

    proc->proc_idleCb = NULL;
    Proc_Break(proc);
}

/* LynJump */
void MapAnim_PrepareNextBattleRound(ProcPtr p)
{
    u16 weapon;
    struct BattleUnit * unit;
    struct Proc * proc = p;
    struct MapAnimState *state = &gManimSt;

#if CHAX
    MapAnim_PrepareNextBattleRound_CleanPreRoundCombo();
#endif

    if (state->pCurrentRound->info & 0x10) {
        Proc_Break(proc);
        Proc_GotoScript(proc, gProc_MapAnimEnd);
        return;
    }
    MapAnim_AdvanceBattleRound();
    unit = state->actor[state->subjectActorId].bu;
    weapon = unit->weaponBefore;
    state->specialProcScr = GetSpellAssocAlt6CPointer(weapon);

#if CHAX
    gManimSt.pCurrentRound--;
    MapAnim_PrepareNextBattleRound_SetNewRoundCombo();
    gManimSt.pCurrentRound++;
    Proc_StartBlocking(ProcScr_MapAnim_PrepareNextBattleHook, proc);

    /* Add a PROC_YIELD */
    proc->proc_idleCb = _dummy_func;
#else
    Proc_Break(proc);
#endif
}
