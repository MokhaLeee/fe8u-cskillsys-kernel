#include "common-chax.h"

#define REG_DEBUG_ENABLE ((vu16 *) 0x4FFF780)
#define REG_DEBUG_FLAGS ((vu16 *) 0x4FFF700)
#define REG_DEBUG_STRING ((char *) 0x4FFF600)

extern int mgba_print_level;

#if CONFIG_MGBA_PRINTF_EN

void mgba_print(const char *buf)
{
	strcpy(REG_DEBUG_STRING, buf);

	mgba_print_level &= 0x7;
	*REG_DEBUG_FLAGS = mgba_print_level | 0x100;
}

void mgba_printf(int level, const char *ptr, ...)
{
	va_list args;
	char buf[0x100];

	level &= 0x7;
	va_start(args, ptr);
	vsprintf(buf, ptr, args);
	va_end(args);

	mgba_print_level = level;
	mgba_print(buf);
}

bool mgba_open(void)
{
	mgba_print_level = MGBA_LOG_INFO;
	*REG_DEBUG_ENABLE = 0xC0DE;
	return *REG_DEBUG_ENABLE == 0x1DEA;
}

void mgba_close(void)
{
	*REG_DEBUG_ENABLE = 0;
}

#else

void mgba_print(const char *buf)
{
}

void mgba_printf(int level, const char *ptr, ...)
{
}

bool mgba_open(void)
{
	return true;
}

void mgba_close(void)
{
}

#endif
