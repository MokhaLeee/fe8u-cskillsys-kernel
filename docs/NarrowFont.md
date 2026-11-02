# Narrow font

Narrow font draws `A-Z`, `a-z` and the space with a set of slimmer glyphs, so that a
menu entry or a skill description can hold more text in the same width. It is used
all over C-SkillSys by default (see [auto_narrow_font](#6-auto_narrow_font)).

This doc covers how the glyphs are stored, how text is authored in a custom build
and in FEBuilder, and the two encodings that the kernel accepts.

## 1. What gets narrow

- Only `A-Z`, `a-z` and space. Digits and punctuation have **no** narrow form.
- `i l m w I M N T W` have no dedicated glyph: they are already narrow enough and
  are drawn with the normal one. `M` / `W` were deliberately left out by the
  original patch author because a shrunken `M` looks bad.
- Two separate sets are installed: the **menu font** (`item` in the font list,
  items / UI) and the **serif font** (`text`, dialogue boxes).

## 2. Native encoding: a real unicode character

Every narrow letter is stored as a genuine unicode character, for example `E` is
`ᴇ` (`U+1D07`). The table lives in
[`AutoNarrowFontConf`](../Kernel/Wizardry/Common/Utf8/Source/data.c):

```c
const struct AutoNarrowFontEnt AutoNarrowFontConf[0x100] = {
	['E'] = { "\xe1\xb4\x87\0" },   /* U+1D07 */
	...
};
```

The images live in [`Fonts/Glyph/NarrowFonts/`](../Fonts/Glyph/NarrowFonts) and are
named after the **UTF-8 bytes** of the character (`ᴇ` → `E1 B4 87` →
`FontItem_E1B487.png` / `FontText_E1B487.png`), which keeps the file names unique
and readable. `Fonts/FontList.txt` lists the font descriptor files and
[`glyph-installer-generator.py`](../Fonts/Scripts/glyph-installer-generator.py)
turns them into `Fonts/GlyphInstaller.event` at build time.

Rendering goes through
[`GetCharGlyphUnicode()`](../Kernel/Wizardry/Common/Utf8/Source/decoding.c): a
character is decoded to a code point, split into `hi` (high byte) and `lo` (low
byte), then `font->glyphs[lo]` is walked until `sjisByte1 == hi`.

## 3. The two encodings

The same glyph has two spellings in a ROM, plus one that is **not** supported:

| | bytes in ROM | code point | written by | supported |
|---|---|---|---|---|
| Native (this kernel) | `E1 B4 87` | `U+1D07` | buildfile text, `text2narrow.py` | yes |
| FEBuilder (DrawUTF8) | `C2 9F` | `U+009F` | FEBuilder `[_E]` | yes, through a compat table |
| FEBuilder (non UTF-8) | `9F` | — | FEBuilder `[_E]` without DrawUTF8 | **no** |

### Why FEBuilder writes `C2 9F`

The `NarrowFont` patch puts each narrow letter into a free slot of the vanilla
font table, and FEBuilder registers `[_E]` → `@00C2@009F` in the patch definition
(`config/patch2/FE8U/NarrowFont/PATCH_Narrowfont.txt`, the `TEXTESCAPE` rows).

`0x9F` is a **slot number**, not a character: it means "the 0x9F-th glyph". To put
that byte into UTF-8 text, FEBuilder encodes the *code point* `U+009F`, which in
UTF-8 is the two bytes `C2 9F` (`U+0080 ~ U+07FF` is always two bytes). So `C2` is
not a second character, it is the UTF-8 lead byte for the whole `U+0080 ~ U+00BF`
range.

Because C-SkillSys installs FEBuilder's DrawUTF8 signature (the fake patch at
`0x44D2` in [`Utf8.event`](../Kernel/Wizardry/Common/Utf8/Utf8.event)), FEBuilder
always picks the **UTF-8** variant, i.e. `C2 xx`.

### How the kernel accepts the FEBuilder form

[`gNarrowFontFebToUnicode[]`](../Kernel/Wizardry/Common/Utf8/Source/data.c) mirrors
the patch's `{UTF-8}` table as code points (`['E'] = 0x0000009F`). On a failed
glyph lookup, `GetCharGlyphUnicode()` calls
[`NarrowFontsFebToAscii()`](../Kernel/Wizardry/Common/Utf8/Source/narrowfont.c) to
turn `U+009F` back into the letter `E`, and then
`AsciiToNarrowFontsUnicode()` to reach `U+1D07`, so both spellings render
identically.

## 4. Writing text in a custom build

Text lives in `Contents/Texts/Source/*.txt` and is compiled by
[`textprocess-chax.py`](../Contents/Texts/Scripts/textprocess-chax.py) with the
`utf8` argument: everything that is not a control code is emitted as **literal
UTF-8 bytes**.

So the narrow characters are simply typed (or pasted) as they are:

```
## MSG_0C66
ᴘᵉᵍᵃˢᵘˢ ᴋⁿiᵍʰᵗ
```

Control codes keep their usual `[...]` form (`[X]`, `[NL]`, `[A]`, ...), see
`Contents/Texts/textdefs.txt`.

> [!WARNING]
> `[_E]` is a **FEBuilder** notation, not a buildfile control code. Unknown
> `[...]` tokens are silently dropped by the text processor, so a buildfile text
> containing `[_E]` renders as nothing at all. In a custom build, paste the
> character itself.

If you would rather not hunt for the characters, `Tools/scripts/text2narrow.py`
converts ASCII on stdin to the same code points:

```bash
echo "Pegasus Knight" | python3 Tools/scripts/text2narrow.py
```

## 5. Writing text in FEBuilder

1. Install the **NarrowFont** patch (`Patches` → search `NarrowFont`), then restart
   FEBuilder: the patch detection result is cached per process.
2. Type the text normally in the Text Editor.
3. Right click in the edit box → `NarrowFont変換`. Every letter is wrapped in
   `[_x]`, spaces become `[_SP]`, and existing `[...]` / `@00xx` escapes are left
   alone.
4. `Write` + `Write to ROM`.

The conversion is a toggle: if the text already contains `[_`, running it again
turns the macros back into plain letters.

> [!WARNING]
> Known defects in the shipped patch table, which the kernel cannot paper over:
>
> - `[_j]` is registered as bare `@0089` (no `C2` prefix), which is not valid
>   UTF-8 and renders as `?`. Type `j` directly instead.
> - `[_w]` is registered as `@0066` (`f`) in the `{UTF-8}` table — a typo. Type
>   `w` directly instead.
> - FEBuilder's ROM Translation Tools do not understand narrow text; keep them
>   away from it.

## 6. auto_narrow_font

The kernel can convert ASCII to narrow font at draw time, no macros involved:

- Config: `auto_narrow_font` (see [SystemConfig](./SystemConfig.md)), on by
  default when the build defines `CONFIG_AUTO_NARROW_FONT`
  ([configs.h](../include/configs/configs.h)).
- Scope: skill names and skill descriptions (`GetSkillNameStr` /
  `GetSkillDescStr`); the config entry also mentions menu items.
- API: [`Utf8ToNarrowFonts(char *str)`](../Kernel/Wizardry/Common/Utf8/Source/narrowfont.c)
  rewrites the string in place from `AutoNarrowFontConf`; it only touches single
  byte (ASCII) characters, so text that is already narrow passes through
  untouched.
- Spaces are **not** narrowed by this path (the space entry of
  `AutoNarrowFontConf` is compiled out), while `[_SP]` from FEBuilder does render
  as the 2 px short space.

## 7. Letter table

Lowercase:

| | | |
|---|---|---|
| a `ᵃ` U+1D43 | b `ᵇ` U+1D47 | c `ᶜ` U+1D9C |
| d `ᵈ` U+1D48 | e `ᵉ` U+1D49 | f `ᶠ` U+1DA0 |
| g `ᵍ` U+1D4D | h `ʰ` U+02B0 | k `ᵏ` U+1D4F |
| n `ⁿ` U+207F | o `ᵒ` U+1D52 | p `ᵖ` U+1D56 |
| q `ᵠ` U+1D60 | r `ʳ` U+02B3 | s `ˢ` U+02E2 |
| t `ᵗ` U+1D57 | u `ᵘ` U+1D58 | v `ᵛ` U+1D5B |
| x `ˣ` U+02E3 | y `ʸ` U+02B8 | z `ᶻ` U+1DBB |

Uppercase:

| | | |
|---|---|---|
| A `ᴀ` U+1D00 | B `ʙ` U+0299 | C `ᴄ` U+1D04 |
| D `ᴅ` U+1D05 | E `ᴇ` U+1D07 | F `ғ` U+0493 |
| G `ɢ` U+0262 | H `ʜ` U+029C | J `ᴊ` U+1D0A |
| K `ᴋ` U+1D0B | L `ʟ` U+029F | O `ᴏ` U+1D0F |
| P `ᴘ` U+1D18 | Q `ǫ` U+01EB | R `ʀ` U+0280 |
| S `Ⴝ` U+10BD | U `ᴜ` U+1D1C | V `ᴠ` U+1D20 |
| X `ჯ` U+10EF | Y `ʏ` U+028F | Z `ᴢ` U+1D22 |

Missing rows (`i l m w I M N T W`) mean "no dedicated glyph, use the normal
letter". Several of the carriers are visually similar characters from other scripts
(`Ⴝ` is Georgian, `ǫ` is Latin small letter o with ogonek); only the shape matters,
the code point carries no meaning.

## 8. Adding a glyph

1. Drop `FontItem_<UTF8 hex>.png` and `FontText_<UTF8 hex>.png` (16x16, 2 bpp —
   grit runs with `-gB2`) in `Fonts/Glyph/NarrowFonts/`.
2. Add one line per font to `Fonts/Glyph/NarrowFonts/font.fontall.txt`:

	```
	ᴇ	item	4	FontItem_E1B487.png
	ᴇ	text	5	FontText_E1B487.png
	```

	The third column is the glyph width in pixels and is used as-is (`M` / `W` are
	6, `ɪ` is 2), so it must match the drawn bitmap.
3. Map the ASCII letter to the character in `AutoNarrowFontConf`
   ([data.c](../Kernel/Wizardry/Common/Utf8/Source/data.c)); add the code point to
   `gNarrowFontFebToUnicode` too if FEBuilder should reach it.
4. Rebuild. `Fonts/makefile` regenerates `GlyphInstaller.event`, and the font
   tables are patched at the addresses listed in `FontTable` in the generator.

## 9. Limitations

- **Digits and punctuation have no narrow form.** An unfinished table for them
  exists in `data.c` behind `#if NARROW_FONT_NUMBER_INSTALLED`, but the glyph set
  is not enabled.
- **`U+0080 ~ U+00BF` is shared with vanilla glyphs.** `0x8C`, `0x91~0x94`,
  `0x9C`, `0xA1`, `0xAA~0xAB`, `0xB2~0xB3`, `0xB7`, `0xB9~0xBB` and `0xBF` already
  hold FE8 special characters, and the current lookup order (font table first)
  means `[_o]` renders the vanilla boxed-E glyph and `[_B]` the boxed-P one; in the
  serif font `[_Z]` renders `²`. Moving the FEBuilder fallback above the font
  table lookup resolves all three, at the price of shadowing those legacy glyphs
  (which are unreachable in a UTF-8 build anyway).
- **Non UTF-8 narrow text is not supported.** A bare `0x9F` byte is not valid
  UTF-8, the decoder rejects it, and the character renders as `?`. Only the
  `C2 xx` form is understood.
- The patch is FE8U only; the tables above are FE8U slot numbers.
