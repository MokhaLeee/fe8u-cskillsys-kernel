#include <common-chax.h>
#include <kernel-lib.h>

#include "kernel-lib-internal.h"

int GetChLenUtf8(const char *str)
{
	const u8 *utf8_in = (const u8 *)str;
	u8 cod = utf8_in[0];

	if ((0b11111000 & cod) == 0b11110000)
		return 4;

	if ((0b11110000 & cod) == 0b11100000)
		return 3;

	if ((0b11100000 & cod) == 0b11000000)
		return 2;

	if ((0b10000000 & cod) == 0x0)
		return 1;

	Errorf("Failed on decoding at %#X!", str);
	return -1;
}

int DecodeUtf8(const char *str, u32 *unicode_out, int *len)
{
	u32 unicod;
	const u8 *utf8_in = (const u8 *)str;

	switch (GetChLenUtf8(str)) {
	case 1:
		*unicode_out = *utf8_in;
		*len = 1;
		return 0;

	case 2:
		unicod = utf8_in[0] & 0b00011111;
		unicod = (unicod << 0x6) | (utf8_in[1] & 0b00111111);

		*unicode_out = unicod;
		*len = 2;
		return 0;

	case 3:
		unicod = utf8_in[0] & 0b00001111;
		unicod = (unicod << 0x6) | (utf8_in[1] & 0b00111111);
		unicod = (unicod << 0x6) | (utf8_in[2] & 0b00111111);

		*unicode_out = unicod;
		*len = 3;
		return 0;

	case 4:
		unicod = utf8_in[0] & 0b00000111;
		unicod = (unicod << 0x6) | (utf8_in[1] & 0b00111111);
		unicod = (unicod << 0x6) | (utf8_in[2] & 0b00111111);
		unicod = (unicod << 0x6) | (utf8_in[3] & 0b00111111);

		*unicode_out = unicod;
		*len = 4;
		return 0;

	default:

		Errorf("Failed on decoding at %#X!", str);

		*unicode_out = 0;
		*len = 0;
		return -1;
	}
}

/**
 * We don't care about overflow (if overflowed, it must be developer's fault)
 */
char *Utf8ToNarrowFonts(char *str)
{
	int i;
	char *buf_cur, *ret_cur, *ret, *buf = (void *)gGenericBuffer;

	if (!str)
		return NULL;

	strcpy(buf, str);

	buf_cur = buf;
	ret = ret_cur = str;

	while (*buf_cur != '\0') {
		int utf8_len = GetChLenUtf8(buf_cur);

		Assert(utf8_len > 0);

		if (utf8_len == 1) {
			/**
			 * Try narrow
			 */
			const char *narrow_str = AutoNarrowFontConf[(u8)*buf_cur].narrow_str;

			if (narrow_str) {
				while (*narrow_str != '\0')
					*ret_cur++ = *narrow_str++;

				buf_cur++;
				continue;
			}
		}

		/**
		 * UTF8 encoded
		 */
		for (i = 0; i < utf8_len; i++)
			*ret_cur++ = *buf_cur++;
	}

	*ret_cur = '\0';
	return ret;
}

char NarrowFontsUtf8ToAscii(const char *str)
{
	u32 i;

	for (i = 0; i < ARRAY_COUNT(AutoNarrowFontConf); i++) {
		const char *narrow_str = AutoNarrowFontConf[i].narrow_str;

		if (!narrow_str)
			continue;

		if (strcmp(str, narrow_str) == 0)
			return i;
	}
	return 0;
}

char NarrowFontsUnicodeToAscii(u32 unicod)
{
	u32 i;

	Printf("Try convert unicode: 0x%08X", unicod);

	for (i = 0; i < ARRAY_COUNT(AutoNarrowFontConf); i++) {
		u32 narrow_unicod = AutoNarrowFontConfToUnicode[i];

		if (narrow_unicod == 0)
			continue;

		if (unicod == narrow_unicod)
			return i;
	}
	return 0;
}

void DumpNarrowFontsUnicode(void)
{
#if 0
	u32 i;

	for (i = 0; i < ARRAY_COUNT(AutoNarrowFontConf); i++) {
		const char *narrow_str = AutoNarrowFontConf[i].narrow_str;
		int ret, len;
		u32 decoded;

		if (!narrow_str)
			continue;

		ret = DecodeUtf8(narrow_str, &decoded, &len);
		if (ret < 0)
			Errorf("[char=0x%02X %c] decode fail", i, i);

		LogPrintf("['%c'] = 0x%08X", i, decoded);
	}
#endif
}