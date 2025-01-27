#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

LYN_REPLACE_CHECK(ArenaBeginInternal);
void ArenaBeginInternal(struct Unit* unit) {
    // int i;

    gArenaState.playerUnit = unit;
    gArenaState.opponentUnit = &gArenaOpponent;

    gUnknown_03003060 = UNIT_ARENA_LEVEL(unit);

    gArenaState.playerClassId = unit->pClassData->number;
    gArenaState.playerWpnType = GetUnitBestWRankType(unit);

    gArenaState.opponentClassId = ArenaGenerateOpposingClassId(gArenaState.playerWpnType);
    gArenaState.opponentWpnType = GetClassBestWRankType(GetClassData(gArenaState.opponentClassId));

    gArenaState.playerIsMagic = IsWeaponMagic(gArenaState.playerWpnType);
    gArenaState.opponentIsMagic = IsWeaponMagic(gArenaState.opponentWpnType);

    gArenaState.playerLevel = unit->level;

    if (UNIT_ARENA_LEVEL(unit) < 5) {
        gArenaState.opponentLevel = ArenaGetOpposingLevel(gArenaState.playerLevel);
    } else {
        gArenaState.opponentLevel = ArenaGetOpposingLevel(gArenaState.playerLevel) + 7;
    }

    ArenaGenerateOpponentUnit();
    ArenaGenerateBaseWeapons();

    // for (i = 0; i < 10; i++) {
    //     if (!ArenaAdjustOpponentPowerRanking()) {
    //         break;
    //     }
    // }

    // for (i = 0; i < 5; i++) {
    //     if (!ArenaAdjustOpponentDamage()) {
    //         break;
    //     }
    // }

    // gArenaState.playerPowerWeight = ArenaGetPowerRanking(gArenaState.playerUnit, gArenaState.opponentIsMagic);

    // gArenaState.opponentPowerWeight = ArenaGetPowerRanking(gArenaState.opponentUnit, gArenaState.playerIsMagic);

    // ArenaGenerateMatchupGoldValue();

    // gArenaState.unk0B = 1;

    // ArenaSetResult(0);

    // ArenaSetFallbackWeaponsMaybe();

    NoCashGBAPrintf("Enemy Level is: %d", gArenaState.opponentUnit->level);
    NoCashGBAPrintf("Enemy HP is: %d", gArenaState.opponentUnit->maxHP);
    NoCashGBAPrintf("Enemy STR is: %d", gArenaState.opponentUnit->pow);
    NoCashGBAPrintf("Enemy SKL is: %d", gArenaState.opponentUnit->skl);
    NoCashGBAPrintf("Enemy SPD is: %d", gArenaState.opponentUnit->spd);
    NoCashGBAPrintf("Enemy DEF is: %d", gArenaState.opponentUnit->def);
    NoCashGBAPrintf("Enemy RES is: %d", gArenaState.opponentUnit->res);
    NoCashGBAPrintf("Enemy LCK is: %d", gArenaState.opponentUnit->lck);

    NoCashGBAPrintf("Enemy STR is: %d", gArenaOpponent.pow);

    return;
};

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

LYN_REPLACE_CHECK(ArenaGetUpgradedWeapon);
u16 ArenaGetUpgradedWeapon(u16 item) {
    u8* iter;

    u8 arenaWeaponUpgrades[] = {
        ITEM_SWORD_IRON, ITEM_SWORD_STEEL, ITEM_SWORD_SILVER, 0,
        ITEM_LANCE_IRON, ITEM_LANCE_STEEL, ITEM_LANCE_SILVER, 0,
        ITEM_AXE_IRON, ITEM_AXE_STEEL, ITEM_AXE_SILVER, 0,
        ITEM_BOW_IRON, ITEM_BOW_STEEL, ITEM_BOW_SILVER, 0,
        ITEM_ANIMA_FIRE, ITEM_ANIMA_ELFIRE, ITEM_ANIMA_FIMBULVETR, 0,
        ITEM_LIGHT_LIGHTNING, ITEM_LIGHT_DIVINE, 0,
        ITEM_DARK_FLUX, 0,

        -1
    };

    for (iter = arenaWeaponUpgrades; *iter != (u8) -1; iter++) {
        if (GetItemIndex(item) != *iter) {
            continue;
        }

        if (*++iter != 0) {
            return MakeNewItem(*iter);
        }

        return item;
    }

    /* The vanilla function lacks a return type outside of the loop */
    /* This will never be reached */
    return item;
};

LYN_REPLACE_CHECK(ArenaAdjustOpponentDamage);
s8 ArenaAdjustOpponentDamage(void) {
    s8 result = 0;

    gBattleActor.battleAttack = GetUnitPower(gArenaState.playerUnit) + 5;

    if (gArenaState.opponentIsMagic) {
        gBattleActor.battleDefense = GetUnitResistance(gArenaState.playerUnit);
    } else {
        gBattleActor.battleDefense = GetUnitDefense(gArenaState.playerUnit);
    }

    gBattleTarget.battleAttack = GetUnitPower(gArenaState.opponentUnit) + 5;

    if (gArenaState.playerIsMagic) {
        gBattleTarget.battleDefense = GetUnitResistance(gArenaState.opponentUnit);
    } else {
        gBattleTarget.battleDefense = GetUnitDefense(gArenaState.opponentUnit);
    }

    if ((gBattleActor.battleAttack - gBattleTarget.battleDefense) < (GetUnitMaxHp(gArenaState.opponentUnit) / 6)) {
        result = 1;

        if (gArenaState.playerIsMagic) {
            gArenaState.opponentUnit->res -= 4;

            if (gArenaState.opponentUnit->res < 0) {
                gArenaState.opponentUnit->res = 0;
            }
        } else {
            gArenaState.opponentUnit->def -= 4;

            if (gArenaState.opponentUnit->def < 0) {
                gArenaState.opponentUnit->def = 0;
            }
        }

        gArenaState.opponentUnit->spd += 1;
        gArenaState.opponentUnit->skl += 1;
    }

    if (gBattleTarget.battleAttack - gBattleActor.battleDefense < (GetUnitMaxHp(gArenaState.playerUnit) / 6)) {
        result = 1;

        gArenaState.opponentUnit->pow += 3;
        gArenaState.opponentUnit->spd += 2;
        gArenaState.opponentUnit->skl += 2;

        gArenaState.opponentWeapon = ArenaGetUpgradedWeapon(gArenaState.opponentWeapon);
    }

    return result;
}

LYN_REPLACE_CHECK(ArenaAdjustOpponentPowerRanking);
s8 ArenaAdjustOpponentPowerRanking(void) {
    int max;
    int diff;

    gArenaState.playerPowerWeight = ArenaGetPowerRanking(gArenaState.playerUnit, gArenaState.opponentIsMagic);

    gArenaState.opponentPowerWeight = ArenaGetPowerRanking(gArenaState.opponentUnit, gArenaState.playerIsMagic);

    max = gArenaState.playerPowerWeight > gArenaState.opponentPowerWeight
        ? gArenaState.playerPowerWeight
        : gArenaState.opponentPowerWeight;

    diff = ABS(gArenaState.playerPowerWeight - gArenaState.opponentPowerWeight);

    if (((diff * 100) / max) <= 20) {
        return 0;
    }

    if (gArenaState.playerPowerWeight < gArenaState.opponentPowerWeight) {
        if (gArenaState.opponentUnit->maxHP != 0) {
            gArenaState.opponentUnit->maxHP -= 1;
            gArenaState.opponentUnit->curHP -= 1;
        }

        if (gArenaState.opponentUnit->pow != 0) {
            gArenaState.opponentUnit->pow -= 1;
        }

        if (gArenaState.opponentUnit->skl != 0) {
            gArenaState.opponentUnit->skl -= 1;
        }

        if (gArenaState.opponentUnit->spd != 0) {
            gArenaState.opponentUnit->spd -= 1;
        }

        if (gArenaState.opponentUnit->def != 0) {
            gArenaState.opponentUnit->def -= 1;
        }

        if (gArenaState.opponentUnit->res != 0) {
            gArenaState.opponentUnit->res -= 1;
        }

        if (gArenaState.opponentUnit->lck != 0) {
            gArenaState.opponentUnit->lck -= 1;
        }
    } else {
        if (gArenaState.opponentUnit->maxHP < 80) {
            gArenaState.opponentUnit->maxHP += 2;
            gArenaState.opponentUnit->curHP += 2;
        }

        if (gArenaState.opponentUnit->pow < 30) {
            gArenaState.opponentUnit->pow += 1;
        }

        if (gArenaState.opponentUnit->skl < 30) {
            gArenaState.opponentUnit->skl += 1;
        }

        if (gArenaState.opponentUnit->spd < 30) {
            gArenaState.opponentUnit->spd += 1;
        }

        if (gArenaState.opponentUnit->def < 30) {
            gArenaState.opponentUnit->def += 1;
        }

        if (gArenaState.opponentUnit->res < 30) {
            gArenaState.opponentUnit->res += 1;
        }

        if (gArenaState.opponentUnit->lck < 30) {
            gArenaState.opponentUnit->lck += 1;
        }
    }

    return 1;
}

//! FE8U = 0x080B59CC
LYN_REPLACE_CHECK(ArenaUi_WagerGoldDialogue);
void ArenaUi_WagerGoldDialogue(ProcPtr proc)
{
    int multiplier = 1;

#if defined(SID_Ludopathy) && (COMMON_SKILL_VALID(SID_Ludopathy))
    if (SkillTester(gActiveUnit, SID_Ludopathy))
        multiplier = 2;
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
        multiplier = 2;
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
        multiplier = 2;
#endif

    SetPartyGoldAmount(GetPartyGoldAmount() - ArenaGetMatchupGoldValue() * multiplier);
    PlaySoundEffect(0xb9);
    DisplayGoldBoxText(TILEMAP_LOCATED(gBG0TilemapBuffer, 0x1B, 0x6));
    DrawArenaOpponentDetailsText(proc);

    return;
};

LYN_REPLACE_CHECK(ArenaGetPowerRanking);
int ArenaGetPowerRanking(struct Unit* unit, s8 opponentIsMagic) {
    int result = unit->maxHP;

    result += unit->maxHP;
    result += unit->pow * 2;
    result += unit->skl * 2;
    result += unit->spd * 2;
    result += unit->lck;
    result += UNIT_CON_BASE(unit);

    result = 1;

    if (opponentIsMagic) {
        result += GetUnitResistance(unit) * 2;
    } else {
        result += GetUnitDefense(unit) * 2;
    }

    if (UNIT_CATTRIBUTES(unit) & CA_CRITBONUS) {
        result += GetUnitPower(unit);
    }

    return result;
}

LYN_REPLACE_CHECK(ArenaGenerateOpponentUnit);
void ArenaGenerateOpponentUnit(void) {
    int level;
    int i;

    struct UnitDefinition udef;

    struct Unit* unit = &gArenaOpponent;

    udef.charIndex = CHARACTER_ARENA_OPPONENT;
    udef.classIndex = gArenaState.opponentClassId;
    udef.allegiance = 0;
    udef.level = gArenaState.opponentLevel;
    udef.autolevel = 1;
    udef.items[0] = 0;
    udef.items[1] = 0;
    udef.items[2] = 0;
    udef.items[3] = 0;
    udef.ai[0] = 0;
    udef.ai[0] = 0;
    udef.ai[1] = 0;
    udef.ai[2] = 0;
    udef.ai[3] = 0;

    ClearUnit(&gArenaOpponent);
    unit->index = 0x80;

    UnitInitFromDefinition(unit, &udef);
    UnitLoadStatsFromCharacter(unit, unit->pCharacterData);

    NoCashGBAPrintf("First mention of opponent HP is: %d", unit->maxHP);
    NoCashGBAPrintf("First mention of opponent STR is: %d", unit->pow);
    NoCashGBAPrintf("First mention of opponent SKL is: %d", unit->skl);
    NoCashGBAPrintf("First mention of opponent SPD is: %d", unit->spd);
    NoCashGBAPrintf("First mention of opponent DEF is: %d", unit->def);
    NoCashGBAPrintf("First mention of opponent RES is: %d", unit->res);
    NoCashGBAPrintf("First mention of opponent LCK is: %d", unit->lck);

    level = unit->level;

    unit->level = ((gPlaySt.chapterStateBits & PLAY_FLAG_HARD) ? level * 24 : level * 12) / 10;

    UnitAutolevel(unit);

    NoCashGBAPrintf("Second mention of opponent HP is: %d", unit->maxHP);
    NoCashGBAPrintf("Second mention of opponent STR is: %d", unit->pow);
    NoCashGBAPrintf("Second mention of opponent SKL is: %d", unit->skl);
    NoCashGBAPrintf("Second mention of opponent SPD is: %d", unit->spd);
    NoCashGBAPrintf("Second mention of opponent DEF is: %d", unit->def);
    NoCashGBAPrintf("Second mention of opponent RES is: %d", unit->res);
    NoCashGBAPrintf("Second mention of opponent LCK is: %d", unit->lck);

    unit->level = level;

    for (i = 0; i < 8; i++) {
        if (unit->ranks[i] != 0) {
            unit->ranks[i] = -75;
        }
    }

    if (unit->level < 1) {
        unit->level = 1;
    }

    if (unit->level > 20) {
        unit->level = 20;
    }

    UnitCheckStatCaps(unit);
    SetUnitHp(unit, GetUnitMaxHp(unit));

    NoCashGBAPrintf("Third mention of opponent HP is: %d", unit->maxHP);

    return;
}
