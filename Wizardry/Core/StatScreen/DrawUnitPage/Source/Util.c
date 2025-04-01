#include "common-chax.h"
#include "stat-screen.h"
#include "strmag.h"
#include "skill-system.h"
#include "constants/skills.h"
#include "item-sys.h"

static const int dict_size = sizeof(dict_skills) / sizeof(dict_skills[0]);

int GetUnitBattleAmt(struct Unit * unit)
{
    int total = 0;

#ifdef CONFIG_TELLIUS_CAPACITY_SYSTEM
    struct SkillList *list;
    list = GetUnitSkillList(unit);
    int i;
    int value = -1;
    char * key;

    for (i = 0; i < list->amt; i++)
    {
        key = GetDuraItemName((list->sid[i] << 8) | CONFIG_ITEM_INDEX_SKILL_SCROLL);
        value = binary_search_skills(dict_skills, dict_size, key, 1);

        if (value == -1 ) 
            value = 0;

        total += value;
    }

#if defined(SID_CapacityHalf) && (COMMON_SKILL_VALID(SID_CapacityHalf))
    if (SkillTester(unit, SID_CapacityHalf))
            total = total / 2;
#endif

#if defined(SID_CapacityOne) && (COMMON_SKILL_VALID(SID_CapacityOne))
    if (SkillTester(unit, SID_CapacityOne))
            total = list->amt;
#endif

#else
    total += GetUnitPower(unit);
    total += GetUnitMagic(unit);
    total += GetUnitSkill(unit);
    total += GetUnitSpeed(unit);
    total += GetUnitLuck(unit);
    total += GetUnitDefense(unit);
    total += GetUnitResistance(unit);
#endif

    return total;
}

u8 SortMax(const u8 * buf, int size)
{
    int i, max = 0;
    for (i = 0; i < size; i++)
    {
        if (max < buf[i])
            max = buf[i];
    }

    return max;
}

u8 GetTalkee(struct Unit * unit)
{
    int i;
    const struct EventListCmdInfo * cmd_info = &gEventListCmdInfoTable[EVT_LIST_CMD_CHAR];
    const EventListScr * list = GetChapterEventDataPointer(gPlaySt.chapterIndex)->characterBasedEvents;

    /* Talk */
    for (;;)
    {
        u8 cmd = EVT_CMD_LO(list[0]);
        if (cmd == EVT_LIST_CMD_END)
            break;

        if (cmd != EVT_LIST_CMD_CHAR)
            continue;

        if (!CheckFlag(EVT_CMD_HI(list[0])))
        {
            const struct EvCheck03 * _chunk = (const void * )list;
            struct EventInfo info = {
                .listScript = list,
                .pidA = UNIT_CHAR_ID(unit),
                .pidB = _chunk->pidB,
            };

            if (cmd_info->func(&info) == true)
            {
                struct Unit * talkee = GetUnitFromCharId(_chunk->pidB);
                if (UNIT_ALIVE(talkee))
                    return _chunk->pidB;
            }
        }
        list += cmd_info->length;
    }

    /* Support */
    for (i = 0; i < GetUnitSupporterCount(unit); i++)
    {
        struct Unit * talkee = GetUnitSupporterUnit(unit, i);
        if (UNIT_ALIVE(talkee) && CanUnitSupportNow(unit, i))
            return UNIT_CHAR_ID(talkee);
    }
    return 0;
}

void InstallExpandedTextPal(void)
{
    extern const u16 ExpandedTextPals[];
    ApplyPalettes(ExpandedTextPals, 0x8, 2);
};

void ResetActiveFontPal(void)
{
    gActiveFont->tileref = gActiveFont->tileref & 0xFFF;
}

int GetTextColorFromGrowth(int growth)
{
    int _mod10 = growth / 10;
    LIMIT_AREA(_mod10, 0, 9);
    return (9 - _mod10) + 5;
}

void PutDrawTextRework(struct Text * text, u16 * tm, int color, int x, int tile_width, char const * str)
{
#ifdef CONFIG_GROWTHS_AS_LETTERS
    PutDrawText(text, tm, TEXT_COLOR_SYSTEM_GOLD, x, tile_width, str);
#else
    int bank;
    ModifyTextPal(bank, color);
    gActiveFont->tileref = TILEREF(gActiveFont->tileref & 0xFFF, bank);

    PutDrawText(text, tm, color, x, tile_width, str);
#endif
}

void DrawStatWithBarReworkExt(int num, int x, int y, u16 * tm, int base, int total, int max, int max_ref)
{
    int diff = total - base;

    LIMIT_AREA(base, 0, max);
    LIMIT_AREA(total, 0, max);

    base  = base * STAT_BAR_MAX_INDENTIFIER / max_ref;
    total = total * STAT_BAR_MAX_INDENTIFIER / max_ref;
    max   = max * STAT_BAR_MAX_INDENTIFIER / max_ref;

    diff = total - base;

    DrawStatBarGfx(
        0x401 + num * 6, 6,
        tm + TILEMAP_INDEX(x - 2, y + 1),
        TILEREF(0, STATSCREEN_BGPAL_6),
        max,
        base,
        diff);
}

void DrawStatWithBarRework(int num, int x, int y, u16 * tm1, u16 * tm2, int base, int total, int max)
{
    /**
     * Here the max value maybe more than 35,
     * but we need to fix the bar's length shorter than 35
     */

    // This shows the bonus numbers
    int diff = total - base;

    // Length of stat bar, higher values reduce the size?
    int max_bar = gStatScreenStExpa.unitpage_max;

    PutNumberOrBlank(
        tm1 + TILEMAP_INDEX(x, y),
        base == max
            ? TEXT_COLOR_SYSTEM_GREEN
            : TEXT_COLOR_SYSTEM_BLUE,
        base);

    PutNumberBonus(
        diff,
        tm1 + TILEMAP_INDEX(x + 1, y));

    DrawStatWithBarReworkExt(num, x, y, tm2, base, total, max, max_bar);
}

void HbPopuplate_Page1TrvTalk(struct HelpBoxProc * proc)
{
    if (gStatScreenStExpa.talkee != 0)
        proc->mid = 0x56A;
    else
        proc->mid = 0x550;
}

LYN_REPLACE_CHECK(DrawStatBar);
void DrawStatBar(
    int tile, int padding, int bufWidth, int barWidth, int progressLength, int cappedLength)
{
    //int i, j, val, val1; //k, val and val1 not used apparently?
    int i;
    u8 *buf = gGenericBuffer;
    int divisor = 1;
    CpuFastFill(0, buf, 0x40 * bufWidth);

#ifdef CONFIG_INSTALL_EXTENDED_STAT_BARS
    divisor = 2;
#endif

    for (i = 1; i < (barWidth + 1)/divisor; i++)
        DrawStatBarUnfilledCol(buf, 8 * bufWidth, i + ({padding + 1;}));

    DrawStatBarLeftBorder(buf, 8 * bufWidth, padding + 1);
    DrawStatBarRightBorder(buf, 8 * bufWidth, padding + (barWidth + 2)/divisor);
    DrawStatBarShadow(buf, 8 * bufWidth, padding + (barWidth + 3)/divisor);

    for (i = 0; i < progressLength/divisor; i++)
        DrawStatBarFilledCol(buf, 8 * bufWidth, i + ({padding + 2;}));

    for (i = 0; i < cappedLength/divisor; i++)
        DrawStatBarCappedCol(buf, 8 * bufWidth, i + progressLength/divisor + padding + 2);

    ApplyBitmap(buf, (void*)(32 * tile + 0x6000000), bufWidth, 1);
}

// bufWidth: The width of the allocated buffer canvas
// barWidth: The width of the bar itself (in tiles)
// progressLength: The length of the "progress" of the bar (the yellow part)
// cappedLength: Same as above, controls the part that flashes green when stat capped
LYN_REPLACE_CHECK(DrawStatBarGfx);
void DrawStatBarGfx(
    int tile, int bufWidth, u16* buf, int tileBase,
    int barWidth, int progressLength, int cappedLength)
{
    DrawStatBar(tile, 1, bufWidth, barWidth, progressLength, cappedLength);
    PutAppliedBitmap(buf, tileBase + (tile & 0x3FF), bufWidth, 1);
}