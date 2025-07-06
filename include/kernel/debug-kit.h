#pragma once

#include <stdlib.h>
#include <string.h>

#include "mgba.h"
#include "no-cash-gba.h"

#define LogInit() {mgba_open(); }
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
 * FPrint
 * Always print to STDOUT regardless on debug config
 */
#define FPrint(string)       LogPrintf("(%s): %s", __func__, string)
#define FPrintf(format, ...) LogPrintf("(%s): "format, __func__, __VA_ARGS__)

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
