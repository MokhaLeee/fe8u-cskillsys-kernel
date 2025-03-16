#include "common-chax.h"
#include "types.h"
#include "eventinfo.h"
#include "constants/texts.h"

/* Rewritten to be like the battle quotes, so multiple can be used */

const struct DefeatTalkEntNew gNewDefeatTalkList[] = {
    {
        .pidA    = CHARACTER_ONEILL,
        .pidB    = CHARACTER_EIRIKA,
        .route   = CHAPTER_MODE_ANY,
        .chapter = 0x00,
        .flag    = EVFLAG_DEFEAT_BOSS,
        .msg     = MSG_DEFEAT_QUOTE_EIRIKA_ONEILL,
    },
    {
        .pidA    = CHARACTER_ONEILL,
        .pidB    = CHARACTER_SETH,
        .route   = CHAPTER_MODE_ANY,
        .chapter = 0x00,
        .flag    = EVFLAG_DEFEAT_BOSS,
        .msg     = MSG_DEFEAT_QUOTE_SETH_ONEILL,
    },
    {
        .pidA    = CHARACTER_BREGUET,
        .route   = CHAPTER_MODE_ANY,
        .chapter = 0x01,
        .flag    = EVFLAG_DEFEAT_BOSS,
        .msg     = MSG_DEFEAT_QUOTE_BREGUET,
    },
    {
        .pidA     = 0xffff,
        .chapter = 0x00,
        .msg     = 0x0000,
    },
};

/* The print statment remains to prevent a heisenbug crash for now */
LYN_REPLACE_CHECK(GetDefeatTalkEntry);
struct DefeatTalkEntNew* GetDefeatTalkEntry(u16 pidA) {
    const struct DefeatTalkEntNew* it;

    for (it = gNewDefeatTalkList; it->pidA != 0xFFFF; it++) {
        NoCashGBAPrintf("PIDA is: %d", it->pidA);
        if (it->chapter != 0xff && it->chapter != gPlaySt.chapterIndex) {
            if (it->chapter != 0xfe || BattleIsTriangleAttack() != 1) {
                continue;
            }
        }

        if (GetEventTriggerState(it->flag)) {
            continue;
        }

        if ((pidA == it->pidA) && ((GetUnit(gBattleActor.unit.index)->pCharacterData->number == it->pidB) || GetUnit(gBattleTarget.unit.index)->pCharacterData->number == it->pidB)) {
            return (struct DefeatTalkEntNew *)it;
        }
    }

    return NULL;

    
}

//! FE8U = 0x080835A8
LYN_REPLACE_CHECK(CheckBattleDefeatTalk);
s8 CheckBattleDefeatTalk(u8 pid) {
    struct DefeatTalkEntNew* ent = GetDefeatTalkEntry(pid);

    if (ent) {
        if (ent->msg == 0 && ent->event == 0) {
            SetPidDefeatedFlag(pid, ent->flag);
        } else {
            return 1;
        }
    }
    return 0;
}

//! FE8U = 0x080835DC
LYN_REPLACE_CHECK(DisplayDefeatTalkForPid);
void DisplayDefeatTalkForPid(u8 pid) {
    struct DefeatTalkEntNew* ent = GetDefeatTalkEntry(pid);

    if (ent) {
        if ((ent->route == 1) && (ent->flag == 0x65)) {
            StartBgm(0x3e, NULL);
            gPlaySt.config.disableBgm = 1;
        } else {
            if (UNIT_FACTION(GetUnitFromCharId(pid)) == FACTION_BLUE) {
                StartBgm(0x3f, NULL);
            }
        }
        if (ent->msg != 0) {
            CallBattleQuoteEventInBattle(ent->msg);
        } else {
            if (ent->event) {
                EventEngine_CreateBattle((u16 *)ent->event);
            }
        }
        SetPidDefeatedFlag(pid, ent->flag);
    }
    return;
}