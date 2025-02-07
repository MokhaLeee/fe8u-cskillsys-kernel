#pragma once

#include <common-chax.h>

int GetChLenUtf8(const char *str);
int DecodeUtf8(const char *str, u32 *unicode_out, int *len);
char *Utf8ToNarrowFonts(char *str);
char NarrowFontsUtf8ToAscii(const char *str);
char NarrowFontsUnicodeToAscii(u32 unicod);

/**
 * internal
 */
struct AutoNarrowFontEnt { const char *narrow_str; };

// extern const struct AutoNarrowFontEnt AutoNarrowFontConf[0x100];
// extern const u32 AutoNarrowFontConfToUnicode[0x100];

extern const struct AutoNarrowFontEnt *gpAutoNarrowFontConf;
extern const u32 *gpAutoNarrowFontConfToUnicode;
