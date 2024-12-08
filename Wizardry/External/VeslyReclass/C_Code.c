
#include "C_Code.h" // headers
#define PUREFUNC __attribute__((pure))
int Mod(int a, int b) PUREFUNC;

#define NumberOfReclassOptions 6 // max 6
struct magClassTable
{
    u8 base;
    u8 growth;
    u8 cap;
    u8 promo;
};
extern struct magClassTable MagClassTable[];
ProcPtr ReclassMenuSelect(ProcPtr parent);
ProcPtr StartReclassSelect(ProcPtr parent);
extern u8 const ReclassTable[][6];
extern u8 const UnitOverrideReclassTable_Unpromoted[][7];
extern u8 const UnitOverrideReclassTable_Promoted[][7];
// extern u8* pPromoJidLut;

int GetReclassTableID(const u8 * table, int size, int classID)
{
    for (int i = 0; i < size; ++i)
    {
        // if (i > ID)
        // {
        // break;
        // }
        if (table[i] == classID)
        {
            return i;
        }
    }
    return (-1);
}

int GetReclassOption(int unitID, int classID, int ID)
{
    int result = 0;
    int ID_orig = ID;
    int reclassTableID = (-1);
    if (GetClassData(classID)->attributes & CA_PROMOTED)
    {
        reclassTableID = GetReclassTableID(UnitOverrideReclassTable_Promoted[unitID], 7, classID);
        if (UnitOverrideReclassTable_Promoted[unitID][0])
        {
            if ((!UnitOverrideReclassTable_Promoted[unitID][5]) ||
                ((!UnitOverrideReclassTable_Promoted[unitID][6]) && (reclassTableID >= 0)))
            {
                ID--;
                if (!ID_orig)
                {
                    return classID;
                }
            }
            if ((ID >= reclassTableID) && (reclassTableID >= 0))
            {
                ID++;
            }
            return UnitOverrideReclassTable_Promoted[unitID][ID];
        }
    }
    else
    {
        ID = ID_orig;
        reclassTableID = GetReclassTableID(UnitOverrideReclassTable_Unpromoted[unitID], 7, classID);

        if (UnitOverrideReclassTable_Unpromoted[unitID][0])
        {
            if ((!UnitOverrideReclassTable_Unpromoted[unitID][5]) ||
                ((!UnitOverrideReclassTable_Unpromoted[unitID][6]) && (reclassTableID >= 0)))
            {
                ID--;
                if (!ID_orig)
                {
                    return classID;
                }
            }
            if ((ID >= reclassTableID) && (reclassTableID >= 0))
            {
                ID++;
            }
            return UnitOverrideReclassTable_Unpromoted[unitID][ID];
        }
    }
    ID = ID_orig;
    if (ReclassTable[classID][0] && (!ReclassTable[classID][5]))
    {
        ID--;
        if (!ID_orig)
        {
            return classID;
        }
    }
    result = ReclassTable[classID][ID];
    return result;
}
int IsStrMagInstalled(void)
{
    return MagClassTable[0].cap;
}

extern int PromoMain_SetupTraineeEvent_(struct ProcPromoMain * proc);
extern bool PromoTraineeEventExists(struct ProcPromoMain * proc);
extern bool sub_80CD330(struct ProcPromoMain * proc);

struct ProcReclassSel
{ // see ProcPromoSel
    PROC_HEADER;
    s8 _u29;
    s8 _u2a;
    s8 _u2b;
    u8 jid[6]; // NumberOfReclassOptions
    u16 sprite[3];
    s16 msg_desc[3]; // previously msg_desc
    u16 _u3e;
    u8 stat;
    u8 main_select;
    u16 pid;
    u16 u44;
    u8 u46;
    u8 u47;
    u16 weapon;
    u8 use_wpn[3]; // unused
    u8 _u4d[3];
    u32 u50; // platform ID?
    ProcPtr menu_proc;
    // s16 msg_desc[6]; // NumberOfReclassOptions
    /* ... more maybe */
};
void ReclassMenuExec(struct ProcClassChgMenuSel * proc);

const struct ProcCmd ProcScr_ReclassMenuSel[] = {
    PROC_SLEEP(6),
    PROC_NAME("Reclass Menu Select"),
    PROC_CALL(ReclassMenuExec),
    PROC_REPEAT(nullsub_80CDDD4),
    PROC_YIELD,

    PROC_LABEL(0),
    PROC_GOTO(2),

    PROC_LABEL(1),
    PROC_CALL(nullsub_61),

    PROC_LABEL(2),
    PROC_END,
};

void ReclassSubConfirm_OnInit(struct MenuProc * proc)
{
    SyncMenuBgs(proc);
}
void ReclassDrawStatChanges(struct Unit * unit, const struct ClassData * classData);
void ReclassSubConfirm_OnEnd(struct MenuProc * proc)
{
    TileMap_FillRect(TILEMAP_LOCATED(gBG0TilemapBuffer, 8, 4), 0xA, 6, 0);
    TileMap_FillRect(TILEMAP_LOCATED(gBG2TilemapBuffer, 8, 4), 0xA, 6, 0);
    SetTextFont(&gFontClassChg);
    sub_80CDA4C(proc->proc_parent);
    RedrawMenu(proc->proc_parent);
    SyncMenuBgs(proc);

    struct ProcClassChgMenuSel * parent;
    struct ProcReclassSel * gparent;

    proc = proc->proc_parent;
    parent = proc->proc_parent;
    gparent = parent->proc_parent;
    // gparent->stat = 1;
    // gparent->main_select = _proc2->itemNumber;

    struct Unit * unit = GetUnitFromCharId(gparent->pid);
    const struct ClassData * classData =
        GetClassData(GetReclassOption(gparent->pid, unit->pClassData->number, proc->itemCurrent));
    ReclassDrawStatChanges(unit, classData);
}

bool StartAndWaitReclassSelect(struct ProcPromoMain * proc);
void ReclassHandlerIdle(struct ProcPromoHandler * proc);
const struct ProcCmd ProcScr_ReclassHandler[] = {
    PROC_SLEEP(3), PROC_NAME("Reclass Handler"),
    PROC_LABEL(0), PROC_CALL(PromoHandler_SetInitStat),

    PROC_LABEL(1), PROC_REPEAT(ReclassHandlerIdle),

    PROC_LABEL(7), PROC_END,
};

void SetLevelFunc(ProcPtr proc)
{
    if (!Proc_Find(ProcScr_ReclassHandler))
    {
        Proc_Break(proc);
    }
    gEkrLvupPostLevel = gActiveUnit->level;
}

const struct ProcCmd ProcScr_SetLevel[] = {
    PROC_SLEEP(0),
    PROC_NAME("Reclass Set Level"),
    PROC_REPEAT(SetLevelFunc),
    PROC_END,
};

extern void ReclassPostConfirmWaitBanimEnd(struct ProcClassChgPostConfirm * proc)
{
    gEkrLvupPostLevel = gActiveUnit->level;
    int game_lock = proc->game_lock;
    if (game_lock == GetGameLock())
        Proc_Break(proc);
}
void ReclassChgExecPromotionReal(struct ProcClassChgPostConfirm * proc);
const struct ProcCmd ProcScr_ReclassChgReal[] = { PROC_NAME("Reclassing Active"),
                                                  PROC_WHILE(MusicProc4Exists),
                                                  PROC_CALL(ReclassChgExecPromotionReal),
                                                  PROC_REPEAT(ReclassPostConfirmWaitBanimEnd),
                                                  PROC_SLEEP(0x8),
                                                  PROC_CALL(sub_80CDE98),
                                                  PROC_SLEEP(0x5),
                                                  PROC_WHILE(MusicProc4Exists),
                                                  PROC_END };

void ExecUnitReclass(struct Unit * unit, u8 classId, int itemIdx, s8 unk);
void ReclassChgExecPromotionReal(struct ProcClassChgPostConfirm * proc)
{
    struct ProcPromoMain * parent = proc->proc_parent;
    struct ProcPromoHandler * gparent = parent->proc_parent;

    struct Unit * unit = GetUnitFromCharId(parent->pid);

    if (unit == NULL)
    {
        Proc_End(proc);
        return;
    }

    proc->game_lock = GetGameLock();
    SetWinEnable(0, 0, 0);
    ExecUnitReclass(unit, parent->jid, -1, 0);

    if (gparent->bmtype != PROMO_HANDLER_TYPE_PREP)
        gBattleStats.config = BATTLE_CONFIG_PROMOTION_PREP | BATTLE_CONFIG_PROMOTION;
    else
        gBattleStats.config = BATTLE_CONFIG_PROMOTION;

    BeginBattleAnimations();
}

// extern void SetBlendConfig(int, int, int, int);
extern void sub_80CDE98(struct ProcClassChgPostConfirm * proc);
//{
//    struct ProcPromoMain *parent = proc->proc_parent;
//    GetUnitFromCharId(parent->pid);
//}

void ExecReclassChgReal(struct ProcPromoMain * proc)
{
    int slot;
    struct ProcPromoHandler * parent = proc->proc_parent;
    gUnknown_03005398 = -1;
    EndCgText();

    ResetDialogueScreen();
    APProc_DeleteAll();
    EndMuralBackground_();

    gLCDControlBuffer.bg0cnt.priority = 0;
    gLCDControlBuffer.bg1cnt.priority = 1;
    gLCDControlBuffer.bg2cnt.priority = 2;
    gLCDControlBuffer.bg3cnt.priority = 3;

    SetBlendConfig(3, 0, 0, 0x10);
    SetBlendTargetA(1, 1, 1, 1, 1);

    EndAllProcChildren(proc);

    Proc_StartBlocking(ProcScr_ReclassChgReal, proc);

    if (parent->bmtype != PROMO_HANDLER_TYPE_TRANINEE)
    {
        slot = parent->item_slot;
        if (slot != -1)
            UnitUpdateUsedItem(parent->unit, slot);
    }
}

int GetStatDiff(int id, struct Unit * unit, const struct ClassData * oldClass, const struct ClassData * newClass)
{
    int result = 0;

    switch (id)
    {
        case 0:
        {
            result = newClass->baseHP - oldClass->baseHP;
            break;
        }
        case 1:
        {
            result = newClass->basePow - oldClass->basePow;
            break;
        }
        case 2:
        {
            result = newClass->baseSkl - oldClass->baseSkl;
            break;
        }
        case 3:
        {
            result = newClass->baseSpd - oldClass->baseSpd;
            break;
        }
        case 4:
        {
            result = newClass->baseDef - oldClass->baseDef;
            break;
        }
        case 5:
        {
            result = newClass->baseRes - oldClass->baseRes;
            break;
        }
        case 6:
        {
            result = newClass->baseCon - oldClass->baseCon;
            break;
        }
        case 7:
        {
            result = newClass->baseMov - oldClass->baseMov;
            break;
        }
        case 8:
        {
            if (!IsStrMagInstalled())
            {
                break;
            }
            result = MagClassTable[newClass->number].base - MagClassTable[oldClass->number].base;
            break;
        }
        default:
    }
    int tmp = 0;
    switch (id)
    {
        case 0:
        {
            tmp = unit->maxHP;
            break;
        }
        case 1:
        {
            tmp = unit->pow;
            break;
        }
        case 2:
        {
            tmp = unit->skl;
            break;
        }
        case 3:
        {
            tmp = unit->spd;
            break;
        }
        case 4:
        {
            tmp = unit->def;
            break;
        }
        case 5:
        {
            tmp = unit->res;
            break;
        }
        case 6:
        {
            tmp = oldClass->baseCon;
            break;
        }
        case 7:
        {
            tmp = oldClass->baseMov;
            break;
        }
        case 8:
        {
            if (!IsStrMagInstalled())
            {
                break;
            }
            tmp = unit->_u3A;
            break;
        }
        default:
    }
    if ((tmp + result) < 0)
    {
        result = 0;
    }
    return result;
}

void ApplyUnitReclass(struct Unit * unit, u8 classId)
{
    const struct ClassData * newClass = GetClassData(classId);

    int baseClassId = unit->pClassData->number;
    // int promClassId = newClass->number;

    const struct ClassData * oldClass = GetClassData(baseClassId);

    int i;

    int tmp;
    unit->maxHP += GetStatDiff(0, unit, oldClass, newClass);
    unit->pow += GetStatDiff(1, unit, oldClass, newClass);
    unit->skl += GetStatDiff(2, unit, oldClass, newClass);
    unit->spd += GetStatDiff(3, unit, oldClass, newClass);
    unit->def += GetStatDiff(4, unit, oldClass, newClass);
    unit->res += GetStatDiff(5, unit, oldClass, newClass);
    tmp = unit->_u3A; // handle mag separately because _u3A is unsigned
    tmp += GetStatDiff(8, unit, oldClass, newClass);
    if (tmp < 0)
    {
        tmp = 0;
    }
    unit->_u3A = tmp;

    if (unit->maxHP < 0)
    {
        unit->maxHP = 0;
    }
    if (unit->pow < 0)
    {
        unit->pow = 0;
    }
    if (unit->skl < 0)
    {
        unit->skl = 0;
    }
    if (unit->spd < 0)
    {
        unit->spd = 0;
    }
    if (unit->def < 0)
    {
        unit->def = 0;
    }
    if (unit->res < 0)
    {
        unit->res = 0;
    }
    // unit->lck += newClass->baseLck - oldClass->basePow;
    // unit->_u3A += newClass->basePow - oldClass->basePow; // mag

    // Remove base class' base wexp from unit wexp
    for (i = 0; i < 8; ++i)
    {
        tmp = unit->ranks[i] - oldClass->baseRanks[i];
        if (tmp >= 0)
        {
            unit->ranks[i] = tmp;
        }
    }

    // Update unit class
    unit->pClassData = newClass;

    // Add promoted class' base wexp to unit wexp
    for (i = 0; i < 8; ++i)
    {
        int wexp = unit->ranks[i];
        tmp = newClass->baseRanks[i];

        if (!tmp)
        {
            unit->ranks[i] = 0;
            continue;
        } // if new class has no rank, set to 0
        wexp += tmp;

        if (wexp > WPN_EXP_S)
            wexp = WPN_EXP_S;

        unit->ranks[i] = wexp;
    }

    // unit->level = 1;
    // unit->exp   = 0;
    UnitCheckStatCaps(unit);
    unit->curHP += newClass->promotionHp;

    if (unit->curHP > GetUnitMaxHp(unit))
        unit->curHP = GetUnitMaxHp(unit);
}

int CanClassEquipWeapon(int weapon, int reclassID)
{
    weapon &= 0xFF; // id only
    int weaponType = GetItemData(weapon)->weaponType;
    return GetClassData(reclassID)->baseRanks[weaponType];
}

void ExecUnitReclass(struct Unit * unit, u8 classId, int itemIdx, s8 unk)
{

    if (itemIdx != -1)
    {
        gBattleActor.weaponBefore = gBattleTarget.weaponBefore = unit->items[itemIdx];
    }

    int weapon = GetUnitEquippedWeapon(unit);
    if (!CanClassEquipWeapon(weapon, classId))
    {
        weapon = 0;
    }
    gBattleActor.weapon = gBattleTarget.weapon = weapon;
    // gBattleActor.weaponBefore = weapon;

    InitBattleUnitWithoutBonuses(&gBattleTarget, unit);

    ApplyUnitReclass(unit, classId);

    InitBattleUnitWithoutBonuses(&gBattleActor, unit);

    GenerateBattleUnitStatGainsComparatively(&gBattleActor, &gBattleTarget.unit);
    // save from battle?
    // struct Unit* actor  = GetUnit(gBattleActor.unit.index);
    // UpdateUnitFromBattle(actor, &gBattleActor);
    // BattleApplyUnitUpdates();
    SetBattleUnitTerrainBonusesAuto(&gBattleActor);
    SetBattleUnitTerrainBonusesAuto(&gBattleTarget);

    if (unk)
    {
        unit->state |= US_HAS_MOVED;
    }

    if (itemIdx != -1)
    {
        UnitUpdateUsedItem(unit, itemIdx);
    }

    gBattleHitArray[0].attributes = 0;
    gBattleHitArray[0].info = BATTLE_HIT_INFO_END;
    gBattleHitArray[0].hpChange = 0;

    gBattleStats.config = BATTLE_CONFIG_PROMOTION;

    return;
}

const struct ProcCmd ProcScr_ReclassMain[] = {
    PROC_NAME("Reclass Main"),

    PROC_LABEL(PROMOMAIN_LABEL_START),
    PROC_CALL(PromoMain_InitScreen),
    PROC_SLEEP(3),

    PROC_LABEL(PROMOMAIN_LABEL_1),
    PROC_CALL(PromoMain_HandleType),

    PROC_LABEL(PROMOMAIN_LABEL_TRAINEE),
    PROC_WHILE(PromoMain_SetupTraineeEvent_),
    PROC_WHILE(PromoTraineeEventExists),
    PROC_CALL(PromoHandleTraineePostType),

    PROC_LABEL(PROMOMAIN_LABEL_SEL_EN),
    PROC_WHILE(StartAndWaitReclassSelect),
    PROC_SLEEP(5),
    PROC_REPEAT(sub_80CD330),

    PROC_LABEL(PROMOMAIN_LABEL_POST_SEL),
    PROC_CALL(ExecReclassChgReal),
    PROC_SLEEP(2),

    PROC_LABEL(6),
    PROC_CALL(PromoMain_HandlePrepEndEffect),

    PROC_LABEL(7),
    PROC_LABEL(8),
    PROC_CALL(PromoMain_OnEnd),
    PROC_END,
};

bool StartAndWaitReclassSelect(struct ProcPromoMain * proc)
{
    struct ProcPromoMain * _proc = (struct ProcPromoMain *)proc;
    switch (_proc->stat)
    {
        case PROMO_MAIN_STAT_SELECTION:
            return false;

        case PROMO_MAIN_STAT_TRAINEE_EVENT:
        case PROMO_MAIN_STAT_INIT:
            proc->sel_en = StartReclassSelect(proc);
            _proc->stat = PROMO_MAIN_STAT_SELECTION;
            return false;

        default:
            return true;
    }
}

u8 ReclassSubConfirmMenuOnSelect(struct MenuProc * proc, struct MenuItemProc * b)
{
    if (b->itemNumber == 0)
    {
        ProcPtr found;
        EndMenu(proc);
        EndMenu(proc->proc_parent);
        found = Proc_Find(ProcScr_ReclassMain);

        EndAllProcChildren(found);
        ClassChgLoadEfxTerrain();
        Proc_Goto(found, PROMOMAIN_LABEL_POST_SEL);
    }
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6A;
}

u8 ReclassMenuSel_OnBPress(struct MenuProc * _proc, struct MenuItemProc * _proc2)
{
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6B;
}

const struct MenuItemDef MenuItem_ReclassSubConfirm[] = {
    { "　決定", 0x23, 0, TEXT_COLOR_SYSTEM_WHITE, 0, MenuAlwaysEnabled, NULL, ReclassSubConfirmMenuOnSelect, NULL, NULL, NULL},
    { "　やめる", 0x24, 0, TEXT_COLOR_SYSTEM_WHITE, 1, MenuAlwaysEnabled, NULL, ReclassSubConfirmMenuOnSelect, NULL, NULL, NULL },
    { 0 },
};

const struct MenuDef Menu_ReclassSubConfirm = { { 9, 4, 6, 0 },
                                                1,
                                                MenuItem_ReclassSubConfirm,
                                                ReclassSubConfirm_OnInit,
                                                ReclassSubConfirm_OnEnd,
                                                NULL,
                                                ReclassMenuSel_OnBPress,
                                                NULL,
                                                MenuStdHelpBox };

u8 ReclassMenuItem_OnSelect(struct MenuProc * pmenu, struct MenuItemProc * pmitem)
{
    struct ProcClassChgMenuSel * parent;
    struct ProcReclassSel * gparent;
    struct ProcPromoMain * ggparent;

    parent = pmenu->proc_parent;
    gparent = parent->proc_parent;
    ggparent = gparent->proc_parent;
    if (gparent->stat == 0)
    {
        struct Unit * unit = GetUnitFromCharId(ggparent->pid);
        u8 classnumber = unit->pClassData->number;
        int id = pmitem->itemNumber;
        if (id < NumberOfReclassOptions)
        {
            classnumber = GetReclassOption(ggparent->pid, classnumber, id);
        }
        ggparent->jid = classnumber;

        if (unit->state & US_IN_BALLISTA)
        {
            TryRemoveUnitFromBallista(unit);
        }

        InitTextFont(&gFontClassChgMenu, (void *)BG_VRAM + 0x1000, 0x80, 0x5);
        TileMap_FillRect(TILEMAP_LOCATED(gBG0TilemapBuffer, 9, 4), 0xA, 0x6, 0);
        BG_EnableSyncByMask(BG0_SYNC_BIT);
        StartMenuExt(&Menu_ReclassSubConfirm, 2, 0, 0, 0, pmenu);
    }

    return 0;
}

u8 ReclassMenuSelOnPressB(struct MenuProc * pmenu, struct MenuItemProc * pmitem)
{
    struct ProcClassChgMenuSel * parent;
    struct ProcReclassSel * gparent;
    struct ProcPromoMain * ggparent;
    struct ProcPromoHandler * gggparent;

    parent = pmenu->proc_parent;
    gparent = parent->proc_parent;
    ggparent = gparent->proc_parent;
    gggparent = ggparent->proc_parent;
    if (gggparent->bmtype == PROMO_HANDLER_TYPE_TRANINEE)
        return 0;

    if (gggparent->bmtype == PROMO_HANDLER_TYPE_BM)
    {
        Proc_End(parent);
        Proc_Goto(gparent, PROC_CLASSCHG_SEL_2);
        return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6B;
    }

    if (gggparent->bmtype == PROMO_HANDLER_TYPE_PREP)
    {
        Proc_End(parent);
        Proc_Goto(gparent, PROC_CLASSCHG_SEL_2);
        return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_SND6B;
    }

    return 0;
}

void ReclassMenuOnDrawCore(struct MenuProc * pmenu, struct MenuItemProc * pmitem, char * str)
{
    // u8 unused_stack[32];
    u16 * mapbuf;
    if (pmitem->def->color)
        Text_SetColor(&pmitem->text, pmitem->def->color);

    if (pmitem->availability == MENU_DISABLED)
        Text_SetColor(&pmitem->text, TEXT_COLOR_SYSTEM_GRAY);

    // ClearTextPart(&pmitem->text, 0, 20);
    ClearTextPart(&pmitem->text, 0, 12);
    Text_SetCursor(&pmitem->text, 8);
    Text_DrawString(&pmitem->text, str);
    mapbuf = BG_GetMapBuffer(pmenu->frontBg);

    PutText(&pmitem->text, &mapbuf[pmitem->yTile * 32 + pmitem->xTile]);
}

int ReclassMenuItem_OnTextDraw(struct MenuProc * pmenu, struct MenuItemProc * pmitem)
{
    // u8 unused_stack[0x48];
    struct ProcClassChgMenuSel * parent;
    struct ProcReclassSel * gparent;

    parent = pmenu->proc_parent;
    gparent = parent->proc_parent;
    ReclassMenuOnDrawCore(
        pmenu, pmitem, GetStringFromIndex(GetClassData(gparent->jid[pmitem->itemNumber])->nameTextId));
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_CLEAR | MENU_ACT_SND6A;
}

int ClassHasMagicRank(const struct ClassData * data)
{ // UnitHasMagicRank
    int combinedRanks = 0;

    combinedRanks |= data->baseRanks[ITYPE_STAFF];
    combinedRanks |= data->baseRanks[ITYPE_ANIMA];
    combinedRanks |= data->baseRanks[ITYPE_LIGHT];
    combinedRanks |= data->baseRanks[ITYPE_DARK];

    return combinedRanks ? TRUE : FALSE;
}

extern struct Font gDefaultFont;
static const char stats[][16] = {
    "HP", "Str", "Skl", "Spd", "Def", "Res", "Con", "Mov", "Mag",
};

void DrawStatDiff(int x, int y, int id, struct Unit * unit, const struct ClassData * classData)
{
    struct Text * th = gStatScreen.text;
    const struct ClassData * oldClass = unit->pClassData;
    int num = GetStatDiff(id, unit, oldClass, classData);
    // PutDrawText(&th[id], TILEMAP_LOCATED(gBG0TilemapBuffer, x, y), 0, 0, 2, stats[id]);
    if (ClassHasMagicRank(classData) && (id == 1) && (!IsStrMagInstalled()))
    {
        id += 7;
    }
    if (num >= 0)
    {
        PutDrawText(&th[id], TILEMAP_LOCATED(gBG0TilemapBuffer, x, y), 0, 0, 3, stats[id]); // "+"
        Text_InsertDrawString(&th[id], 18, th[id].colorId, "+");                            // th[id].x + 8
    }
    else
    {
        PutDrawText(&th[id], TILEMAP_LOCATED(gBG0TilemapBuffer, x, y), 0, 0, 3, stats[id]); // "-"
        Text_InsertDrawString(&th[id], 19, th[id].colorId, "-");
        th[id].x++;
    }
    th[id].x++;

    if (ABS(num) > 9)
    {
        Text_InsertDrawNumberOrBlank(&th[id], th[id].x + 8, th[id].colorId, ABS(num));
        // PutNumber(TILEMAP_LOCATED(gBG0TilemapBuffer, x+4, y), 0, ABS(num));
    }
    else
    {
        Text_InsertDrawNumberOrBlank(&th[id], th[id].x, th[id].colorId, ABS(num));
        // PutNumber(TILEMAP_LOCATED(gBG0TilemapBuffer, x+3, y), 0, ABS(num));
    }
}

struct SpecialCharSt
{
    s8 color;
    s8 id;
    s16 chr_position;
};
extern u16 Pal_SpinningArrow[];
extern struct Font * gActiveFont;
extern struct SpecialCharSt sSpecialCharStList[64];
void ReclassDrawStatChanges(struct Unit * unit, const struct ClassData * classData)
{
    struct Text * th = gStatScreen.text;
    InitTextFont(&gDefaultFont, (void *)(VRAM + 0x4400), 0x220, 0);
    sSpecialCharStList[0].color = -1; // redraw numbers !!
    for (int i = 0; i < 10; ++i)
    {
        InitText(&th[i], 6);
    }
    SetTextFontGlyphs(0);

    int x = 12;
    int y = 12;
    TileMap_FillRect(TILEMAP_LOCATED(gBG0TilemapBuffer, x, y), 8, 2, 0);
    BG_EnableSyncByMask(BG0_SYNC_BIT);

    int c = 0;
    LoadIconPalette(1, 3); // loaded already if non prep
    for (int i = 0; i < 8; ++i)
    {
        if (classData->baseRanks[i])
        {
            DrawIcon(gBG0TilemapBuffer + TILEMAP_INDEX(x + (c * 2), y), 0x70 + i, TILEREF(0, 3));
            c++;
        }
    }

    y = 4;

    DrawUiFrame(
        BG_GetMapBuffer(2),                  // back BG
        11, y - 1, 6, 10, TILEREF(0, 0), 0); // style as 0 ?
    DrawUiFrame(
        BG_GetMapBuffer(2),                  // back BG
        24, y - 1, 6, 10, TILEREF(0, 0), 0); // style as 0 ?
    BG_EnableSyncByMask(BG2_SYNC_BIT);
    SetBlendTargetA(0, 0, 1, 0, 0);

    DrawStatDiff(12, y + 0, 0, unit, classData);
    DrawStatDiff(25, y + 0, 1, unit, classData); // Str or Mag
    DrawStatDiff(12, y + 2, 2, unit, classData);
    DrawStatDiff(25, y + 2, 3, unit, classData);
    DrawStatDiff(12, y + 4, 4, unit, classData);
    DrawStatDiff(25, y + 4, 5, unit, classData);
    DrawStatDiff(12, y + 6, 6, unit, classData);
    if (IsStrMagInstalled())
    {
        DrawStatDiff(25, y + 6, 8, unit, classData); // mag
    }
    else
    {
        DrawStatDiff(25, y + 6, 7, unit, classData); // mov
    }

    // PutDrawText(&th[0], TILEMAP_LOCATED(gBG0TilemapBuffer, 13, 1), 0, 0, 5, "HP");
    // PutDrawText(&th[1], TILEMAP_LOCATED(gBG0TilemapBuffer, 8, 3), 0, 0, 5, "Str");
    // PutDrawText(&th[2], TILEMAP_LOCATED(gBG0TilemapBuffer, 8, 5), 0, 0, 5, "Skl");
    // PutDrawText(&th[3], TILEMAP_LOCATED(gBG0TilemapBuffer, 8, 7), 0, 0, 5, "Spd");
    // PutDrawText(&th[4], TILEMAP_LOCATED(gBG0TilemapBuffer, 8, 9), 0, 0, 5, "Def");
    // PutDrawText(&th[5], TILEMAP_LOCATED(gBG0TilemapBuffer, 8, 11), 0, 0, 5, "Res");
    // PutDrawText(&th[6], TILEMAP_LOCATED(gBG0TilemapBuffer, 8, 13), 0, 0, 5, "Lck");
    // PutDrawText(&th[7], TILEMAP_LOCATED(gBG0TilemapBuffer, 8, 15), 0, 0, 5, "Mag");

    // PutDrawText(struct Text* text, u16* dest, int colorId, int x, int tileWidth, const char* string);
}
extern int ClassDescEnabled;
int ReclassMenuItem_OnChange(struct MenuProc * pmenu, struct MenuItemProc * pmitem)
{
    struct ProcClassChgMenuSel * parent;
    struct ProcReclassSel * gparent;

    parent = pmenu->proc_parent;
    gparent = parent->proc_parent;
    gparent->stat = 1;
    gparent->main_select = pmitem->itemNumber;

    struct Unit * unit = GetUnitFromCharId(gparent->pid);
    const struct ClassData * classData =
        GetClassData(GetReclassOption(gparent->pid, unit->pClassData->number, pmenu->itemCurrent));
    ReclassDrawStatChanges(unit, classData);
    int msg_desc = classData->descTextId; // pmenu->itemCurrent
    // ChangeClassDescription(gparent->msg_desc[gparent->main_select]);
    if (ClassDescEnabled)
    {
        ChangeClassDescription(msg_desc);
    }
    SetTalkPrintDelay(-1);
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_CLEAR | MENU_ACT_SND6A;
}

u8 ReclassMenuItem_Usability(const struct MenuItemDef * _def, int _number)
{
    struct ProcClassChgMenuSel * proc = Proc_Find(ProcScr_ReclassMenuSel);
    struct ProcReclassSel * parent = proc->proc_parent;
    struct ProcPromoMain * gparent = parent->proc_parent;
    if (GetReclassOption(gparent->pid, GetUnitFromCharId(gparent->pid)->pClassData->number, _number))
    {
        return MENU_ENABLED;
    }
    return MENU_NOTSHOWN;
}

const struct MenuItemDef gMenuItem_ReclassSel[] = {
    { "　第１兵種", 0, 0x6DC, /* Discard items. Important[NL]items cannot be discarded. */
      TEXT_COLOR_SYSTEM_WHITE, 0, MenuAlwaysEnabled, ReclassMenuItem_OnTextDraw, ReclassMenuItem_OnSelect, 0,
      ReclassMenuItem_OnChange, 0 },
    { "　第３兵種", 0, 0x6DC, TEXT_COLOR_SYSTEM_WHITE, 1, ReclassMenuItem_Usability, ReclassMenuItem_OnTextDraw,
      ReclassMenuItem_OnSelect, 0, ReclassMenuItem_OnChange, 0 },
    { "　第３兵種", 0, 0x6DC, TEXT_COLOR_SYSTEM_WHITE, 2, ReclassMenuItem_Usability, ReclassMenuItem_OnTextDraw,
      ReclassMenuItem_OnSelect, 0, ReclassMenuItem_OnChange, 0 },
    { "　第３兵種", 0, 0x6DC, TEXT_COLOR_SYSTEM_WHITE, 3, ReclassMenuItem_Usability, ReclassMenuItem_OnTextDraw,
      ReclassMenuItem_OnSelect, 0, ReclassMenuItem_OnChange, 0 },
    { "　第３兵種", 0, 0x6DC, TEXT_COLOR_SYSTEM_WHITE, 4, ReclassMenuItem_Usability, ReclassMenuItem_OnTextDraw,
      ReclassMenuItem_OnSelect, 0, ReclassMenuItem_OnChange, 0 },
    { "　第３兵種", 0, 0x6DC, TEXT_COLOR_SYSTEM_WHITE, 5, ReclassMenuItem_Usability, ReclassMenuItem_OnTextDraw,
      ReclassMenuItem_OnSelect, 0, ReclassMenuItem_OnChange, 0 },
    { 0 }
};

void ReclassMenuSelOnInit(struct MenuProc * proc)
{
    SyncMenuBgs(proc);
}

void ReclassMenuSelOnEnd(struct MenuProc * proc)
{
    SyncMenuBgs(proc);
}

const struct MenuDef gMenuDef_ReclassSel = {
    .rect = { 16, 1, 8, 0 },
    .menuItems = gMenuItem_ReclassSel,
    .onInit = ReclassMenuSelOnInit,
    .onEnd = ReclassMenuSelOnEnd,
    .onBPress = ReclassMenuSelOnPressB,
};

const struct MenuRect ReclassMenuRect = { .x = 1,
                                          .y = 0,
                                          .w = 10, // InitText uses this for tile width
                                          .h = 0 };

extern void Make6C_PromotionMenuSelect(struct ProcReclassSel * proc);
extern void sub_80CCF60(struct ProcReclassSel * proc);
extern void sub_805AE14(void *);
// extern void LoadBattleSpritesForBranchScreen(struct ProcReclassSel *proc);
extern void sub_80CD294(struct ProcReclassSel * proc);
extern void sub_80CD1D4(struct ProcReclassSel * proc);
extern void sub_80CD2CC(struct ProcReclassSel * proc);
extern void NewCcramifyEnd(void);
extern void PrepClassChgOnCancel(struct ProcReclassSel * proc);

void ReclassMenuExec(struct ProcClassChgMenuSel * proc)
{
    proc->unk4C = 0;
    ResetTextFont();
    ResetText();
    SetTextFontGlyphs(0);
    // InitTextFont(&gFontClassChg, (void *)BG_VRAM + 0x1400, 0xA0, 5);
    InitTextFont(&gFontClassChg, (void *)BG_VRAM + 0x3400, 0x1A0, 5);
    // InitTextFont(&gFontClassChg, (void *)BG_VRAM + 0x1000, 160, 5);
    SetTextFont(&gFontClassChg);
    proc->pmenu = StartMenuCore(&gMenuDef_ReclassSel, ReclassMenuRect, 2, 0, 0, 0, (struct Proc *)proc);
}

extern int ClassNameTopEnabled;
void LoadClassNameInClassReelFont2(struct ProcReclassSel * proc)
{
    if (!ClassNameTopEnabled)
    {
        return;
    }
    s8 str[0x20];
    s32 index;
    u8 idx = proc->main_select;
    u16 classNum = proc->jid[idx];
    u32 xOffs = 0x74;
    const struct ClassData * class = GetClassData(classNum);
    GetStringFromIndexInBuffer(class->nameTextId, (void *)str);
    for (index = 0; index < 0x14 && str[index] != '\0'; index++)
    {
        struct ClassDisplayFont * font = GetClassDisplayFontInfo(str[index]);
        if (font)
        {
            if (font->a)
            { // font->yBase + y is where to draw on screen
                PutSpriteExt(4, xOffs - font->xBase - 2, font->yBase + 0, font->a, 0x81 << 7);
                xOffs += font->width - font->xBase;
            }
        }
        else
        {
            xOffs += 4;
        }
    }

    if (proc->u44 < 0xff)
        proc->u44++;
}

extern int BottomMenuBGEnabled; // behind class text description
void ReclassChgLoadUI(void)
{
    if (!BottomMenuBGEnabled)
    {
        return;
    }
    // asm("mov r11, r11");
    u8 * src = gUnknown_08A30800;
    u32 off = GetBackgroundTileDataOffset(BG_2); // 0x3800?
    // off = 0x800;
    Decompress(src, (void *)VRAM + 0x3000 + off);
    RegisterTsaWithOffset(
        gBG2TilemapBuffer, gUnknown_08A30978, TILEREF(0x180, 0) + 0x1000); // vanilla 0x1180, text rework 0x11c0 fsr
}

extern int PlatformYPos;
void Make6C_ReclassMenuSelect(struct ProcReclassSel * proc)
{
    struct ProcPromoMain * parent = proc->proc_parent;
    struct ProcPromoHandler * grandparent;
    struct Unit * unit;
    int i;
    int pid = 0;

    parent->stat = PROMO_MAIN_STAT_2;
    proc->pid = parent->pid;
    proc->u50 = 9;
    BG_Fill(gBG0TilemapBuffer, 0);
    BG_Fill(gBG1TilemapBuffer, 0);
    BG_Fill(gBG2TilemapBuffer, 0);
    LoadUiFrameGraphics();
    LoadObjUIGfx();
    sub_80CD47C(0, -1, 0xfb * 2, PlatformYPos, 6);
    ReclassChgLoadUI();
    // ClassChgLoadUI();
    sub_80CD408(proc->u50, 0x8c * 2, PlatformYPos + 0x10);

    proc->sprite[0] = 0;
    proc->sprite[1] = 0;
    proc->sprite[2] = 0;

    for (i = 1; i < 0x40; i++)
    {
        // u16 classFromSwitch;

        u16 weapon;
        s32 j;
        unit = GetUnit(i);

        if (!UNIT_IS_VALID(unit))
            continue;

        if (unit->pCharacterData->number != proc->pid)
            continue;

        pid = unit->pClassData->number;
        weapon = GetUnitEquippedWeapon(unit);

        int maybeWep = 0;

        // for (j = 0; j < 2; j++) {
        for (j = 0; j < NumberOfReclassOptions; j++)
        {
            int reclassID = GetReclassOption(proc->pid, pid, j);
            proc->jid[j] = reclassID;
            maybeWep = 0;
            if (CanClassEquipWeapon(weapon, reclassID))
            {
                maybeWep = weapon;
            }
            // proc->use_wpn[j] =

            // LoadClassBattleSprite((void*)&proc->sprite[j], ReclassTable[pid][j], weapon);
            if (j < 2)
            {
                proc->msg_desc[j] = GetClassData(reclassID)->descTextId; // i dunno
            }
            if (reclassID)
            {
                LoadClassBattleSprite((void *)&proc->sprite[j], reclassID, maybeWep);
            }
        }

        proc->weapon = weapon;
        break;
    }

    if (proc->sprite[0] == 0 && proc->sprite[1] == 0)
    {
        proc->sprite[1] = 0;
        proc->sprite[0] = 0;
    }

    proc->stat = 1;
    proc->main_select = 0;
    LoadClassReelFontPalette((void *)proc, pid);
    LoadClassNameInClassReelFont2(proc);
    LoadObjUIGfx();

    proc->menu_proc = ReclassMenuSelect(proc);

    grandparent = parent->proc_parent;
    if (grandparent->bmtype == PROMO_HANDLER_TYPE_BM)
    {
        RestartMuralBackground();
        BG_EnableSyncByMask(0xf);
    }
}

#define NONMATCHING
void LoadBattleSpritesForBranchScreen2(struct ProcReclassSel * proc)
{
    u32 a;
    u8 b;
    struct ProcReclassSel * p2;
    struct ProcReclassSel * c2;
    struct Anim * anim1;
    struct Anim * anim2;
    struct Unit copied_unit;
    void * tmp;
    u16 chara_pal;
    anim1 = gUnknown_030053A0.anim1;
    anim2 = gUnknown_030053A0.anim2;

    p2 = (void *)gUnknown_0201FADC.proc14;
    c2 = (void *)gUnknown_0201FADC.proc18;

    a = proc->stat;
    tmp = &gUnknown_030053A0;

    if (a == 1)
    {
        u16 _pid, _jid;
        s16 i;
        struct Unit * unit;
        const struct BattleAnimDef * battle_anim_ptr;
        u32 battle_anim_id;
        u16 ret;
        if ((s16)p2->sprite[0] <= 0x117)
        {
            p2->sprite[0] += 12;
            c2->sprite[0] += 12;
            anim1->xPosition += 12;
            anim2->xPosition = anim1->xPosition;
        }
        else
        {
            proc->stat = 2;
        }

        if (proc->stat == 2)
        {
            EndEfxAnimeDrvProc();
            sub_805AA28(&gUnknown_030053A0);
            _pid = proc->pid - 1;
            _jid = proc->jid[proc->main_select];
            chara_pal = -1;
            unit = GetUnitFromCharId(proc->pid);
            copied_unit = *unit;
            copied_unit.pClassData = GetClassData(proc->jid[proc->main_select]);
            battle_anim_ptr = copied_unit.pClassData->pBattleAnimDef;
            int weapon = GetUnitEquippedWeapon(&copied_unit);
            if (!CanClassEquipWeapon(weapon, proc->jid[proc->main_select]))
            {
                weapon = 0;
            }

            ret = GetBattleAnimationId(&copied_unit, battle_anim_ptr, (u16)weapon, &battle_anim_id);
            for (i = 0; i <= 6; i++)
            {
                if (gAnimCharaPalConfig[(s16)_pid][i] == (s16)_jid)
                {
                    chara_pal = gAnimCharaPalIt[(s16)_pid][i] - 1;
                    break;
                }
            }
            sub_80CD47C((s16)ret, (s16)chara_pal, (s16)(p2->sprite[0] + 0x28), PlatformYPos, 6);
            sub_805AE14(&gUnknown_0201FADC);
            sub_80CD408(proc->u50, p2->sprite[0], p2->msg_desc[1]); // I dunno
            // sub_80CD408(proc->u50, 0x8c * 2, PlatformYPos+0x10);
        }
        else
        {
            goto D1AC;
        }
    }
    ++proc;
    --proc;
    b = proc->stat;
    tmp = &gUnknown_030053A0;
    if (b == 2)
    {
        if ((s16)p2->sprite[0] > 0x82)
        {
#ifdef NONMATCHING
            u16 off = 12;
#else
            register u16 off asm("r1") = 12;
#endif // NONMATCHING
            p2->sprite[0] -= off;
            c2->sprite[0] -= off;
            anim1->xPosition -= off;
            anim2->xPosition = anim1->xPosition;
        }
        else
        {
            proc->stat = 0;
        }
    }
D1AC:
    if ((u8)sub_805A96C(tmp))
    {
        sub_805A990(tmp);
    }
    LoadClassNameInClassReelFont2((void *)proc);
    return;
}

void Reclasssub_80CCF60(struct ProcReclassSel * proc)
{
    u16 tmp;

    ResetTextFont();
    ResetText();
    BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT | BG2_SYNC_BIT | BG3_SYNC_BIT);
    InitTalk(0x100, 2, 0);
    if (ClassDescEnabled)
    {

        ChangeClassDescription(proc->msg_desc[proc->main_select]);
    }

    SetTalkPrintDelay(-1);

    gLCDControlBuffer.bg0cnt.priority = 0;
    gLCDControlBuffer.bg1cnt.priority = 2;
    gLCDControlBuffer.bg2cnt.priority = 1;
    gLCDControlBuffer.bg3cnt.priority = 3;

    BG_EnableSyncByMask(BG0_SYNC_BIT | BG1_SYNC_BIT | BG2_SYNC_BIT | BG3_SYNC_BIT);

    tmp = REG_BG0CNT;
    tmp &= 0xFFFC;
    REG_BG0CNT = tmp + 1;
    tmp = REG_BG1CNT;
    tmp &= 0xFFFC;
    REG_BG1CNT = tmp + 1;
    tmp = REG_BG2CNT;
    tmp &= 0xFFFC;
    REG_BG2CNT = tmp + 1;
    tmp = REG_BG3CNT;
    tmp &= 0xFFFC;
    REG_BG3CNT = tmp + 1;
}

const struct ProcCmd ProcScr_ReclassSelect[] = {
    PROC_CALL(StartMidFadeToBlack),
    PROC_REPEAT(WaitForFade),

    PROC_NAME("Reclass Select"),

    PROC_LABEL(PROC_CLASSCHG_SEL_INIT),
    PROC_CALL(Make6C_ReclassMenuSelect),
    PROC_SLEEP(6),
    PROC_CALL(Reclasssub_80CCF60),

    PROC_LABEL(PROC_CLASSCHG_SEL_1),
    PROC_CALL(StartMidFadeFromBlack),
    PROC_REPEAT(WaitForFade),
    PROC_REPEAT(LoadBattleSpritesForBranchScreen2),
    PROC_GOTO(PROC_CLASSCHG_SEL_END1),

    /* Pre End */
    PROC_LABEL(PROC_CLASSCHG_SEL_2),
    PROC_CALL(sub_80CD294),
    PROC_CALL(StartMidFadeToBlack),
    PROC_REPEAT(WaitForFade),

    /* On End */
    PROC_LABEL(PROC_CLASSCHG_SEL_4),
    PROC_CALL(sub_80CD1D4),
    PROC_CALL(sub_80CD2CC),
    PROC_SET_END_CB(NewCcramifyEnd),
    PROC_CALL(StartMidFadeToBlack),
    PROC_REPEAT(WaitForFade),
    PROC_CALL(PrepClassChgOnCancel),

    PROC_LABEL(PROC_CLASSCHG_SEL_END2),
    PROC_LABEL(PROC_CLASSCHG_SEL_END1),
    PROC_END,
};

ProcPtr StartReclassSelect(ProcPtr parent)
{
    return Proc_StartBlocking(ProcScr_ReclassSelect, parent);
}

ProcPtr ReclassMenuSelect(ProcPtr parent)
{
    return Proc_Start(ProcScr_ReclassMenuSel, parent);
}

struct ProcPromoMain * Make6C_ReclassMain(ProcPtr proc)
{
    return Proc_StartBlocking(ProcScr_ReclassMain, proc);
}

void MakeReclassScreen(struct ProcPromoHandler * proc, u8 pid, u8 terrain);
u32 ReclassHandler_SetupAndStartUI(struct ProcPromoHandler * proc)
{
    Proc_Start(ProcScr_SetLevel, (void *)3);
    struct Unit * unit;
    u8 classNumber;
    u32 terrain = TERRAIN_PLAINS;

    switch (gPlaySt.chapterModeIndex)
    {
        case CHAPTER_MODE_EIRIKA:
        default:
            terrain = TERRAIN_PLAINS;
            break;

        case CHAPTER_MODE_EPHRAIM:
            // terrain = TERRAIN_DESERT;
            break;
    }
    unit = GetUnitFromCharId(proc->pid);
    classNumber = unit->pClassData->number;
    int reclassID_A = GetReclassOption(proc->pid, classNumber, 0);
    int reclassID_B = GetReclassOption(proc->pid, classNumber, 1);
    // if no options, end
    // if 1 option, reclass into that without the menu?
    // if >1 option, show menu

    if (proc->bmtype == PROMO_HANDLER_TYPE_BM)
    {
        proc->bmtype = PROMO_HANDLER_TYPE_BM;
        proc->sel_en = 1;

        /* If no class to reclass into, end the handler proc */
        if (!reclassID_A && !reclassID_B)
            return PROMO_HANDLER_STAT_END;

        // if (reclassID_A && !reclassID_B) {
        // proc->jid = reclassID_A;
        // proc->sel_en = 0;
        // }

        // if (!reclassID_A && reclassID_B) {
        // proc->jid = reclassID_B;
        // proc->sel_en = 0;
        // }

        MakeReclassScreen(proc, proc->pid, terrain);
        return PROMO_HANDLER_STAT_IDLE;
    }
    else if (proc->bmtype == PROMO_HANDLER_TYPE_PREP)
    {
        proc->bmtype = PROMO_HANDLER_TYPE_PREP;
        proc->sel_en = 1;

        if (!reclassID_A && !reclassID_B)
        {
            BMapDispResume();
            RefreshBMapGraphics();
            return PROMO_HANDLER_STAT_END;
        }
        // if (reclassID_A && !reclassID_B) {
        // proc->jid = reclassID_A;
        // proc->sel_en = 0;
        // }
        // if (!reclassID_A && reclassID_B) {
        // proc->jid = reclassID_B;
        // proc->sel_en = 0;
        // }
        MakeReclassScreen(proc, proc->pid, terrain);
        return PROMO_HANDLER_STAT_IDLE;
    }
    else
        return PROMO_HANDLER_STAT_END;
}

void ReclassHandlerIdle(struct ProcPromoHandler * proc)
{
    switch (proc->stat)
    {
        case PROMO_HANDLER_STAT_IDLE:
        default:
            return;

        case PROMO_HANDLER_STAT_INIT:
            proc->stat = ReclassHandler_SetupAndStartUI(proc);
            break;

        case PROMO_HANDLER_STAT_END:
            Proc_Break(proc);
            break;
    }
}

void MakeReclassScreen(struct ProcPromoHandler * proc, u8 pid, u8 terrain)
{
    struct ProcPromoMain * child;

    /* set callback stat */
    proc->stat = PROMO_HANDLER_STAT_INIT;

    child = Make6C_ReclassMain(proc);
    proc->promo_main = child;
    child->pid = pid;
    child->terrain = terrain;
}

void StartPrepScreenReclass(struct ProcPrepItemUse * proc);
s8 CanUnitReclass(struct Unit * unit)
{
    return GetReclassOption(unit->pCharacterData->number, unit->pClassData->number, 0);
}
/*
const struct ProcCmd ProcScr_PrepItemUseJunaFruit[] = {
    //PROC_SET_END_CB(PrepItemUseJuna_OnEnd),
    //PROC_CALL(PrepItemUseJuna_OnInit),
    //PROC_REPEAT(PrepItemUseJuna_IDLE),
    //PROC_CALL(EndManimLevelUpStatGainLabels),
    //PROC_SLEEP(0x1),
    PROC_CALL_ARG(NewFadeOut, 0x10),
    PROC_WHILE(FadeOutExists),
    PROC_CALL(StartMidFadeToBlack),
    PROC_REPEAT(WaitForFade),
    PROC_CALL(StartPrepScreenReclass),
    PROC_SLEEP(0x8),
    PROC_CALL(PrepItemUse_ResetBgmAfterPromo),
    PROC_SLEEP(0x1E),
    PROC_CALL(PrepItemUse_PostPromotion),

    PROC_CALL(PrepItemUse_InitDisplay),
    PROC_CALL_ARG(NewFadeIn, 0x10),
    PROC_WHILE(FadeInExists),
    PROC_WHILE(MusicProc4Exists),
    PROC_END,
};
*/

void CallPrepItemUse_InitDisplay(struct ProcPrepItemUse * proc)
{
    PrepItemUse_InitDisplay(proc->proc_parent);
}
void CallPrepItemUse_PostPromotion(struct ProcPrepItemUse * proc)
{
    PrepItemUse_PostPromotion(proc->proc_parent);
    PrepItemUse_PostPromotion(proc);
}

// asmc or whatever
extern struct ProcCmd sProc_Menu[];
int StartBmReclass(ProcPtr proc)
{
    struct Unit * unit = gActiveUnit;
    gActiveUnit = unit;
    gActiveUnitId = unit->index;

    gActiveUnitMoveOrigin.x = unit->xPos;
    gActiveUnitMoveOrigin.y = unit->yPos;
    gActionData.xMove = unit->xPos;
    gActionData.yMove = unit->yPos;

    gActionData.subjectIndex = unit->index;
    // deplete the used item when reclassing if it's being used via IER or juna fruit etc
    if (gActionData.unitActionType != 0x1A)
    {
        gActionData.itemSlotIndex = -1; // don't deplete the item if being called in a different way
    }
    gActionData.unitActionType = 0;
    gActionData.moveCount = 0;

    gBmSt.taken_action = 0;
    gBmSt.unk3F = 0xFF;

    sub_802C334();

    gActiveUnit->xPos = gActionData.xMove;
    gActiveUnit->yPos = gActionData.yMove;
    UnitFinalizeMovement(gActiveUnit);
    ResetTextFont();
    gBattleActor.weaponBefore = gBattleTarget.weaponBefore =
        GetUnit(gActionData.subjectIndex)->items[gActionData.itemSlotIndex];

    int weapon = GetUnitEquippedWeapon(GetUnit(gActionData.subjectIndex));
    if (!CanClassEquipWeapon(weapon, gActiveUnit->pClassData->number))
    {
        weapon = 0;
    }
    gBattleActor.weapon = gBattleTarget.weapon = weapon;

    gBattleTarget.statusOut = -1;
    struct ProcPromoHandler * new_proc;
    struct MenuProc * menu = Proc_Find(sProc_Menu);
    // asm("mov r11, r11");
    if (menu)
    { // if a menu is active, don't block it. Instead, end it
        // EndAllMenus();
        new_proc = Proc_Start(ProcScr_ReclassHandler, (void *)3);
        //        new_proc = Proc_StartBlocking(ProcScr_ReclassHandler, proc);
    }
    else
    {
        new_proc = Proc_StartBlocking(ProcScr_ReclassHandler, proc);
    }
    new_proc->bmtype = PROMO_HANDLER_TYPE_PREP;
    new_proc->u32 = 0;
    unit = GetUnit(gActionData.subjectIndex);
    new_proc->pid = unit->pCharacterData->number;
    new_proc->unit = GetUnit(gActionData.subjectIndex);
    new_proc->item_slot = gActionData.itemSlotIndex;
    BMapDispSuspend();
    EndAllMus();
    return MENU_ACT_SKIPCURSOR | MENU_ACT_END | MENU_ACT_CLEAR | MENU_ACT_SND6A;
}

void StartPrepScreenReclass(struct ProcPrepItemUse * proc)
{
    struct ProcPrepItemUse * aParent = proc->proc_parent;
    proc->unit = aParent->unit;
    proc->slot = aParent->slot;
    proc->unk34 = aParent->unk34;
    proc->slot_rtext = aParent->slot_rtext;
    proc->pos_subbox = aParent->pos_subbox;
    // proc->game_lock = aParent->game_lock;

    gActiveUnit = proc->unit;
    // struct BattleUnit *actor, *target;
    struct ProcPromoHandler * new_proc;
    struct ProcPrepItemUse * parent;

    // u32 weapon;
    // u32 slot = proc->slot;
    // if (slot != -1) {
    //     struct BattleUnit *actor, *target;
    //     actor = &gBattleActor;
    //     target = &gBattleTarget;
    //     target->weaponBefore = proc->unit->items[slot];
    //     actor->weaponBefore = proc->unit->items[slot];
    // }
    // ApplyPalette(Pal_SpinningArrow, 0x3);
    gBattleActor.weaponBefore = gBattleTarget.weaponBefore = proc->unit->items[gActionData.itemSlotIndex];

    int weapon = GetUnitEquippedWeapon(proc->unit);
    if (!CanClassEquipWeapon(weapon, proc->unit->pClassData->number))
    {
        weapon = 0;
    }
    gBattleActor.weapon = gBattleTarget.weapon = weapon;

    gBattleTarget.statusOut = -1;

    new_proc = Proc_StartBlocking(ProcScr_ReclassHandler, proc);
    new_proc->bmtype = PROMO_HANDLER_TYPE_BM;
    new_proc->u32 = 0;

    parent = new_proc->proc_parent;
    new_proc->pid = parent->unit->pCharacterData->number;
    new_proc->unit = parent->unit;
    new_proc->item_slot = parent->slot;
}

// void StartBmPromotion(ProcPtr proc) { // test hook
// StartBmReclass(proc);
// }
