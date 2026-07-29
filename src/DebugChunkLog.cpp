#include "DebugChunkLog.hpp"
#include "FormatSpecs.hpp"
#include "ThreadTracker.hpp"

void PerChunkLogHandler::log_to_chunk(WorldChunkCoord key, std::string_view msg){
    auto lock = per_chunk_log.lock_guard();
    log_to_chunk(LogType::all,key,msg);
}
void PerChunkLogHandler::log_to_chunk(LogType log_type, WorldChunkCoord key, std::string_view msg){
    if (!pause_logging){
        auto lock = per_chunk_log.lock_guard();
        auto [it, inserted] = per_chunk_log.try_emplace(key, global_epoch);
        it->second.make_entry(log_type,msg);
    }
}
