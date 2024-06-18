#pragma once

#include "common-chax.h"

enum char_identifier_event_expa {
    CHAR_EVT_UID_AT_SLOT2 = -4,

    CHAR_EVT_K_MAX = -5,
};

/* input: [slot-2]=uid */
extern const EventListScr EventScr_UidWarpIN[];
extern const EventListScr EventScr_UidWarpOUT[];
