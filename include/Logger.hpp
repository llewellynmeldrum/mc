#pragma once

#include <print>
#include <cxxabi.h>
#include <memory>
#include <string>
#include <sstream>
#include <concepts>
#include <format>
#include <print>
#include <string>
#include <string_view>
#include <type_traits>

#include "Types.h"
#include "glHelpers.hpp"
#include "glbinding-aux/types_to_string.h"
#include "glbinding/gl/enum.h"
#include "glmWrapper.hpp"

extern u64 program_epoch_ns;  // TODO: Must be defined in main file! do get_current_ns() as soon as
                              // main begins.

// clang-format off
static const char* errno_meaning_str[] = { "NONE", "Operation not permitted", "No such file or directory", "No such process", "Interrupted system call", "I/O error", "No such device or address", "Argument list too long", "Exec format error", "Bad file number", "No child processes", "Try again", "Out of memory", "Permission denied", "Bad address", "Block device required", "Device or resource busy", "File exists", "Cross-device link", "No such device", "Not a directory", "Is a directory", "Invalid argument", "File table overflow", "Too many open files", "Not a typewriter", "Text file busy", "File too large", "No space left on device", "Illegal seek", "Read-only file system", "Too many links", "Broken pipe", "Math argument out of domain of func", "Math result not representable", "Resource deadlock would occur", "File name too long", "No record locks available", "Invalid system call number", "Directory not empty", "Too many symbolic links encountered", "Operation would block", "No message of desired type", "Identifier removed", "Channel number out of range", "Level 2 not synchronized", "Level 3 halted", "Level 3 reset", "Link number out of range", "Protocol driver not attached", "No CSI structure available", "Level 2 halted", "Invalid exchange", "Invalid request descriptor", "Exchange full", "No anode", "Invalid request code", "Invalid slot", "Deadlock?",  "Bad font file format", "Device not a stream", "No data available", "Timer expired", "Out of streams resources", "Machine is not on the network", "Package not installed", "Object is remote", "Link has been severed", "Advertise error", "Srmount error", "Communication error on send", "Protocol error", "Multihop attempted", "RFS specific error", "Not a data message", "Value too large for defined data type", "Name not unique on network", "File descriptor in bad state", "Remote address changed", "Can not access a needed shared library", "Accessing a corrupted shared library", ".lib section in a.out corrupted", "Attempting to link in too many shared libraries", "Cannot exec a shared library directly", "Illegal byte sequence", "Interrupted system call should be restarted", "Streams pipe error", "Too many users", "Socket operation on non-socket", "Destination address required", "Message too long", "Protocol wrong type for socket", "Protocol not available", "Protocol not supported", "Socket type not supported", "Operation not supported on transport endpoint", "Protocol family not supported", "Address family not supported by protocol", "Address already in use", "Cannot assign requested address", "Network is down", "Network is unreachable", "Network dropped connection because of reset", "Software caused connection abort", "Connection reset by peer", "No buffer space available", "Transport endpoint is already connected", "Transport endpoint is not connected", "Cannot send after transport endpoint shutdown", "Too many references: cannot splice", "Connection timed out", "Connection refused", "Host is down", "No route to host", "Operation already in progress", "Operation now in progress", "Stale file handle", "Structure needs cleaning", "Not a XENIX named type file", "No XENIX semaphores available", "Is a named type file",
    "Remote I/O error", "Quota exceeded", "No medium found", "Wrong medium type", "Operation Canceled", "Required key not available", "Key has expired", "Key has been revoked", "Key was rejected by service", "Owner died", "State not recoverable", "Operation not possible due to RF-kill", "Memory page has hardware error",
};
static const char* errno_id_str[]={
"NONE","EPERM","ENOENT","ESRCH","EINTR","EIO","ENXIO","E2BIG","ENOEXEC","EBADF","ECHILD","EAGAIN","ENOMEM","EACCES","EFAULT","ENOTBLK","EBUSY","EEXIST","EXDEV","ENODEV","ENOTDIR","EISDIR","EINVAL","ENFILE","EMFILE","ENOTTY","ETXTBSY","EFBIG","ENOSPC","ESPIPE","EROFS","EMLINK","EPIPE","EDOM","ERANGE","EDEADLK","ENAMETOOLONG","ENOLCK","ENOSYS","ENOTEMPTY","ELOOP","EWOULDBLOCK","ENOMSG","EIDRM","ECHRNG","EL2NSYNC","EL3HLT","EL3RST","ELNRNG","EUNATCH","ENOCSI","EL2HLT","EBADE","EBADR","EXFULL","ENOANO","EBADRQC","EBADSLT","EDEADLOCK","EBFONT","ENOSTR","ENODATA","ETIME","ENOSR","ENONET","ENOPKG","EREMOTE","ENOLINK","EADV","ESRMNT","ECOMM","EPROTO","EMULTIHOP","EDOTDOT","EBADMSG","EOVERFLOW","ENOTUNIQ","EBADFD","EREMCHG","ELIBACC","ELIBBAD","ELIBSCN","ELIBMAX","ELIBEXEC","EILSEQ","ERESTART","ESTRPIPE","EUSERS","ENOTSOCK","EDESTADDRREQ","EMSGSIZE","EPROTOTYPE","ENOPROTOOPT","EPROTONOSUPPORT","ESOCKTNOSUPPORT","EOPNOTSUPP","EPFNOSUPPORT","EAFNOSUPPORT","EADDRINUSE","EADDRNOTAVAIL","ENETDOWN","ENETUNREACH","ENETRESET","ECONNABORTED","ECONNRESET","ENOBUFS","EISCONN","ENOTCONN","ESHUTDOWN","ETOOMANYREFS","ETIMEDOUT","ECONNREFUSED","EHOSTDOWN","EHOSTUNREACH","EALREADY","EINPROGRESS","ESTALE","EUCLEAN","ENOTNAM","ENAVAIL","EISNAM","EREMOTEIO","EDQUOT","ENOMEDIUM","EMEDIUMTYPE","ECANCELED","ENOKEY","EKEYEXPIRED","EKEYREVOKED","EKEYREJECTED","EOWNERDEAD","ENOTRECOVERABLE","ERFKILL","EHWPOISON",
};
namespace fmt{
const static char* bold                             = "\e[1m";
const static char* underline                            = "\e[4m";
const static char* underbold                            = "\e[1;4m";

const static char* clear                            = "\e[0m";
const static char* italic                           = "\e[4m";
const static char* rev                          = "\e[7m";
const static char* bold_red                             = "\e[1;31m";
const static char* bold_green                           = "\e[1;32m";
const static char* bold_blue                            = "\e[1;33m";
const static char* bold_yellow                          = "\e[1;34m";
const static char* bg_white_fg_black                            = "\e[47;30m";

const static char* ligrey                           = "\e[90m";
const static char* cyan                             = "\e[34m";
const static char* green                            = "\e[32m";
const static char* blue                             = "\e[36m";
const static char* grey                             = "\e[90";
const static char* yellow                           = "\e[33m";
const static char* purple                           = "\e[35m";
const static char* iyellow                          = "\e[7;33m";
const static char* pink                             = "\e[35m";
const static char* lired                            = "\e[91m";
const static char* red                          = "\e[31m";
const static char* dwhite                           = "\e[96m";
const static char* bold_dwhite                          = "\e[1;96m";
}


static inline double nstoms(const u64 ns) {
    return ns / 1000000.0;
}
static inline double stons(const u64 ns) {
    return ns * 1000000000ULL;
}

static inline double get_current_ns() {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (u64)stons(ts.tv_sec) + ts.tv_nsec;
}

static inline u64 ms_since_start() {
    u64    current_ns = get_current_ns();
    u64    ns_elapsed = current_ns - program_epoch_ns;
    double ms = nstoms(ns_elapsed);
    return ms;
}



struct LogLevel{
    const char* prefix;
    const char* color;
    i32 precedence;
};
LogLevel DEBUG{"[DEBUG]",       fmt::cyan,  -1};
LogLevel INFO{"[INFO]",         fmt::yellow,0};
LogLevel NOTICE{"[NOTICE]",     fmt::pink,  1};
LogLevel WARN{"[WARN]",         fmt::lired, 2};
LogLevel ERROR{"[ERROR]",       fmt::red,   3};
LogLevel FATAL{"[FATAL]",       fmt::green, 4};
LogLevel COUNT{"[COUNT]",       fmt::red,   5};



#define LOG_LVL(lvl,file, ln, fmt_str, ...)do{                          \
    std::print("{:03.3f} ", ms_since_start()/1000.0);                \
    std::print("{}{:<8}{} ", lvl.color, lvl.prefix, fmt::clear);       \
    std::print("{}{}:{:<3}{} ", fmt::bold, file, ln, fmt::clear);      \
    std::println(fmt_str, ##__VA_ARGS__);                               \
}while(0)

#define LOG_DEBUG(fmt, ...) LOG_LVL(DEBUG, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG_LVL(INFO, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)
#define LOG_NOTICE(fmt, ...) LOG_LVL(NOTICE, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LOG_LVL(WARN, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG_LVL(ERROR, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOG_LVL(FATAL, __FILE_NAME__,__LINE__ ,fmt, ##__VA_ARGS__)

#define LOG_EXIT(code) do{\
    LOG_LVL(FATAL, __FILE_NAME__,__LINE__ ,"Exiting with code {}.",code);\
    exit(code);\
}while(0)


template <typename T>
constexpr std::string_view pretty_type_name() {
#if defined(__clang__)
    std::string_view p = __PRETTY_FUNCTION__;
    auto start = p.find("T = ");
    start += 4;
    auto end = p.rfind(']');
    return p.substr(start, end - start);

#elif defined(__GNUC__)
    std::string_view p = __PRETTY_FUNCTION__;
    auto start = p.find("with T = ");
    start += 9;
    auto end = p.find(';', start);
    return p.substr(start, end - start);

#else
    return "unsupported compiler";
#endif
}

// Requires that T is implemented by std::format
template <typename T>
concept Formattable = std::formattable<std::remove_cvref_t<T>, char>;

// Requires that a type implements a '<<' overload which returns an ostream& 
template <typename T>
concept OStreamable = requires(T x, std::ostream& os){
    {os << x} -> std::same_as<std::ostream&>;
};

// returns a string containing the type and value category, i.e the type that is passed with perfect forwarding
template <typename T>
constexpr std::string fmt_expr(const char* identifier, T&& expr) {
    using Arg = std::remove_reference_t<T>;
    std::string expr_str{};

    if constexpr (Formattable<T>) {
        expr_str = std::format("{}",expr);
    } if constexpr (OStreamable<T>){
        std::ostringstream oss;
        oss << expr;
        std::string expr_str = oss.str();
    }else if constexpr (is_glm_type<T>){
        if constexpr (is_glm_vec2<T>){
            expr_str = std::format("[{}{}{}, {}{}{}]",
                                    fmt::red, expr.x, fmt::clear,
                                    fmt::green, expr.y, fmt::clear);
        }else if constexpr(is_glm_vec3<T>){
            expr_str = std::format("[{}{}{}, {}{}{}, {}{}{}]",
                                    fmt::red, expr.x, fmt::clear,
                                    fmt::green, expr.y, fmt::clear,
                                    fmt::blue, expr.z, fmt::clear);
        } else if constexpr(std::convertible_to<T,glm::vec<4,float>>){
            expr_str = std::format("[{}{}{}, {}{}{}, {}{}{}, {}{}{}]",
                                    fmt::red, expr.x, fmt::clear,
                                    fmt::green, expr.y, fmt::clear,
                                    fmt::blue, expr.z, fmt::clear,
                                    fmt::grey , expr.w, fmt::clear);
        } else {
            expr_str.append("\n");
            constexpr u64 ext = 4;
            for (u64 row = 0; row< ext; row++){
                expr_str.append("| ");
                for (u64 col = 0; col< ext; col++){
                    expr_str.append(std::format("{: 3.1f}",expr[col][row]));
                    if (col!=ext-1){
                        expr_str.append(" ");
                    }
                }
                expr_str.append(" |");
                if (row!=ext-1)
                expr_str.append("\n");
            }
        }
    }else {
        expr_str =  "<unformattable tomato>";
    }
    return std::format("{}{:>12}{} "
                       "{}{:<12}{} "
                       " = {:<12};",
                       fmt::cyan, pretty_type_name<Arg>(),fmt::clear,
                       fmt::red, identifier, fmt::clear,
                       expr_str);
}

// returns a string containing the underlying type of the object expr represents. No value categories.
template <typename T>
std::string fmt_obj(const char* identifier, T&& expr) {
    using Arg = std::remove_reference_t<T>;

    if constexpr (Formattable<T>) {
        return std::format("{} {} = {}", pretty_type_name<Arg>(), identifier, expr);
    } else {
        return std::format("{} {} = <unformattable tomato>", pretty_type_name<Arg>(), identifier);
    }
}

#define LOG_EXPR(expr) std::println("{}", fmt_expr(#expr, (expr)))



