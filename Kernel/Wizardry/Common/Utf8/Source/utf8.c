#include <common-chax.h>
#include <utf8.h>

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
