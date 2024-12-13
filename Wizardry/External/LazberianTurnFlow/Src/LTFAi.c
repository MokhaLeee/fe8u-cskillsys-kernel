#include "LTF.h"

void LTFAi_CpOrderInit(struct Proc*);
static void LTFAi_BskOrderInit(struct Proc*);
static void LTFAi_CpOrderTryDoAi1(struct Proc*);
static void LTFAi_CpOrderTryDoAi2(struct Proc*);
static void LTFAi_CpOrderTryDoHealEscape(struct Proc*);
static void LTFAi_CpOrderTryDoSpecialItems(struct Proc*);
static void LTFAi_CpOrderEndTurns(struct Proc*);

typedef void(*CpOrderFunc)(struct Proc*);
typedef void(*AiDecitionMakerFunc)(void);

static void DecideHealOrEscape(void);
static void DecideScriptA(void);
static void DecideScriptB(void);
static void DecideSpecialItems(void);

static void LTFAi_EndTurnDicider(void);

static void LTFAi_CallDecide(struct Proc*, AiDecitionMakerFunc decisionMaker);

static int GetUnitAiPriority(struct Unit* unit);
static int BuildAiUnitList(void);

// static int GetUnitBattleAiPriority(struct Unit* unit);

static
u32* const sUnitPriorityArray = (void*) gGenericBuffer;

void DecideHealOrEscape(void)
{
    if (gAiState.flags & AI_FLAG_BERSERKED)
        return;

    if (AiUpdateGetUnitIsHealing(gActiveUnit) == TRUE)
    {
        struct Vec2 vec2;

        if (AiTryHealSelf() == TRUE)
            return;

        if ((gActiveUnit->aiFlags & AI_UNIT_FLAG_3) && (AiTryMoveTowardsEscape() == TRUE))
        {
            AiTryDanceOrStealAfterMove();
            return;
        }

        if (AiTryGetNearestHealPoint(&vec2) != TRUE)
            return;

        AiTryMoveTowards(vec2.x, vec2.y, 0, 0, 1);

        if (gAiDecision.actionPerformed == TRUE)
            AiTryActionAfterMove();
    }
    else
    {
        if ((gActiveUnit->aiFlags & AI_UNIT_FLAG_3) && (AiTryMoveTowardsEscape() == TRUE))
            AiTryDanceOrStealAfterMove();
    }
}

void DecideSpecialItems(void)
{
    if (gAiState.flags & AI_FLAG_BERSERKED)
        return;

    AiTryDoSpecialItems();
}

void DecideScriptA(void)
{
    int i = 0;

    if (UNIT_IS_GORGON_EGG(gActiveUnit))
        return;

    if (gAiState.flags & AI_FLAG_BERSERKED)
    {
        AiDoBerserkAction();
        return;
    }

    for (i = 0; i < 0x100; ++i)
    {
        if (AiTryExecScriptA() == TRUE)
            return;
    }

    AiExecFallbackScriptA();
}

void DecideScriptB(void)
{
    int i = 0;

    if ((gActiveUnit->state & US_IN_BALLISTA) && (GetRiddenBallistaAt(gActiveUnit->xPos, gActiveUnit->yPos) != NULL))
        return;

    if (gAiState.flags & AI_FLAG_BERSERKED)
    {
        AiDoBerserkMove();
        return;
    }

    for (i = 0; i < 0x100; ++i)
    {
        if (AiTryExecScriptB() == TRUE)
            return;
    }

    AiExecFallbackScriptB();
}

// LYN_REPLACE_CHECK(GetUnitBattleAiPriority);
int GetUnitBattleAiPriority(struct Unit* unit)
{
    int i, item;

    u8 rangedAmt = 0;
    u8 meleeAmt = 0;

    for (i = 0; (i < UNIT_ITEM_COUNT) && !!(item = unit->items[i]); ++i)
    {
        if (!CanUnitUseWeapon(unit, item) && !CanUnitUseStaff(unit, item))
            continue;

        if (GetItemAttributes(item) & IA_STAFF)
        {
            return 72;
        }

        if (GetItemAttributes(item) & IA_WEAPON)
        {
            int range = GetItemMaxRange(item);

            if (range > 1)
                rangedAmt++;
            else
                meleeAmt++;
        }
    }

    if (rangedAmt != 0)
        return 40;

    if (meleeAmt != 0)
        return 20;

    return 87;
}

int GetUnitAiPriority(struct Unit* unit)
{
    int priority = UNIT_MOV(unit);

    u16 lead = GetUnitLeaderCharId(unit);

    if (UNIT_CATTRIBUTES(unit) & (CA_DANCE | CA_PLAY))
        return priority - 149;

    if (!(unit->aiFlags & AI_UNIT_FLAG_0))
    {
        priority += lead << 8;

        if (UNIT_CATTRIBUTES(unit) & CA_STEAL)
            return priority + 60;

        if ((unit->pCharacterData->number == lead) || (UNIT_CATTRIBUTES(unit) & CA_LORD))
            return priority + 87;

        priority = priority + GetUnitBattleAiPriority(unit);
    }

    return priority;
}


int BuildAiUnitList(void)
{
    int i, aiNum = 0;

    u32 faction = gPlaySt.faction;
    u32* prioIt = sUnitPriorityArray;

    int factionUnitCountLut[3] = { 62, 20, 50 }; // TODO: named constant for those

    for (i = 0; i < factionUnitCountLut[faction >> 6]; ++i)
    {
        struct Unit* unit = GetUnit(faction + i + 1);

        if (!unit->pCharacterData)
            continue;

        if (unit->statusIndex == UNIT_STATUS_SLEEP)
            continue;

        if (unit->statusIndex == UNIT_STATUS_BERSERK)
            continue;

        if (unit->state & (US_HIDDEN | US_UNSELECTABLE | US_DEAD | US_RESCUED | US_HAS_MOVED_AI))
            continue;

        gAiState.units[aiNum] = faction + i + 1;
        *prioIt++ = GetUnitAiPriority(unit);

        aiNum++;
    }

    return aiNum;
}


const CpOrderFunc LTFAi_NewCpOrderFuncList[] =
{
	LTFAi_CpOrderInit,
	LTFAi_CpOrderTryDoAi1,
	LTFAi_CpOrderTryDoHealEscape,
	LTFAi_CpOrderTryDoAi2,
	LTFAi_CpOrderTryDoSpecialItems,
	LTFAi_CpOrderEndTurns,
	(CpOrderFunc)Proc_Break,
};

const struct ProcCmd gProc_LTFAiNewBskOrder[] =
{
	PROC_NAME("Stan:LTF:NewBskOrder"),

	PROC_CALL(LTFAi_BskOrderInit),

	PROC_CALL(LTFAi_CpOrderTryDoAi1),
	PROC_YIELD,

	PROC_CALL(LTFAi_CpOrderTryDoAi2),
	PROC_YIELD,

	PROC_CALL(LTFAi_CpOrderEndTurns),
	PROC_YIELD,

	PROC_END
};

void LTFAi_CpOrderInit(struct Proc* cpOrderProc)
{
	int count = BuildAiUnitList();

	if (!count)
	{
		// This shouldn't happen tbbqh
		Proc_Break(cpOrderProc);
		return;
	}

//	SortAiUnitQueue(count);
	gAiState.units[count] = 0; // terminator

	AiClearDecision();
}

static void LTFAi_BskOrderInit(struct Proc* bskOrderProc)
{
	int count = 0;

	for (unsigned index = 1; index < 0x100; ++index)
	{
		struct Unit* unit = GetUnit(index);

		if (!unit || !unit->pCharacterData)
			continue;
	
		if (unit->state & (US_DEAD | US_NOT_DEPLOYED | US_RESCUED | US_UNDER_A_ROOF | 0x00010000 | US_UNSELECTABLE))
			continue;
	
		if (unit->statusIndex != UNIT_STATUS_BERSERK)
			continue;

		gAiState.units[count++] = unit->index;
	}

	if (!count)
	{
		// This shouldn't happen tbbqh
		Proc_Break(bskOrderProc);
		return;
	}

//	SortAiUnitQueue(count);
	gAiState.units[count] = 0; // terminator

	AiClearDecision();
}

static void LTFAi_CpOrderTryDoAi1(struct Proc* cpOrderProc)
{
	gAiState.decideState = 1;
	LTFAi_CallDecide(cpOrderProc, DecideScriptA);
}

static void LTFAi_CpOrderTryDoAi2(struct Proc* cpOrderProc)
{
	gAiState.decideState = 2;
	LTFAi_CallDecide(cpOrderProc, DecideScriptB);
}

static void LTFAi_CpOrderTryDoHealEscape(struct Proc* cpOrderProc)
{
	gAiState.decideState = 0;
	LTFAi_CallDecide(cpOrderProc, DecideHealOrEscape);
}

static void LTFAi_CpOrderTryDoSpecialItems(struct Proc* cpOrderProc)
{
	gAiState.decideState = 3;
	LTFAi_CallDecide(cpOrderProc, DecideSpecialItems);
}

static void LTFAi_CpOrderEndTurns(struct Proc* cpOrderProc)
{
	LTFAi_CallDecide(cpOrderProc, LTFAi_EndTurnDicider);
}

static void LTFAi_EndTurnDicider(void)
{
	AiSetDecision(
		gActiveUnit->xPos, gActiveUnit->yPos,
		AI_ACTION_NONE,
		gActiveUnitId, 0, 0, 0
	);

}

static void LTFAi_CallDecide(struct Proc* cpOrderProc, AiDecitionMakerFunc decisionMaker)
{
	if (gAiDecision.actionPerformed)
	{
		Proc_Break(cpOrderProc);
		return;
	}


	gAiState.unitIt = gAiState.units;
	AiDecideMainFunc = decisionMaker;

	Proc_StartBlocking(gProcScr_CpDecide, cpOrderProc);
}
