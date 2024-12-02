#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "elog.h"


enum elog_verbosity elog_verbosity = ELOG_DEBUG;
elog_strerror_t elog_strerror = strerror;
int elog_outfd = STDOUT_FILENO;
int elog_errfd = STDERR_FILENO;


const char * elog_verbosities[] = {
    [ELOG_SILENT]  = "never",  // 0
    [ELOG_FATAL]   = "fatal",  // 1
    [ELOG_ERROR]   = "error",  // 2
    [ELOG_WARNING] = "warn",   // 3
    [ELOG_INFO]    = "info",   // 4
    [ELOG_DEBUG]   = "debug",  // 5
};


void
elog_vlog(enum elog_verbosity level, const char *filename, int lineno,
        const char *function, int flags, const char *format,
        va_list args) {
    int fd = (level <= ELOG_ERROR)? elog_errfd: elog_outfd;

    if (level > elog_verbosity) {
        return;
    }

    dprintf(fd, "[%-5s]", elog_verbosities[level]);
    if (elog_verbosity >= ELOG_DEBUG) {
        dprintf(fd, " [%s:%d %s]", filename, lineno, function);
    }

    if (format) {
        dprintf(fd, " ");
        vdprintf(fd, format, args);
    }

    if (errno && (level != ELOG_INFO)) {
        dprintf(fd, " -- %s. errno: %d", elog_strerror(errno), errno);
    }

    if (!(flags & ELOG_NOTRAILINGLF)) {
        dprintf(fd, ELOG_LF);
    }

    if (level == ELOG_FATAL) {
        exit(EXIT_FAILURE);
    }
}


void
elog_log(enum elog_verbosity level, const char *filename, int lineno,
        const char *function, int flags, const char *format, ...) {
    va_list args;

    if (format) {
        va_start(args, format);
    }

    elog_vlog(level, filename, lineno, function, flags, format, args);

    if (format) {
        va_end(args);
    }
}
