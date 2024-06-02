#include "common-chax.h"
#include "utf8.h"

static struct Glyph * GetCharGlyphUnicode(u32 unicode_ch, struct Font * font)
{
    struct Glyph * glyph;
    int hi = (unicode_ch >> 0x8) & 0xFF;
    int lo = unicode_ch & 0xFF;

    /* For now, we can only support for group 1 of unicode (U_0000 ~ U_FFFF) */
    if (unicode_ch >= 0x10000)
    {
        Errorf("Unicode %#x overflow!", unicode_ch);
        return NULL;
    }

    for(glyph = font->glyphs[lo]; glyph != NULL; glyph = glyph->sjisNext)
    {
        if (glyph->sjisByte1 == hi)
            return glyph;
    }

    LogErrorf("Failed to get glyph: %#x", unicode_ch);

    return NULL;
}

/* LynJump! */
const char * GetCharTextLen(const char * str, u32 * width)
{
    struct Glyph * glyph;
    u32 unicod;
    int ret, decode_len;

    ret = DecodeUtf8(str, &unicod, &decode_len);
    if (ret)
        return GetCharTextLen("?", width);

    glyph = GetCharGlyphUnicode(unicod, gActiveFont);
    if (glyph == NULL)
        return GetCharTextLen("?", width);

    *width = glyph->width;
    return str + decode_len;
}

/* LynJump! */
int GetStringTextLen(const char * str)
{
    u32 _wid;
    int width = 0;
    while (*str != 0 && *str != CHAR_NEWLINE)
    {
        str = GetCharTextLen(str, &_wid);
        width += _wid;
    }
    return width;
}

/* LynJump! */
const char * Text_DrawCharacter(struct Text * text, const char * str)
{
    struct Glyph * glyph;
    u32 unicod;
    int ret, decode_len;

    ret = DecodeUtf8(str, &unicod, &decode_len);
    if (ret)
    {
        unicod = '?';
        decode_len = 1;
    }

    glyph = GetCharGlyphUnicode(unicod, gActiveFont);
    if (glyph == NULL)
        glyph = GetCharGlyphUnicode('?', gActiveFont);

    gActiveFont->drawGlyph(text, glyph);
    return str + decode_len;
}

/* LynJump! */
void Text_DrawString(struct Text * text, const char * str)
{
    while (*str != 0 && *str != CHAR_NEWLINE)
        str = Text_DrawCharacter(text, str);
}
