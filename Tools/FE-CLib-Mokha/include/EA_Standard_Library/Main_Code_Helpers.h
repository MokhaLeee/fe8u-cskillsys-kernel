#pragma once

#include "constants/items.h"

#define TurnEventPlayer(eid, scr, turn) TURN(eid, scr, turn, 0, FACTION_BLUE)
#define TurnEventPlayer_(eid, scr, turn, dura) TURN(eid, scr, turn, turn + dura - 1, FACTION_BLUE)
#define TurnEventEnemy(eid, scr, turn) TURN(eid, scr, turn, 0, FACTION_RED)
#define TurnEventEnemy_(eid, scr, turn, dura) TURN(eid, scr, turn, turn + dura - 1, FACTION_RED)
#define TurnEventNPC(eid, scr, turn) TURN(eid, scr, turn, 0, FACTION_GREEN)
#define TurnEventNPC_(eid, scr, turn, dura) TURN(eid, scr, turn, turn + dura - 1, FACTION_GREEN)
#define Survive(scr, turn) TurnEventPlayer(0, scr, turn)
#define OpeningTurnEvent(scr) TurnEventPlayer(0, scr, 1)

#define CharacterEvent(eid, scr, pid1, pid2) CHAR((eid), (scr), (pid1), (pid2))
#define CharacterEvent_(eid, scr, pid1, pid2, trigg_eid) CHAR_((eid), (scr), (pid1), (pid2), (trigg_eid))
#define CharacterEventBothWays(eid, scr, pid1, pid2) CharacterEvent(eid, scr, pid1, pid2) CharacterEvent(eid, scr, pid2, pid1)

#define VILLAGE(eid, scr, x, y) \
    VILL(eid, scr, x, y, TILE_COMMAND_VISIT) \
    LOCA(eid, 1, x, y - 1, TILE_COMMAND_20)

#define HOUSE(eid, scr, x, y) LOCA(eid, scr, x, y, TILE_COMMAND_VISIT)
#define Seize_(eid, scr, x, y) LOCA(eid, scr, x, y, TILE_COMMAND_SEIZE)
#define SEIZE(x, y) Seize_(EVFLAG_WIN, EVENT_NOSCRIPT, x, y)

// #ifdef CONFIG_ESCAPE_EVENT
#define ESCAPE_(eid, scr, x, y) LOCA(eid, scr, x, y, TILE_COMMAND_MAP_ESCAPE)
#define ESCAPE(x, y) ESCAPE_(EVFLAG_WIN, EVENT_NOSCRIPT, x, y)
//#endif

#define CAUSE_GAME_OVER_IF_LORD_DIES AFEV(0, EventScr_GameOver, EVFLAG_GAMEOVER)
#define DEFEAT_BOSS(event_scr) AFEV(EVFLAG_WIN, (event_scr), EVFLAG_DEFEAT_BOSS)
#define DEFEAT_ALL(event_scr) AFEV(EVFLAG_WIN, (event_scr), EVFLAG_DEFEAT_ALL)
#define NOFADE EVBIT_T(EV_STATE_SKIPPING | EV_STATE_0002 | EV_STATE_ABORT)

#define ARMORY(list, x, y) EvtListShop(list, x, y, TILE_COMMAND_ARMORY)
#define VENDOR(list, x, y) EvtListShop(list, x, y, TILE_COMMAND_VENDOR)
#define SecretShop(list, x, y) EvtListShop(list, x, y, TILE_COMMAND_SECRET)

#define CHEST(item, x, y, flag) CHES(item, x, y, flag)
#define ChestMoney(amt, x, y, flag) CHES(ITEM_GOLD | (amt << 16), x, y, flag)
#define DOOR_2(x, y, flag)
#define DOOR_2_(x, y) DOOR(x, y, 0)

#define HouseEvent(msg, bg) \
    MUSI \
    TEXT_BG(bg, msg) \
    MUNO \
    NOFADE \
    ENDA

#define ConvoEvent(textID) \
    MUSI \
    TEXT(textID) \
    MUNO \
    NOFADE \
    ENDA
