#ifndef GUARD_CONSTANTS_CHARACTERS_H
#define GUARD_CONSTANTS_CHARACTERS_H

enum {
    CHARACTER_NONE         = 0x00,

    // Player Characters
    CHARACTER_EIRIKA       = 0x01,
    CHARACTER_SETH         = 0x02,
    CHARACTER_GILLIAM      = 0x03,
    CHARACTER_FRANZ        = 0x04,
    CHARACTER_MOULDER      = 0x05,
    CHARACTER_VANESSA      = 0x06,
    CHARACTER_ROSS         = 0x07,
    CHARACTER_NEIMI        = 0x08,
    CHARACTER_COLM         = 0x09,
    CHARACTER_GARCIA       = 0x0A,
    CHARACTER_INNES        = 0x0B,
    CHARACTER_LUTE         = 0x0C,
    CHARACTER_NATASHA      = 0x0D,
    CHARACTER_CORMAG       = 0x0E,
    CHARACTER_EPHRAIM      = 0x0F,
    CHARACTER_FORDE        = 0x10,
    CHARACTER_KYLE         = 0x11,
    CHARACTER_AMELIA       = 0x12,
    CHARACTER_ARTUR        = 0x13,
    CHARACTER_GERIK        = 0x14,
    CHARACTER_TETHYS       = 0x15,
    CHARACTER_MARISA       = 0x16,
    CHARACTER_SALEH        = 0x17,
    CHARACTER_EWAN         = 0x18,
    CHARACTER_LARACHEL     = 0x19,
    CHARACTER_DOZLA        = 0x1A,
    CHARACTER_RENNAC       = 0x1C,
    CHARACTER_DUESSEL      = 0x1D,
    CHARACTER_MYRRH        = 0x1E,
    CHARACTER_KNOLL        = 0x1F,
    CHARACTER_JOSHUA       = 0x20,
    CHARACTER_SYRENE       = 0x21,
    CHARACTER_TANA         = 0x22,
    CHARACTER_LYON_CC      = 0x23,
    CHARACTER_ORSON_CC     = 0x24,
    CHARACTER_GLEN_CC      = 0x25,
    CHARACTER_SELENA_CC    = 0x26,
    CHARACTER_VALTER_CC    = 0x27,
    CHARACTER_RIEV_CC      = 0x28,
    CHARACTER_CAELLACH_CC  = 0x29,
    CHARACTER_FADO_CC      = 0x2A,
    CHARACTER_ISMAIRE_CC   = 0x2B,
    CHARACTER_HAYDEN_CC    = 0x2C,

    // Summoned Characters
    CHARACTER_SUMMON_LYON  = 0x3B,
    CHARACTER_SUMMON_KNOLL = 0x3E,
    CHARACTER_SUMMON_EWAN  = 0x3F,

    // Boss/Unique Enemy Characters
    CHARACTER_LYON         = 0x40,
    CHARACTER_MORVA        = 0x41,
    CHARACTER_ORSON_CH5X   = 0x42,
    CHARACTER_VALTER       = 0x43,
    CHARACTER_SELENA       = 0x44,
    CHARACTER_VALTER_PROLOGUE = 0x45,
    CHARACTER_BREGUET      = 0x46,
    CHARACTER_BONE         = 0x47,
    CHARACTER_BAZBA        = 0x48,
    CHARACTER_ENTOUMBED_CH4 = 0x49,
    CHARACTER_SAAR         = 0x4A,
    CHARACTER_NOVALA       = 0x4B,
    CHARACTER_MURRAY       = 0x4C,
    CHARACTER_TIRADO       = 0x4D,
    CHARACTER_BINKS        = 0x4E,
    CHARACTER_PABLO        = 0x4F,
    CHARACTER_MAELDUIN_CHUnk = 0x50, // TODO: which chapter?
    CHARACTER_AIAS         = 0x51,
    CHARACTER_CARLYLE      = 0x52,
    CHARACTER_CAELLACH     = 0x53,
    CHARACTER_PABLO_2      = 0x54, // TODO: which chapter?
    CHARACTER_GORGON_CHUnk = 0x56, // TODO: which chapter?
    CHARACTER_RIEV         = 0x57,
    CHARACTER_GHEB         = 0x5A,
    CHARACTER_BERAN        = 0x5B,
    CHARACTER_CYCLOPS_CHUnk = 0x5C,
    CHARACTER_WIGHT_CHUnk  = 0x5D,
    CHARACTER_DEATHGOYLE_CHUnk = 0x5E,
    CHARACTER_BANDIT_CH5   = 0x66,
    CHARACTER_ONEILL       = 0x68,
    CHARACTER_GLEN         = 0x69,
    CHARACTER_ZONTA        = 0x6A,
    CHARACTER_VIGARDE      = 0x6B,
    CHARACTER_LYON_FINAL   = 0x6C,
    CHARACTER_ORSON        = 0x6D,

    CHARACTER_SOLDIER_83   = 0x83,

    CHARACTER_MONSTER_BA   = 0xBA,
    CHARACTER_FOMORTIIS    = 0xBE,

    CHARACTER_FRELIAN      = 0xC0,
    CHARACTER_FADO         = 0xC5,

    CHARACTER_HAYDEN       = 0xC7,
    CHARACTER_MANSEL       = 0xC8,
    CHARACTER_KLIMT        = 0xC9,
    CHARACTER_DARA         = 0xCA,
    CHARACTER_ISMAIRE      = 0xCB,
    CHARACTER_MESSENGER    = 0xCC,

    CHARACTER_CITIZEN      = 0xFC,
    CHARACTER_ARENA_OPPONENT = 0xFD,
    CHARACTER_WALL         = 0xFE,
    CHARACTER_SNAG         = 0xFF,
};

enum event_autoload_pid_idx {
    CHARACTER_EVT_LEADER = 0,
    CHARACTER_EVT_ACTIVE = -1,
    CHARACTER_EVT_SLOTB = -2,
    CHARACTER_EVT_SLOT2 = -3,
};

#endif // GUARD_CONSTANTS_CHARACTERS_H
