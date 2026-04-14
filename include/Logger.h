#pragma once

#include <ctype.h>
#include <errno.h>
#include <print>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Types.h"
#include <time.h>

extern u64 program_epoch_ns;  // TODO: Must be defined in main file! do get_current_ns() as soon as
                              // main begins.
#define OSTREAM stdout

// clang-format off
static const char* errno_meaning_str[] = { "NONE", "Operation not permitted", "No such file or directory", "No such process", "Interrupted system call", "I/O error", "No such device or address", "Argument list too long", "Exec format error", "Bad file number", "No child processes", "Try again", "Out of memory", "Permission denied", "Bad address", "Block device required", "Device or resource busy", "File exists", "Cross-device link", "No such device", "Not a directory", "Is a directory", "Invalid argument", "File table overflow", "Too many open files", "Not a typewriter", "Text file busy", "File too large", "No space left on device", "Illegal seek", "Read-only file system", "Too many links", "Broken pipe", "Math argument out of domain of func", "Math result not representable", "Resource deadlock would occur", "File name too long", "No record locks available", "Invalid system call number", "Directory not empty", "Too many symbolic links encountered", "Operation would block", "No message of desired type", "Identifier removed", "Channel number out of range", "Level 2 not synchronized", "Level 3 halted", "Level 3 reset", "Link number out of range", "Protocol driver not attached", "No CSI structure available", "Level 2 halted", "Invalid exchange", "Invalid request descriptor", "Exchange full", "No anode", "Invalid request code", "Invalid slot", "Deadlock?",  "Bad font file format", "Device not a stream", "No data available", "Timer expired", "Out of streams resources", "Machine is not on the network", "Package not installed", "Object is remote", "Link has been severed", "Advertise error", "Srmount error", "Communication error on send", "Protocol error", "Multihop attempted", "RFS specific error", "Not a data message", "Value too large for defined data type", "Name not unique on network", "File descriptor in bad state", "Remote address changed", "Can not access a needed shared library", "Accessing a corrupted shared library", ".lib section in a.out corrupted", "Attempting to link in too many shared libraries", "Cannot exec a shared library directly", "Illegal byte sequence", "Interrupted system call should be restarted", "Streams pipe error", "Too many users", "Socket operation on non-socket", "Destination address required", "Message too long", "Protocol wrong type for socket", "Protocol not available", "Protocol not supported", "Socket type not supported", "Operation not supported on transport endpoint", "Protocol family not supported", "Address family not supported by protocol", "Address already in use", "Cannot assign requested address", "Network is down", "Network is unreachable", "Network dropped connection because of reset", "Software caused connection abort", "Connection reset by peer", "No buffer space available", "Transport endpoint is already connected", "Transport endpoint is not connected", "Cannot send after transport endpoint shutdown", "Too many references: cannot splice", "Connection timed out", "Connection refused", "Host is down", "No route to host", "Operation already in progress", "Operation now in progress", "Stale file handle", "Structure needs cleaning", "Not a XENIX named type file", "No XENIX semaphores available", "Is a named type file",
    "Remote I/O error", "Quota exceeded", "No medium found", "Wrong medium type", "Operation Canceled", "Required key not available", "Key has expired", "Key has been revoked", "Key was rejected by service", "Owner died", "State not recoverable", "Operation not possible due to RF-kill", "Memory page has hardware error",
};
static const char* errno_id_str[]={
"NONE","EPERM","ENOENT","ESRCH","EINTR","EIO","ENXIO","E2BIG","ENOEXEC","EBADF","ECHILD","EAGAIN","ENOMEM","EACCES","EFAULT","ENOTBLK","EBUSY","EEXIST","EXDEV","ENODEV","ENOTDIR","EISDIR","EINVAL","ENFILE","EMFILE","ENOTTY","ETXTBSY","EFBIG","ENOSPC","ESPIPE","EROFS","EMLINK","EPIPE","EDOM","ERANGE","EDEADLK","ENAMETOOLONG","ENOLCK","ENOSYS","ENOTEMPTY","ELOOP","EWOULDBLOCK","ENOMSG","EIDRM","ECHRNG","EL2NSYNC","EL3HLT","EL3RST","ELNRNG","EUNATCH","ENOCSI","EL2HLT","EBADE","EBADR","EXFULL","ENOANO","EBADRQC","EBADSLT","EDEADLOCK","EBFONT","ENOSTR","ENODATA","ETIME","ENOSR","ENONET","ENOPKG","EREMOTE","ENOLINK","EADV","ESRMNT","ECOMM","EPROTO","EMULTIHOP","EDOTDOT","EBADMSG","EOVERFLOW","ENOTUNIQ","EBADFD","EREMCHG","ELIBACC","ELIBBAD","ELIBSCN","ELIBMAX","ELIBEXEC","EILSEQ","ERESTART","ESTRPIPE","EUSERS","ENOTSOCK","EDESTADDRREQ","EMSGSIZE","EPROTOTYPE","ENOPROTOOPT","EPROTONOSUPPORT","ESOCKTNOSUPPORT","EOPNOTSUPP","EPFNOSUPPORT","EAFNOSUPPORT","EADDRINUSE","EADDRNOTAVAIL","ENETDOWN","ENETUNREACH","ENETRESET","ECONNABORTED","ECONNRESET","ENOBUFS","EISCONN","ENOTCONN","ESHUTDOWN","ETOOMANYREFS","ETIMEDOUT","ECONNREFUSED","EHOSTDOWN","EHOSTUNREACH","EALREADY","EINPROGRESS","ESTALE","EUCLEAN","ENOTNAM","ENAVAIL","EISNAM","EREMOTEIO","EDQUOT","ENOMEDIUM","EMEDIUMTYPE","ECANCELED","ENOKEY","EKEYEXPIRED","EKEYREVOKED","EKEYREJECTED","EOWNERDEAD","ENOTRECOVERABLE","ERFKILL","EHWPOISON",
};
#define BOLD "\e[1m"
#define UNDERLINE "\e[4m"
#define UNDERBOLD "\e[1;4m"

#define FMT_CLEAR "\e[0m"
#define ITALIC "\e[4m"
#define REV "\e[7m"
#define BOLD_RED "\e[1;31m"
#define BOLD_GREEN "\e[1;32m"
#define BOLD_BLUE "\e[1;33m"
#define BOLD_YELLOW "\e[1;34m"
#define BG_WHITE_FG_BLACK "\e[47;30m"

#define LIGREY "\e[90m"
#define PURPLE "\e[36m"
#define CYAN "\e[34m"
#define GREEN "\e[32m"
#define YELLOW "\e[33m"
#define PUPLE2 "\e[35m"
#define IYELLOW "\e[7;33m"
#define PINK "\e[35m"
#define LIRED "\e[91m"
#define RED "\e[31m"
#define DWHITE "\e[96m"
#define BOLD_DWHITE "\e[1;96m"


static inline double nstoms(const u64 ns) {
    return ns / 1000000.0;
}
static inline double stons(const u64 ns) {
    return ns * 1000000000ULL;
}

static inline double get_current_ns(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (u64)stons(ts.tv_sec) + ts.tv_nsec;
}

static inline u64 ms_since_start(void) {
    u64    current_ns = get_current_ns();
    u64    ns_elapsed = current_ns - program_epoch_ns;
    double ms = nstoms(ns_elapsed);
    return ms;
}

static inline void log_trace(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(OSTREAM, fmt, ap);
    va_end(ap);
}

static inline void LOG_ERRNO(void) {
    log_trace("\t%s(%d) = '%s'\n", errno_id_str[errno], errno, errno_meaning_str[errno]);
}

static inline void log_return_internal(const char* func_str, const char* expr_str) {
    double ms = ms_since_start();
    log_trace(FMT_CLEAR);
    log_trace("%06.3lfs ", ms / 1000.0);
    log_trace(CYAN);
    log_trace("%-8s ", "FUNCTION");
    log_trace(FMT_CLEAR);

    log_trace(BOLD_DWHITE);
    log_trace("%s: ", func_str);
    log_trace(FMT_CLEAR);
    log_trace("returned -> %s", expr_str);
    log_trace("\n");
    if (strcmp(expr_str, "void.") != 0) {
        free((void*)expr_str);
    }
}

static const char* FMT_LOGLEVEL_COLORS[] = {
    CYAN,    // DEBUG
    CYAN,    // RETURN
    LIGREY,  // INFO
    YELLOW,  // NOTICE
    PINK,    // WARN
    LIRED,   // ERROR
    RED,     // FATAL
    GREEN,   // EXIT_SUCCESS
    RED,     // EXIT_FAILURE
};

// clang-format off
static const char* loglevel_tostr[] = { 
    "[DEBUG]", 
    "[DEBUG-RET]", 
    "[INFO_]",
    "[NOTE_]",
    "[WARN_]",
    "[ERROR]",
    "[FATAL]",
    "[N/A]  ",
    "[EXIT]",
    "[EXIT]", 
};

typedef enum LogLevel {
    LogLevel_DEBUG,
    LogLevel_DEBUG_RETURN,
    LogLevel_INFO,
    LogLevel_NOTICE,
    LogLevel_WARN,
    LogLevel_ERROR,
    LogLevel_FATAL,
    LogLevel__COUNT,
    LogLevel_EXIT_SUCCESS,
    LogLevel_EXIT_FAILURE,
} LogLevel;

// everything below this will be ignored
#define LOGLEVEL LogLevel_DEBUG
static inline void log_internal(LogLevel level, const char* filename, int line, const char* fmt, ...) {
    if (level<LOGLEVEL){
        return;
    }
    #ifdef LOGGER_DISABLE_TIMER
    double ms = ms_since_start();
    log_trace(FMT_CLEAR);
    log_trace("%06.3lfs ", ms / 1000.0);
    #endif
    log_trace(FMT_LOGLEVEL_COLORS[level]);
    log_trace("%-8s", loglevel_tostr[level]);
    if (level!=LogLevel_FATAL){
        log_trace(FMT_CLEAR);
        log_trace(BOLD_DWHITE);
    } else{
        log_trace(BOLD);
    }
    log_trace("%s:%d: ", filename, line);
    if (!(level==LogLevel_FATAL)){
        log_trace(FMT_CLEAR);
    }
    va_list ap;
    va_start(ap, fmt);
    vfprintf(OSTREAM, fmt, ap);
    va_end(ap);
    fprintf(OSTREAM, "\n");
        log_trace(FMT_CLEAR);
    if (level==LogLevel_EXIT_FAILURE || level==LogLevel_EXIT_SUCCESS){
        exit(level);
    }
}

#define SETLOG_SHOWFUNCTIONS(val) log_settings.showFunctions = val;
#define LOG_LVL(lvl,file, ln, fmt, ...)         std::println("{:03.3f} {}{:<8}{} {}{}:{:<3}{} {}{}| " fmt,\
                                                 ms_since_start()/1000.0,\
                                                 FMT_LOGLEVEL_COLORS[lvl],\
                                                 loglevel_tostr[lvl],FMT_CLEAR,\
                                                 BOLD, file,ln,FMT_CLEAR,FMT_LOGLEVEL_COLORS[lvl],FMT_CLEAR,\
                                                 ##__VA_ARGS__)

#define LOG_DEBUG(fmt, ...) LOG_LVL(LogLevel_DEBUG, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG_LVL(LogLevel_INFO, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)
#define LOG_NOTICE(fmt, ...) LOG_LVL(LogLevel_NOTICE, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LOG_LVL(LogLevel_WARN, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG_LVL(LogLevel_ERROR, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOG_LVL(LogLevel_FATAL, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)

#define LOG_EXIT(code) do{\
LOG_LVL(LogLevel_FATAL, __FILE_NAME__,__LINE__ ,"Exiting with code {}.",code);\
exit(code);\
}while(0)

