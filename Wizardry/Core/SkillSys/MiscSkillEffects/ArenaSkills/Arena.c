#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

LYN_REPLACE_CHECK(ArenaGenerateBaseWeapons);
void ArenaGenerateBaseWeapons(void)
{
    u8 arenaWeapons[] = {
        [ITYPE_SWORD] = ITEM_SWORD_IRON,
        [ITYPE_LANCE] = ITEM_LANCE_IRON,
        [ITYPE_AXE] = ITEM_AXE_IRON,
        [ITYPE_BOW] = ITEM_BOW_IRON,
        [ITYPE_STAFF] = ITEM_NONE,
        [ITYPE_ANIMA] = ITEM_ANIMA_FIRE,
        [ITYPE_LIGHT] = ITEM_LIGHT_LIGHTNING,
        [ITYPE_DARK] = ITEM_DARK_FLUX};

    gArenaState.playerWeapon = MakeNewItem(arenaWeapons[gArenaState.playerWpnType]);

    gArenaState.opponentWeapon = MakeNewItem(arenaWeapons[gArenaState.opponentWpnType]);

    gArenaState.range = 1;

    if (gArenaState.playerWpnType == ITYPE_BOW)
    {
        gArenaState.range = 2;
    }

    if (gArenaState.opponentWpnType == ITYPE_BOW)
    {
        gArenaState.range = 2;
    }

    /*
    ** If the opponent is using a bow, the skill holder's equipped weapon must
    ** allow the opponent to counter in order for the skill to activate
    */
#if (defined(SID_ConcealedWeapon) && COMMON_SKILL_VALID(SID_ConcealedWeapon))
    if (SkillTester(gActiveUnit, SID_ConcealedWeapon))
    {
        if (GetUnitEquippedWeapon(gActiveUnit) != 0)
        {
            if ((GetItemMinRange(gArenaState.opponentWeapon) == GetItemMinRange(GetUnitEquippedWeapon(gActiveUnit))) ||
                (GetItemMaxRange(gArenaState.opponentWeapon) == GetItemMaxRange(GetUnitEquippedWeapon(gActiveUnit))))
            {
                gArenaState.playerWeapon = GetUnitEquippedWeapon(gActiveUnit);

                if (gArenaState.opponentWpnType != ITYPE_BOW && GetItemType(GetUnitEquippedWeapon(gActiveUnit)) != ITYPE_BOW)
                    gArenaState.range = 1;
            }
        }
    }
#endif

    return;
}

//! FE8U = 0x080B59CC
LYN_REPLACE_CHECK(ArenaUi_WagerGoldDialogue);
void ArenaUi_WagerGoldDialogue(ProcPtr proc)
{
    int multiplier = 1;

#if defined(SID_Ludopathy) && (COMMON_SKILL_VALID(SID_Ludopathy))
    if (SkillTester(gActiveUnit, SID_Ludopathy))
        multiplier *= 2;
#endif

    SetTalkNumber(ArenaGetMatchupGoldValue() * multiplier);
    StartArenaDialogue(0x8D2, proc);
    // TODO: msgid "Would you like to wager[.][NL][G] gold?[Yes]"
    return;
}

//! FE8U = 0x080B59EC
LYN_REPLACE_CHECK(ArenaUi_CheckConfirmation);
void ArenaUi_CheckConfirmation(ProcPtr proc)
{

    int multiplier = 1;

#if defined(SID_Ludopathy) && (COMMON_SKILL_VALID(SID_Ludopathy))
    if (SkillTester(gActiveUnit, SID_Ludopathy))
        multiplier *= 2;
#endif

    if (GetTalkChoiceResult() != 1)
    {
        StartArenaDialogue(0x8D4, proc);
        // TODO: msgid "What's that? Bah![.][NL]Get outta here![.][A]"
        Proc_Goto(proc, 2);
    }
    else
    {
        if (ArenaGetMatchupGoldValue() > (int)GetPartyGoldAmount() * multiplier)
        {
            StartArenaDialogue(0x8DA, proc);
            // TODO: msgid "You don't have the money![.][NL]Try again later.[A]"
            Proc_Goto(proc, 2);
        }
    }

    return;
}

//! FE8U = 0x080B5A38
LYN_REPLACE_CHECK(ArenaUi_ConfirmWager);
void ArenaUi_ConfirmWager(ProcPtr proc)
{
    int multiplier = 1;

#if defined(SID_Ludopathy) && (COMMON_SKILL_VALID(SID_Ludopathy))
    if (SkillTester(gActiveUnit, SID_Ludopathy))
        multiplier *= 2;
#endif

    SetPartyGoldAmount(GetPartyGoldAmount() - ArenaGetMatchupGoldValue() * multiplier);
    PlaySoundEffect(0xb9);
    DisplayGoldBoxText(TILEMAP_LOCATED(gBG0TilemapBuffer, 0x1B, 0x6));
    DrawArenaOpponentDetailsText(proc);

    return;
}