#include "UIDebugLog.hpp"
DebugLog global_log{};

void log_to_all_impl(std::string s ){
    std::lock_guard<std::mutex> guard(global_log_write);
    global_log.make_entry(s);
}
