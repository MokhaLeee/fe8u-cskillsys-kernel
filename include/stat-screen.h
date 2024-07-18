#pragma once

#include "common-chax.h"

#define STAT_BAR_MAX_INDENTIFIER 41

typedef const struct HelpBoxInfo _DECL_INFO;
extern _DECL_INFO * const RTextPageUnit, * const RTextPageSkill;
extern _DECL_INFO * const RTextPageSupport;

extern struct {
    u8 talkee;
    u8 _pad_[3];
} gStatScreenStExpa;
