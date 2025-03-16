#include "common-chax.h"
#include "types.h"
#include "eventinfo.h"
#include "constants/texts.h"

const struct BattleTalkExtEnt gNewBattleTalkList[] = {
    {
        .pidA    = CHARACTER_VALTER_PROLOGUE,
        .pidB    = CHARACTER_SETH,
        .chapter = 0x40,
        .msg     = MSG_BATTLE_QUOTE_VALTER_PROLOGUE,
    },
    {
        .pidA    = CHARACTER_EIRIKA,
        .pidB    = CHARACTER_ONEILL,
        .chapter = 0x00,
        .flag    = EVFLAG_BATTLE_QUOTES,
        .msg     = MSG_BATTLE_QUOTE_EIRIKA_ONEILL,
    },
    {
        .pidA    = CHARACTER_SETH,
        .pidB    = CHARACTER_ONEILL,
        .chapter = 0x00,
        .flag    = EVFLAG_BATTLE_QUOTES,
        .msg     = MSG_BATTLE_QUOTE_SETH_ONEILL,
    },
    {
        .pidA    = CHARACTER_BREGUET,
        .chapter = 0x01,
        .flag    = EVFLAG_BATTLE_QUOTES,
        .msg     = MSG_BATTLE_QUOTE_BREGUET,
    },
    {
        .pidA    = 0xffff,
        .chapter = 0x00,
        .msg     = 0x0000,
    },

};

//! FE8U = 0x0808464C
LYN_REPLACE_CHECK(GetBattleQuoteEntry);
struct BattleTalkExtEnt* GetBattleQuoteEntry(u16 pidA, u16 pidB) {
    const struct BattleTalkExtEnt* it;

    for (it = gNewBattleTalkList; it->pidA != 0xFFFF; it++) {

        if (it->chapter != 0xff && it->chapter != gPlaySt.chapterIndex) {
            if (it->chapter != 0xfe || BattleIsTriangleAttack() != 1) {
                continue;
            }
        }

        if (GetEventTriggerState(it->flag)) {
            continue;
        }

        if (it->pidA != 0) {

            if (it->pidB == 0) {
                if (pidA == it->pidA) {
                    return (struct BattleTalkExtEnt *)it;
                }
                continue;
            }

        } else {
            if (it->pidB == 0) {
                continue;
            }

            if (pidB == it->pidB) {
                return (struct BattleTalkExtEnt *)it;
            }

            continue;
        }

        if ((pidA == it->pidA) && (pidB == it->pidB)) {
            return (struct BattleTalkExtEnt *)it;
        }

        if ((pidB == it->pidA) && (pidA == it->pidB)) {
            return (struct BattleTalkExtEnt *)it;
        }
    }

    return NULL;
}

//! FE8U = 0x08083504
LYN_REPLACE_CHECK(CallBattleQuoteEventsIfAny);
void CallBattleQuoteEventsIfAny(u8 pidA, u8 pidB) {
    struct BattleTalkExtEnt* ent;

    if (GetBattleMapKind() == BATTLEMAP_KIND_SKIRMISH) {
        return;
    }

    if (gActionData.unitActionType != UNIT_ACTION_COMBAT) {
        return;
    }

    if ((ent = GetBattleQuoteEntry(pidA, pidB), ent != NULL) || (ent = GetBattleQuoteEntry(pidA, 0), ent != NULL) || (ent = GetBattleQuoteEntry(0, pidB), ent != NULL))
    {
        if (ent->msg) {
            CallBattleQuoteEventInBattle(ent->msg);
        } else {
            if (ent->event != 0) {
                EventEngine_CreateBattle((u16 *)ent->event);
            }
        }

        SetFlag(ent->flag);
    }

    return;
}