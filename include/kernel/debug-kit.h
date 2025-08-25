#pragma once

#include <string.h>
#include <stdarg.h>

#include "mgba.h"
#include "no-cash-gba.h"

#ifndef __PRINTFLIKE
#define __PRINTFLIKE(__fmt,__varargs) __attribute__((__format__ (__printf__, __fmt, __varargs)))
#endif

/**
 * stdio
 */
struct io_handle;

typedef struct FILE {
	const struct io_handle *io;
} FILE;

struct io_handle {
	size_t (*write)(FILE *fp, const char *buf, size_t len);
	size_t (*read)(FILE *fp, char *buf, size_t len);
};

extern FILE __stdio_FILEs[3];

#define stdin  (&__stdio_FILEs[0])
#define stdout (&__stdio_FILEs[1])
#define stderr (&__stdio_FILEs[2])

void io_init(void);

/**
 * console
 */
int k_vsnprintf(char *str, size_t len, const char *fmt, va_list ap);
int k_vsprintf(char *str, const char *fmt, va_list ap);
int k_vfprintf(FILE *fp, const char *fmt, va_list ap);

int k_sprintf(char *str, const char *fmt, ...) __PRINTFLIKE(2, 3);
int k_snprintf(char *str, size_t len, const char *fmt, ...) __PRINTFLIKE(3, 4);
int k_fprintf(FILE *fp, const char *fmt, ...) __PRINTFLIKE(2, 3);
int k_printf(const char *fmt, ...) __PRINTFLIKE(1, 2);

/**
 * Always print to STDOUT regardless on debug config
 */
#define fmt_printf(format, ...) k_printf("(%s): "format, __func__, ##__VA_ARGS__)

/**
 * Old
 */
#define LogInit() {io_init(); }
#define LogPrint(string)        { REG_IME = 0; mgba_printf(MGBA_LOG_INFO, string);               NoCashGBAPrint(string); REG_IME = 1; }
#define LogPrintf(format, ...)  { REG_IME = 0; mgba_printf(MGBA_LOG_INFO, format, __VA_ARGS__);  NoCashGBAPrintf(format, __VA_ARGS__); REG_IME = 1;}
#define LogWarn(string)         { REG_IME = 0; mgba_printf(MGBA_LOG_WARN, string);               NoCashGBAPrint("[WARN] "string); REG_IME = 1;}
#define LogWarnf(format, ...)   { REG_IME = 0; mgba_printf(MGBA_LOG_WARN, format, __VA_ARGS__);  NoCashGBAPrintf("[WARN] "format, __VA_ARGS__); REG_IME = 1;}
#define LogInfo(string)         { REG_IME = 0; mgba_printf(MGBA_LOG_INFO, string);               NoCashGBAPrint("[INFO] "string); REG_IME = 1;}
#define LogInfof(format, ...)   { REG_IME = 0; mgba_printf(MGBA_LOG_INFO, format, __VA_ARGS__);  NoCashGBAPrintf("[INFO] "format, __VA_ARGS__); REG_IME = 1;}
#define LogDebug(string)        { REG_IME = 0; mgba_printf(MGBA_LOG_DEBUG, string);              NoCashGBAPrint("[DEBUG] "string); REG_IME = 1;}
#define LogDebugf(format, ...)  { REG_IME = 0; mgba_printf(MGBA_LOG_DEBUG, format, __VA_ARGS__); NoCashGBAPrintf("[DEBUG] "format, __VA_ARGS__); REG_IME = 1;}
#define LogFatal(string)        { REG_IME = 0; mgba_printf(MGBA_LOG_FATAL, string);              NoCashGBAPrint("[FATAL] "string); REG_IME = 1; abort(); }
#define LogFatalf(format, ...)  { REG_IME = 0; mgba_printf(MGBA_LOG_FATAL, format, __VA_ARGS__); NoCashGBAPrintf("[FATAL] "format, __VA_ARGS__); REG_IME = 1; abort(); };
#define LogError(string)        { REG_IME = 0; mgba_printf(MGBA_LOG_ERROR, string);              NoCashGBAPrint("[ERROR] "string); REG_IME = 1;}
#define LogErrorf(format, ...)  { REG_IME = 0; mgba_printf(MGBA_LOG_ERROR, format, __VA_ARGS__); NoCashGBAPrintf("[ERROR] "format, __VA_ARGS__); REG_IME = 1;}

#if (CONFIG_FORCE_PRIENT_ERROR || defined(CONFIG_USE_DEBUG))
	#define Fatal(string)       LogFatalf("(%s): %s", __func__, string)
	#define Fatalf(format, ...) LogFatalf("(%s): "format, __func__, __VA_ARGS__)
	#define Error(string)       LogErrorf("(%s): %s", __func__, string)
	#define Errorf(format, ...) LogErrorf("(%s): "format, __func__, __VA_ARGS__)
	#define Assert(condition)   do { if (!(condition)) { Fatal("Assertion failed: " #condition); }} while (0)
#else
	#define Fatal(string)
	#define Fatalf(format, ...)
	#define Error(string)
	#define Errorf(format, ...)
	#define Assert(condition)
#endif

#ifdef CONFIG_USE_DEBUG
	#define Print(string)       LogPrintf("(%s): %s", __func__, string)
	#define Printf(format, ...) LogPrintf("(%s): "format, __func__, __VA_ARGS__)
	#define Warn(string)        LogWarnf("(%s): %s", __func__, string)
	#define Warnf(format, ...)  LogWarnf("(%s): "format, __func__, __VA_ARGS__)
	#define Info(string)        LogInfof("(%s): %s", __func__, string)
	#define Infof(format, ...)  LogInfof("(%s): "format, __func__, __VA_ARGS__)
	#define Debug(string)       LogDebugf("(%s): %s", __func__, string)
	#define Debugf(format, ...) LogDebugf("(%s): "format, __func__, __VA_ARGS__)
#else
	#define Print(string)
	#define Printf(format, ...)
	#define Warn(string)
	#define Warnf(format, ...)
	#define Info(string)
	#define Infof(format, ...)
	#define Debug(string)
	#define Debugf(format, ...)
#endif

/**
 * Custom controller on print
 * The debug info inside one file can only show the log in case LOCAL_TRACE=1 is defined in local file
 */
#define LTRACE(x)     do { if (LOCAL_TRACE != 0) { Print(x); } } while (0)
#define LTRACEF(x...) do { if (LOCAL_TRACE != 0) { Printf(x); } } while (0)

/**
 * Misc
 */
extern u8 GenericBufferUsedFlag;

#define WARN_GENERIC_BUF_USED \
do { \
	if (CONFIG_DEBUG_ABORT_ON_ERROR_USING_GENERIC_BUFFER) { \
		Assert(GenericBufferUsedFlag == 0); \
	} \
	GenericBufferUsedFlag = 1; \
	if (CONFIG_DEBUG_ON_USING_GENERIC_BUFFER) { \
		Warn("Generic buffer used"); \
	} \
} while (0)

#define WARN_GENERIC_BUF_RELEASED \
do { \
	if (CONFIG_DEBUG_ABORT_ON_ERROR_USING_GENERIC_BUFFER) { \
		Assert(GenericBufferUsedFlag == 1); \
	} \
	GenericBufferUsedFlag = 0; \
	if (CONFIG_DEBUG_ON_USING_GENERIC_BUFFER) { \
		Warn("Generic buffer released"); \
	} \
} while (0)
