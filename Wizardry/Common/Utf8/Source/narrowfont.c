#include <common-chax.h>
#include <utf8.h>

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
			const char *narrow_str = gpAutoNarrowFontConf[(u8)*buf_cur].narrow_str;

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

	for (i = 0; i < 0x100; i++) {
		const char *narrow_str = gpAutoNarrowFontConf[i].narrow_str;

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

	for (i = 0; i < 0x100; i++) {
		u32 narrow_unicod = gpAutoNarrowFontConfToUnicode[i];

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

	for (i = 0; i < 0x100; i++) {
		const char *narrow_str = gpAutoNarrowFontConf[i].narrow_str;
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
