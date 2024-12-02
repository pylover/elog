#if !defined(ELOG_H_) && defined(ESPIDF_DEBUG)
#define ELOG_H_


#include <stdarg.h>
#include <string.h>


enum elog_verbosity {
    ELOG_UNKNOWN = -1,
    ELOG_SILENT = 0,
    ELOG_FATAL = 1,
    ELOG_ERROR = 2,
    ELOG_WARNING = 3,
    ELOG_INFO = 4,
    ELOG_DEBUG = 5,
};


typedef char* (*elog_strerror_t) (int errnum);


extern enum elog_verbosity elog_verbosity;
extern elog_strerror_t elog_strerror;
extern int elog_outfd;
extern int elog_errfd;


#ifndef ELOG_LF
#define ELOG_LF "\n"
#endif


#define ELOG_NOTRAILINGLF 0x1


void
elog_vlog(enum elog_verbosity level, const char *filename, int lineno,
        const char *function, int flags, const char *format,
        va_list args);


void
elog_log(enum elog_verbosity level, const char *filename, int lineno,
        const char *function, int flags, const char *format, ...);


#define LOG(l, n, ...) \
    elog_log(l, basename(__FILE__), __LINE__ , __FUNCTION__, n, __VA_ARGS__)


/* Regular logging macros */
#define DEBUG(...)   LOG(ELOG_DEBUG,   0, __VA_ARGS__)
#define INFO(...)    LOG(ELOG_INFO,    0, __VA_ARGS__)
#define WARN(...)    LOG(ELOG_WARNING, 0, __VA_ARGS__)
#define ERROR(...)   LOG(ELOG_ERROR,   0, __VA_ARGS__)
#define FATAL(...)   LOG(ELOG_FATAL,   0, __VA_ARGS__)

/* No newline appended */
#define DEBUGN(...)   LOG(ELOG_DEBUG,   ELOG_NOTRAILINGLF, __VA_ARGS__)
#define INFON(...)    LOG(ELOG_INFO,    ELOG_NOTRAILINGLF, __VA_ARGS__)
#define WARNN(...)    LOG(ELOG_WARNING, ELOG_NOTRAILINGLF, __VA_ARGS__)
#define ERRORN(...)   LOG(ELOG_ERROR,   ELOG_NOTRAILINGLF, __VA_ARGS__)
#define FATALN(...)   LOG(ELOG_FATAL,   ELOG_NOTRAILINGLF, __VA_ARGS__)

/* Just prints into standard error */
#define PRINT(...)  dprintf(elog_outfd, __VA_ARGS__)


#endif  // ELOG_H_
