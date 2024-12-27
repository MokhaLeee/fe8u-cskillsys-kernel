#include <common-chax.h>

extern char const *const *const prTextTable;
extern void (** _DecodeString)(const char *src, char *dst);

LYN_REPLACE_CHECK(GetStringFromIndexInBuffer);
char *GetStringFromIndexInBuffer(int index, char *buffer)
{
	const char *src = prTextTable[index];

	if (IS_ANTI_HUFFMAN(src))
		strcpy(buffer, (const void *)((uintptr_t)src & 0x0FFFFFFF));
	else
		(*_DecodeString)(src, buffer);

	SetMsgTerminator((void *)buffer);
	return buffer;
}

LYN_REPLACE_CHECK(GetStringFromIndex);
char *GetStringFromIndex(int index)
{
	char *dst = (char *)sMsgString.buffer1;

	if (index == sActiveMsg)
		return dst;

	sActiveMsg = index;
	return GetStringFromIndexInBuffer(index, dst);
}
