#include "common-chax.h"
#include "bwl.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "battle-system.h"
#include "eventinfo.h"
#include "../../../../Contants/Texts/build/msgs.h"

#define SUPPORT_RATE_KILL 100
#define SUPPORT_RATE_COMBAT 100
#define SUPPORT_RATE_STAFF 100
#define SUPPORT_RATE_DANCE 100

static const struct SupportTalkEnt gNewSupportTalkList[] = 
{
    { CHARACTER_EIRIKA,   CHARACTER_EPHRAIM,  {MSG_0C53, MSG_0C54, MSG_0C55} },
    { CHARACTER_EIRIKA,   CHARACTER_SETH,     {MSG_0C56, MSG_0C57, MSG_0C58} },
    { CHARACTER_EIRIKA,   CHARACTER_SALEH,    {MSG_0C65, MSG_0C66, MSG_0C67} },
    { CHARACTER_EIRIKA,   CHARACTER_LARACHEL, {MSG_0C59, MSG_0C5A, MSG_0C5B} },
    { CHARACTER_EIRIKA,   CHARACTER_TANA,     {MSG_0C5C, MSG_0C5D, MSG_0C5E} },
    { CHARACTER_EIRIKA,   CHARACTER_INNES,    {MSG_0C5F, MSG_0C60, MSG_0C61} },
    { CHARACTER_EIRIKA,   CHARACTER_FORDE,    {MSG_0C62, MSG_0C63, MSG_0C64} },
    { CHARACTER_EPHRAIM,  CHARACTER_KYLE,     {MSG_0C68, MSG_0C69, MSG_0C6A} },
    { CHARACTER_EPHRAIM,  CHARACTER_FORDE,    {MSG_0C6B, MSG_0C6C, MSG_0C6D} },
    { CHARACTER_EPHRAIM,  CHARACTER_MYRRH,    {MSG_0C6E, MSG_0C6F, MSG_0C70} },
    { CHARACTER_EPHRAIM,  CHARACTER_LARACHEL, {MSG_0C71, MSG_0C72, MSG_0C73} },
    { CHARACTER_EPHRAIM,  CHARACTER_DUESSEL,  {MSG_0C74, MSG_0C75, MSG_0C76} },
    { CHARACTER_EPHRAIM,  CHARACTER_TANA,     {MSG_0C77, MSG_0C78, MSG_0C79} },
    { CHARACTER_SETH,     CHARACTER_FRANZ,    {MSG_0C7A, MSG_0C7B, MSG_0C7C} },
    { CHARACTER_SETH,     CHARACTER_GARCIA,   {MSG_0C7D, MSG_0C7E, MSG_0C7F} },
    { CHARACTER_SETH,     CHARACTER_NATASHA,  {MSG_0C80, MSG_0C81, MSG_0C82} },
    { CHARACTER_SETH,     CHARACTER_CORMAG,   {MSG_0C83, MSG_0C84, MSG_0C85} },
    { CHARACTER_FRANZ,    CHARACTER_GILLIAM,  {MSG_0C86, MSG_0C87, MSG_0C88} },
    { CHARACTER_FRANZ,    CHARACTER_FORDE,    {MSG_0C8C, MSG_0C8D, MSG_0C8E} },
    { CHARACTER_FRANZ,    CHARACTER_AMELIA,   {MSG_0C8F, MSG_0C90, MSG_0C91} },
    { CHARACTER_FRANZ,    CHARACTER_NATASHA,  {MSG_0C89, MSG_0C8A, MSG_0C8B} },
    { CHARACTER_GILLIAM,  CHARACTER_GARCIA,   {MSG_0C92, MSG_0C93, MSG_0C94} },
    { CHARACTER_GILLIAM,  CHARACTER_SYRENE,   {MSG_0C95, MSG_0C96, MSG_0C97} },
    { CHARACTER_GILLIAM,  CHARACTER_MOULDER,  {MSG_0C98, MSG_0C99, MSG_0C9A} },
    { CHARACTER_GILLIAM,  CHARACTER_NEIMI,    {MSG_0C9B, MSG_0C9C, MSG_0C9D} },
    { CHARACTER_MOULDER,  CHARACTER_VANESSA,  {MSG_0C9E, MSG_0C9F, MSG_0CA0} },
    { CHARACTER_MOULDER,  CHARACTER_COLM,     {MSG_0CA1, MSG_0CA2, MSG_0CA3} },
    { CHARACTER_MOULDER,  CHARACTER_SYRENE,   {MSG_0CA4, MSG_0CA5, MSG_0CA6} },
    { CHARACTER_VANESSA,  CHARACTER_SYRENE,   {MSG_0CA7, MSG_0CA8, MSG_0CA9} },
    { CHARACTER_VANESSA,  CHARACTER_LUTE,     {MSG_0CAA, MSG_0CAB, MSG_0CAC} },
    { CHARACTER_VANESSA,  CHARACTER_FORDE,    {MSG_0CAD, MSG_0CAE, MSG_0CAF} },
    { CHARACTER_VANESSA,  CHARACTER_INNES,    {MSG_0CB0, MSG_0CB1, MSG_0CB2} },
    { CHARACTER_GARCIA,   CHARACTER_ROSS,     {MSG_0CB3, MSG_0CB4, MSG_0CB5} },
    { CHARACTER_GARCIA,   CHARACTER_DOZLA,    {MSG_0CB6, MSG_0CB7, MSG_0CB8} },
    { CHARACTER_GARCIA,   CHARACTER_NEIMI,    {MSG_0CB9, MSG_0CBA, MSG_0CBB} },
    { CHARACTER_ROSS,     CHARACTER_LUTE,     {MSG_0CBC, MSG_0CBD, MSG_0CBE} },
    { CHARACTER_ROSS,     CHARACTER_AMELIA,   {MSG_0CBF, MSG_0CC0, MSG_0CC1} },
    { CHARACTER_ROSS,     CHARACTER_EWAN,     {MSG_0CC2, MSG_0CC3, MSG_0CC4} },
    { CHARACTER_ROSS,     CHARACTER_GERIK,    {MSG_0CC5, MSG_0CC6, MSG_0CC7} },
    { CHARACTER_COLM,     CHARACTER_NEIMI,    {MSG_0CC8, MSG_0CC9, MSG_0CCA} },
    { CHARACTER_COLM,     CHARACTER_MARISA,   {MSG_0CCB, MSG_0CCC, MSG_0CCD} },
    { CHARACTER_COLM,     CHARACTER_RENNAC,   {MSG_0CCE, MSG_0CCF, MSG_0CD0} },
    { CHARACTER_COLM,     CHARACTER_KYLE,     {MSG_0CD1, MSG_0CD2, MSG_0CD3} },
    { CHARACTER_NEIMI,    CHARACTER_AMELIA,   {MSG_0CD4, MSG_0CD5, MSG_0CD6} },
    { CHARACTER_NEIMI,    CHARACTER_ARTUR,    {MSG_0CD7, MSG_0CD8, MSG_0CD9} },
    { CHARACTER_ARTUR,    CHARACTER_LUTE,     {MSG_0CDA, MSG_0CDB, MSG_0CDC} },
    { CHARACTER_ARTUR,    CHARACTER_TETHYS,   {MSG_0CDD, MSG_0CDE, MSG_0CDF} },
    { CHARACTER_ARTUR,    CHARACTER_CORMAG,   {MSG_0CE0, MSG_0CE1, MSG_0CE2} },
    { CHARACTER_ARTUR,    CHARACTER_JOSHUA,   {MSG_0CE3, MSG_0CE4, MSG_0CE5} },
    { CHARACTER_LUTE,     CHARACTER_KNOLL,    {MSG_0CE6, MSG_0CE7, MSG_0CE8} },
    { CHARACTER_LUTE,     CHARACTER_KYLE,     {MSG_0CE9, MSG_0CEA, MSG_0CEB} },
    { CHARACTER_NATASHA,  CHARACTER_JOSHUA,   {MSG_0CEC, MSG_0CED, MSG_0CEE} },
    { CHARACTER_NATASHA,  CHARACTER_KNOLL,    {MSG_0CEF, MSG_0CF0, MSG_0CF1} },
    { CHARACTER_NATASHA,  CHARACTER_CORMAG,   {MSG_0CF2, MSG_0CF3, MSG_0CF4} },
    { CHARACTER_JOSHUA,   CHARACTER_INNES,    {MSG_0CF5, MSG_0CF6, MSG_0CF7} },
    { CHARACTER_JOSHUA,   CHARACTER_LARACHEL, {MSG_0CF8, MSG_0CF9, MSG_0CFA} },
    { CHARACTER_JOSHUA,   CHARACTER_MARISA,   {MSG_0CFB, MSG_0CFC, MSG_0CFD} },
    { CHARACTER_JOSHUA,   CHARACTER_GERIK,    {MSG_0CFE, MSG_0CFF, MSG_0D00} },
    { CHARACTER_KYLE,     CHARACTER_FORDE,    {MSG_0D01, MSG_0D02, MSG_0D03} },
    { CHARACTER_KYLE,     CHARACTER_SYRENE,   {MSG_0D04, MSG_0D05, MSG_0D06} },
    { CHARACTER_TANA,     CHARACTER_SYRENE,   {MSG_0D07, MSG_0D08, MSG_0D09} },
    { CHARACTER_TANA,     CHARACTER_CORMAG,   {MSG_0D0A, MSG_0D0B, MSG_0D0C} },
    { CHARACTER_TANA,     CHARACTER_INNES,    {MSG_0D0D, MSG_0D0E, MSG_0D0F} },
    { CHARACTER_TANA,     CHARACTER_MARISA,   {MSG_0D10, MSG_0D11, MSG_0D12} },
    { CHARACTER_INNES,    CHARACTER_GERIK,    {MSG_0D13, MSG_0D14, MSG_0D15} },
    { CHARACTER_INNES,    CHARACTER_LARACHEL, {MSG_0D16, MSG_0D17, MSG_0D18} },
    { CHARACTER_GERIK,    CHARACTER_TETHYS,   {MSG_0D19, MSG_0D1A, MSG_0D1B} },
    { CHARACTER_GERIK,    CHARACTER_MARISA,   {MSG_0D1C, MSG_0D1D, MSG_0D1E} },
    { CHARACTER_GERIK,    CHARACTER_SALEH,    {MSG_0D1F, MSG_0D20, MSG_0D21} },
    { CHARACTER_TETHYS,   CHARACTER_EWAN,     {MSG_0D22, MSG_0D23, MSG_0D24} },
    { CHARACTER_TETHYS,   CHARACTER_MARISA,   {MSG_0D25, MSG_0D26, MSG_0D27} },
    { CHARACTER_TETHYS,   CHARACTER_RENNAC,   {MSG_0D28, MSG_0D29, MSG_0D2A} },
    { CHARACTER_SALEH,    CHARACTER_MYRRH,    {MSG_0D2B, MSG_0D2C, MSG_0D2D} },
    { CHARACTER_SALEH,    CHARACTER_EWAN,     {MSG_0D2E, MSG_0D2F, MSG_0D30} },
    { CHARACTER_EWAN,     CHARACTER_DOZLA,    {MSG_0D31, MSG_0D32, MSG_0D33} },
    { CHARACTER_EWAN,     CHARACTER_AMELIA,   {MSG_0D34, MSG_0D35, MSG_0D36} },
    { CHARACTER_LARACHEL, CHARACTER_DOZLA,    {MSG_0D37, MSG_0D38, MSG_0D39} },
    { CHARACTER_LARACHEL, CHARACTER_RENNAC,   {MSG_0D3A, MSG_0D3B, MSG_0D3C} },
    { CHARACTER_DOZLA,    CHARACTER_MYRRH,    {MSG_0D3D, MSG_0D3E, MSG_0D3F} },
    { CHARACTER_DOZLA,    CHARACTER_RENNAC,   {MSG_0D40, MSG_0D41, MSG_0D42} },
    { CHARACTER_CORMAG,   CHARACTER_DUESSEL,  {MSG_0D43, MSG_0D44, MSG_0D45} },
    { CHARACTER_AMELIA,   CHARACTER_DUESSEL,  {MSG_0D46, MSG_0D47, MSG_0D48} },
    { CHARACTER_DUESSEL,  CHARACTER_KNOLL,    {MSG_0D49, MSG_0D4A, MSG_0D4B} },
    { 0xFFFF,             0x0000,             {0x0000, 0x000, 0x0000} }
};

#define LOCAL_TRACE 0

static u8 * GetUnitBwlSupports(u8 pid)
{
    struct NewBwl * bwl = GetNewBwl(pid);
    if (bwl)
        return bwl->supports;

    return NULL;
}

LYN_REPLACE_CHECK(CallMapSupportEvent);
void CallMapSupportEvent(u16 musicIndex, u16 textIndex) {
    // Calls text with music (just quiets music when id is 0)
    // On-map supports?
    CallEvent((u16 *)EventScr_MapSupportConversation, EV_EXEC_CUTSCENE);

    gEventSlots[0x2] = musicIndex;
    gEventSlots[0x3] = textIndex;
}

LYN_REPLACE_CHECK(CallSupportViewerEvent);
void CallSupportViewerEvent(u16 textIndex) {
    // Calls text with random background (support viewer?)
    CallEvent((u16 *)EventScr_SupportViewerConversation, EV_EXEC_QUIET);

    gEventSlots[0x2] = textIndex;
}

//! FE8U = 0x080847F8
LYN_REPLACE_CHECK(GetSupportTalkList);
struct SupportTalkEnt* GetSupportTalkList(void) {
    return (struct SupportTalkEnt*)gNewSupportTalkList;
}

//! FE8U = 0x08084748
LYN_REPLACE_CHECK(GetSupportTalkEntry);
struct SupportTalkEnt * GetSupportTalkEntry(u16 pidA, u16 pidB)
{
    const struct SupportTalkEnt * it;

    for (it = gNewSupportTalkList; it->unitA != 0xFFFF; it++)
    {
        if ((pidA == it->unitA) && (pidB == it->unitB))
            return (struct SupportTalkEnt *)it;  // Cast const away only on return
            
        if ((pidB == it->unitA) && (pidA == it->unitB))
            return (struct SupportTalkEnt *)it;  // Cast const away only on return
    }
    
    return NULL;
}

//! FE8U = 0x080a3724
bool UpdateBestGlobalSupportValue(int unitA, int unitB, int supportRank) {
    int convo;
    int var0;
    int var1;
    struct GlobalSaveInfo info;
    struct SupportTalkEnt* ptr;

    supportRank = supportRank & 3;

    if (!ReadGlobalSaveInfo(&info)) {
        return 0;
    }

    convo = 0;

    for (ptr = GetSupportTalkList(); ; ptr++) {

        if (ptr->unitA == 0xFFFF)
            break;

        if ((ptr->unitA == unitA) && (ptr->unitB == unitB))
            break;

        if ((ptr->unitA == unitB) && (ptr->unitB == unitA))
            break;

        convo++;
    }

    var0 = convo >> 2;
    var1 = (convo & 3) << 1;

    if (((info.SuppordRecord[var0] >> var1) & 3) >= (supportRank))
        return false;

    info.SuppordRecord[var0] &= ~(3 << var1);
    info.SuppordRecord[var0] += (supportRank << var1);

    WriteGlobalSaveInfo(&info);

    return true;
}

//! FE8U = 0x0808371C
LYN_REPLACE_CHECK(StartSupportTalk);
void StartSupportTalk(u8 pidA, u8 pidB, int rank) {
    struct SupportTalkEnt* ent = GetSupportTalkEntry(pidA, pidB);

    if (ent) {
        CallMapSupportEvent(
            GetSupportTalkSong(pidA, pidB, rank),
            ent->msgSupports[rank - 1]
        );

        UpdateBestGlobalSupportValue(pidA, pidB, rank);
    }

    return;
}

LYN_REPLACE_CHECK(ActionSupport);
s8 ActionSupport(ProcPtr proc)
{
    int subjectExp;
    int targetExp;
    u8 * supp1, * supp2;

    struct Unit * target = GetUnit(gActionData.targetIndex);

    int targetSupportNum = GetUnitSupporterNum(gActiveUnit, target->pCharacterData->number);
    int subjectSupportNum = GetUnitSupporterNum(target, gActiveUnit->pCharacterData->number);

    CanUnitSupportNow(target, subjectSupportNum);

    UnitGainSupportLevel(gActiveUnit, targetSupportNum);
    UnitGainSupportLevel(target, subjectSupportNum);

    StartSupportTalk(
        gActiveUnit->pCharacterData->number,
        target->pCharacterData->number,
        GetUnitSupportLevel(gActiveUnit, targetSupportNum)
    );

    supp1 = GetUnitBwlSupports(UNIT_CHAR_ID(gActiveUnit));
    supp2 = GetUnitBwlSupports(UNIT_CHAR_ID(target));

    if (supp1 && supp2)
    {
        subjectExp = supp1[targetSupportNum];
        targetExp = supp2[subjectSupportNum];

        if (subjectExp != targetExp)
        {
            if (subjectExp > targetExp)
                supp2[subjectSupportNum] = subjectExp;

            if (subjectExp < targetExp)
                supp1[targetSupportNum] = targetExp;
        }
    }

    return 0;
}

LYN_REPLACE_CHECK(GetUnitSupportLevel);
int GetUnitSupportLevel(struct Unit * unit, int num)
{
    int exp;
    u8 * supp;

    if (!UNIT_IS_VALID(unit))
        return SUPPORT_LEVEL_NONE;

    supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));
    if (supp)
    {
        exp = supp[num];
        // LTRACEF("[pid=%d, num=%d] exp=%d at BWL", UNIT_CHAR_ID(unit), num, exp);
    }
    else
    {
        /**
         * for none-BWL characters,
         * directly judge on its rom data
         */
        exp = unit->pCharacterData->pSupportData->supportExpBase[num];
        // LTRACEF("[pid=%d, num=%d] exp=%d at ROM", UNIT_CHAR_ID(unit), num, exp);
    }

    if (exp > 240)
        return SUPPORT_LEVEL_A;

    if (exp > 160)
        return SUPPORT_LEVEL_B;

    if (exp > 80)
        return SUPPORT_LEVEL_C;

    return SUPPORT_LEVEL_NONE;
}

LYN_REPLACE_CHECK(UnitGainSupportExp);
void UnitGainSupportExp(struct Unit * unit, int num)
{
    u8 * supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));

    if (UNIT_SUPPORT_DATA(unit) && supp)
    {
        int gain = UNIT_SUPPORT_DATA(unit)->supportExpGrowth[num];
#ifdef CONFIG_VESLY_SUPPORT_POST_BATTLE
    if (gBattleActorGlobalFlag.enemy_defeated)
        gain += SUPPORT_RATE_KILL;
    else if (gActionData.unitActionType == UNIT_ACTION_COMBAT)
        gain += SUPPORT_RATE_COMBAT;
    else if (gActionData.unitActionType == UNIT_ACTION_DANCE)
        gain += SUPPORT_RATE_DANCE;
    else if (gActionData.unitActionType == UNIT_ACTION_STAFF)
        gain += SUPPORT_RATE_STAFF;
#else
        gain = UNIT_SUPPORT_DATA(unit)->supportExpGrowth[num];
#endif
        int currentExp = supp[num];
        int maxExp = sSupportMaxExpLookup[GetUnitSupportLevel(unit, num)];

        FORCE_DECLARE struct Unit * other = GetUnitSupporterUnit(unit, num);

#if defined(SID_SocialButterfly) && (COMMON_SKILL_VALID(SID_SocialButterfly))
        if (SkillTester(unit, SID_SocialButterfly) || SkillTester(other, SID_SocialButterfly))
            gain *= 2;
#endif

        if (currentExp + gain > maxExp)
            gain = maxExp - currentExp;

        supp[num] = currentExp + gain;
        gPlaySt.chapterTotalSupportGain += gain;
    }
}

LYN_REPLACE_CHECK(UnitGainSupportLevel);
void UnitGainSupportLevel(struct Unit* unit, int num)
{
    u8 * supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));
    if (supp)
        supp[num]++;

    gPlaySt.chapterTotalSupportGain++;

    SetSupportLevelGained(unit->pCharacterData->number, GetUnitSupporterCharacter(unit, num));
}

LYN_REPLACE_CHECK(CanUnitSupportNow);
s8 CanUnitSupportNow(struct Unit * unit, int num)
{
    int exp, maxExp;
    u8 * supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));

    if (gPlaySt.chapterStateBits & PLAY_FLAG_EXTRA_MAP)
        return FALSE;

    if (gPlaySt.chapterStateBits & PLAY_FLAG_TUTORIAL)
        return FALSE;

    if (HasUnitGainedSupportLevel(unit, num))
        return FALSE;

    if (GetUnitTotalSupportLevel(unit) >= MAX_SIMULTANEOUS_SUPPORT_COUNT)
        return FALSE;

    if (GetUnitTotalSupportLevel(GetUnitSupporterUnit(unit, num)) >= MAX_SIMULTANEOUS_SUPPORT_COUNT)
        return FALSE;

    if (!supp)
        return FALSE;

    exp    = supp[num];
    maxExp = sSupportMaxExpLookup[GetUnitSupportLevel(unit, num)];

    if (exp == SUPPORT_EXP_A)
        return FALSE;

    return (exp == maxExp) ? TRUE : FALSE;
}

LYN_REPLACE_CHECK(InitUnitsupports);
void InitUnitsupports(struct Unit * unit)
{
    u8 * supp1, * supp2;
    int i, count = GetUnitSupporterCount(unit);

    for (i = 0; i < count; ++i)
    {
        struct Unit * other = GetUnitSupporterUnit(unit, i);

        if (!other)
            continue;

        supp1 = GetUnitBwlSupports(UNIT_CHAR_ID(unit));
        supp2 = GetUnitBwlSupports(UNIT_CHAR_ID(other));

        if (!supp1 || !supp2)
            continue;

        supp2[GetUnitSupporterNum(other, unit->pCharacterData->number)] = 0;
        supp1[i] = 0;
    }
}

LYN_REPLACE_CHECK(UnitLoadSupports);
void UnitLoadSupports(struct Unit * unit)
{
    int i, count = GetUnitSupporterCount(unit);
    u8 * supp = GetUnitBwlSupports(UNIT_CHAR_ID(unit));
    if (supp)
    {
        CpuFill16(0, supp, UNIT_SUPPORT_MAX_COUNT);

        for (i = 0; i < count; ++i)
            supp[i] = GetUnitSupporterInitialExp(unit, i);
    }
}
