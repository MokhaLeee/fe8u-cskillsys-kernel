#pragma once

#include <stdlib.h>
#include <string.h>

#include "mgba.h"
#include "no-cash-gba.h"

#ifdef CONFIG_USE_DEBUG

static inline void LogInit(void)
{
    mgba_open();
}

#define LogPrint(string) { mgba_printf(MGBA_LOG_INFO, string); \
                            NoCashGBAPrint(string); }
#define LogPrintf(format, ...) { mgba_printf(MGBA_LOG_INFO, format, __VA_ARGS__); \
                            NoCashGBAPrintf(format, __VA_ARGS__); }

#define LogFatal(string) { mgba_printf(MGBA_LOG_FATAL, string); \
                            NoCashGBAPrint("[FATAL] "string); \
                            abort(); }

#define LogFatalf(format, ...) { mgba_printf(MGBA_LOG_FATAL, format, __VA_ARGS__); \
                            NoCashGBAPrintf("[FATAL] "format, __VA_ARGS__); \
                            abort(); };

#define LogError(string) { mgba_printf(MGBA_LOG_ERROR, string); \
                            NoCashGBAPrint("[ERROR] "string); }
#define LogErrorf(format, ...) { mgba_printf(MGBA_LOG_ERROR, format, __VA_ARGS__); \
                            NoCashGBAPrintf("[ERROR] "format, __VA_ARGS__); }

#define LogWarn(string) { mgba_printf(MGBA_LOG_WARN, string); \
                            NoCashGBAPrint("[WARN] "string); }
#define LogWarnf(format, ...) { mgba_printf(MGBA_LOG_WARN, format, __VA_ARGS__); \
                            NoCashGBAPrintf("[WARN] "format, __VA_ARGS__); }

#define LogInfo(string) { mgba_printf(MGBA_LOG_INFO, string); \
                            NoCashGBAPrint("[INFO] "string); }
#define LogInfof(format, ...) { mgba_printf(MGBA_LOG_INFO, format, __VA_ARGS__); \
                            NoCashGBAPrintf("[INFO] "format, __VA_ARGS__); }

#define LogDebug(string) { mgba_printf(MGBA_LOG_DEBUG, string); \
                            NoCashGBAPrint("[DEBUG] "string); }
#define LogDebugf(format, ...) { mgba_printf(MGBA_LOG_DEBUG, format, __VA_ARGS__); \
                            NoCashGBAPrintf("[DEBUG] "format, __VA_ARGS__); }

#define Print(string) LogPrintf("(%s): %s", __func__, string)
#define Printf(format, ...) LogPrintf("(%s): "format, __func__, __VA_ARGS__)

#define Fatal(string) LogFatalf("(%s): %s", __func__, string)
#define Fatalf(format, ...) LogFatalf("(%s): "format, __func__, __VA_ARGS__)

#define Error(string) LogErrorf("(%s): %s", __func__, string)
#define Errorf(format, ...) LogErrorf("(%s): "format, __func__, __VA_ARGS__)

#define Warn(string) LogWarnf("(%s): %s", __func__, string)
#define Warnf(format, ...) LogWarnf("(%s): "format, __func__, __VA_ARGS__)

#define Info(string) LogInfof("(%s): %s", __func__, string)
#define Infof(format, ...) LogInfof("(%s): "format, __func__, __VA_ARGS__)

#define Debug(string) LogDebugf("(%s): %s", __func__, string)
#define Debugf(format, ...) LogDebugf("(%s): "format, __func__, __VA_ARGS__)

#define Assert(condition) if (!(condition)) { Fatal("Assertion failed: " #condition); }

#else

static inline void LogInit(void)
{
    return;
}

#define LogPrint(string)
#define LogPrintf(format, ...)
#define LogFatal(string)
#define LogFatalf(format, ...)
#define LogError(string)
#define LogErrorf(format, ...)
#define LogWarn(string)
#define LogWarnf(format, ...)
#define LogInfo(string)
#define LogInfof(format, ...)
#define LogDebug(string)
#define LogDebugf(format, ...)
#define Print(string)
#define Printf(format, ...)
#define Fatal(string)
#define Fatalf(format, ...)
#define Error(string)
#define Errorf(format, ...)
#define Warn(string)
#define Warnf(format, ...)
#define Info(string)
#define Infof(format, ...)
#define Debug(string)
#define Debugf(format, ...)
#define Assert(condition)

#endif
