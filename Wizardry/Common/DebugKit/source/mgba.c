#include "global.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "mgba.h"

#define REG_DEBUG_ENABLE (vu16 *) 0x4FFF780
#define REG_DEBUG_FLAGS (vu16 *) 0x4FFF700
#define REG_DEBUG_STRING (char *) 0x4FFF600

void mgba_printf(int level, const char * ptr, ...)
{
	level &= 0x7;
	va_list args;
	va_start(args, ptr);
	vsprintf(REG_DEBUG_STRING, ptr, args);
	va_end(args);
	*REG_DEBUG_FLAGS = level | 0x100;
}

bool mgba_open(void)
{
	*REG_DEBUG_ENABLE = 0xC0DE;
	return *REG_DEBUG_ENABLE == 0x1DEA;
}

void mgba_close(void)
{
	*REG_DEBUG_ENABLE = 0;
}
