#include "ThreadTracker.hpp"
void ThreadTracker::assert_main_thread(){
    if (!is_main_thread()){
        LOG_WARN_NOLOCK("THREAD {} (type={}) IS NOT MAIN THREAD.",my_id(),my_thread_type());
        BREAKPOINT();
    }
}
