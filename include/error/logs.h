#pragma once
#include <stdio.h>

#ifdef LOGCOLORED

#ifdef LOGSUCCESS
#define SSUCCESS(format, ...)                                                    \
  do {                                                                         \
    fprintf(stderr, "\x1b[32m[\x1b[1;32mSUCCESS\x1b[0m\x1b[32m] %s:%d: \x1b[0m", \
            __FILE__, __LINE__);                                               \
    fprintf(stderr, format, ##__VA_ARGS__);                                    \
    fprintf(stderr, "\n");                                                     \
  } while (0)
#endif

#ifdef LOGERROR
#define SERROR(format, ...)                                                    \
  do {                                                                         \
    fprintf(stderr, "\x1b[31m[\x1b[1;37mERROR\x1b[0m\x1b[31m] %s:%d: \x1b[0m", \
            __FILE__, __LINE__);                                               \
    fprintf(stderr, format, ##__VA_ARGS__);                                    \
    fprintf(stderr, "\n");                                                     \
  } while (0)
#endif

#ifdef LOGWARN
#define SWARN(format, ...)                                                     \
  do {                                                                         \
    fprintf(stderr, "\x1b[93m[\x1b[1;37mWARN\x1b[0m\x1b[93m] %s:%d: \x1b[0m",  \
            __FILE__, __LINE__);                                               \
    fprintf(stderr, format, ##__VA_ARGS__);                                    \
    fprintf(stderr, "\n");                                                     \
  } while (0)
#endif

#ifdef LOGDEBUG
#define SDEBUG(format, ...)                                                    \
  do {                                                                         \
    fprintf(stderr, "\x1b[96m[\x1b[1;37mDEBUG\x1b[0m\x1b[96m] %s:%d: \x1b[0m", \
            __FILE__, __LINE__);                                               \
    fprintf(stdout, format, ##__VA_ARGS__);                                    \
    fprintf(stdout, "\n");                                                     \
  } while (0)
#else
#define SDEBUG(format, ...)
#endif

#ifdef LOGTRACE
#define STRACE(format, ...)                                                    \
  do {                                                                         \
    fprintf(stderr, "\x1b[92m[\x1b[1;37mTRACE\x1b[0m\x1b[92m] %s:%d: \x1b[0m", \
            __FILE__, __LINE__);                                               \
    fprintf(stdout, format, ##__VA_ARGS__);                                    \
    fprintf(stdout, "\n");                                                     \
  } while (0)
#else
#define STRACE(format, ...)
#endif

#ifdef LOGINFO
#define SINFO(format, ...)                                                     \
  do {                                                                         \
    fprintf(stderr, "\x1b[37m[\x1b[1;37mINFO\x1b[0m\x1b[37m] %s:%d: \x1b[0m",  \
            __FILE__, __LINE__);                                               \
    fprintf(stdout, format, ##__VA_ARGS__);                                    \
    fprintf(stdout, "\n");                                                     \
  } while (0)
#endif

#endif

#ifndef LOGCOLORED
#ifdef LOGERROR
#define SERROR(format, ...)                                                    \
  do {                                                                         \
    fprintf(stderr, "[ERROR] %s:%d: ", __FILE__, __LINE__);                    \
    fprintf(stderr, format, ##__VA_ARGS__);                                    \
    fprintf(stderr, "\n");                                                     \
  } while (0)
#endif

#ifdef LOGWARN
#define SWARN(format, ...)                                                     \
  do {                                                                         \
    fprintf(stderr, "[WARN] %s:%d: ", __FILE__, __LINE__);                     \
    fprintf(stderr, format, ##__VA_ARGS__);                                    \
    fprintf(stderr, "\n");                                                     \
  } while (0)
#endif


#ifdef LOGDEBUG
#define SDEBUG(format, ...)                                                    \
  do {                                                                         \
    fprintf(stdout, "[DEBUG] %s:%d: ", __FILE__, __LINE__);                    \
    fprintf(stdout, format, ##__VA_ARGS__);                                    \
    fprintf(stdout, "\n");                                                     \
  } while (0)
#else
#define SDEBUG(format, ...)
#endif

#ifdef LOGTRACE
#define STRACE(format, ...)                                                    \
  do {                                                                         \
    fprintf(stdout, "[TRACE] %s:%d: ", __FILE__, __LINE__);                    \
    fprintf(stdout, format, ##__VA_ARGS__);                                    \
    fprintf(stdout, "\n");                                                     \
  } while (0)
#else
#define STRACE(format, ...)
#endif

#ifdef LOGINFO
#define SINFO(format, ...)                                                     \
  do {                                                                         \
    fprintf(stdout, "[INFO] %s:%d: ", __FILE__, __LINE__);                     \
    fprintf(stdout, format, ##__VA_ARGS__);                                    \
    fprintf(stdout, "\n");                                                     \
  } while (0)
#endif
#endif
