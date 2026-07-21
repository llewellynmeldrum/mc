#include "DebugChunkLog.hpp"
#include "FormatSpecs.hpp"
#include "ThreadTracker.hpp"

void DebugChunkLogger::log_to_chunk(WorldChunkCoord key, std::string_view msg){
    std::lock_guard lock (per_chunk_log_mut);
    log_to_chunk(default_log_type,key,msg);
}
void DebugChunkLogger::log_to_chunk(std::string_view log_type, WorldChunkCoord key, std::string_view msg){
    if (!pause_logging){
        std::lock_guard lock (per_chunk_log_mut);
        auto [it, inserted] = per_chunk_log.try_emplace(key, global_epoch);
        it->second.make_entry(log_type,msg);
    }
}
