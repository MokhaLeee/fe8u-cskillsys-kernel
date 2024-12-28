#pragma once

#include <common-chax.h>

struct AutoNarrowFontEnt { const char *narrow_str; };

extern const struct AutoNarrowFontEnt AutoNarrowFontConf[0x100];
extern const u32 AutoNarrowFontConfToUnicode[0x100];