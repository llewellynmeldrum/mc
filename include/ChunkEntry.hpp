#pragma once 

#include <functional>

#include "Breakpoints.hpp"
#include "Chunk.hpp"
#include "ChunkHelpers.hpp"
#include "ChunkStorage.hpp"
#include "CoordTypes.hpp"
#include "DebugChunkLog.hpp"
#include "Geometry.hpp"
#include "Types.h"
#include "WorldGen_NoiseGeneration.hpp"
#include "cppslop.hpp"
#include "BitwiseOps.hpp"
#include "glm/vec4.hpp"
#include "Colors01.hpp"

#include "ChunkNoiseDebug.hpp"

#include "Assertion.hpp"

#include "ChunkState.hpp"
struct RevisionState{
    using ID = u64;
    RevisionState() = default;
    RevisionState(ID _target) : target(_target) {}

    u64 target{0uz};     // The actual underlying data's revision    (++ on MakeDirty())
    u64 inflight{0uz};   // newest revision in flight (on queue)     
    u64 loaded{0uz};     // The data loaded right now
    
    bool is_dirty()const noexcept;
    bool is_clean()const noexcept;
    bool qualifies_for_enqueue(const PipelineState& state)const noexcept;
    bool qualifies_for_dequeue(const PipelineState& state)const noexcept;
    bool is_candidate_newer_than_loaded(ID candidate_id) const noexcept;
    void mark_deleted();
    void mark_dirty()noexcept;
};
// @Brief:
// represents the in memory store of a chunks data.
// A ChunkEntry is created upon request for chunk generation.
struct ChunkEntry{

    ChunkEntry(WorldChunkCoord chunkCoord, i32 worldgen_epoch)
        :coord(chunkCoord)
        ,bounding_box(
            toWorldOrigin(chunkCoord).raw(),
            toWorldOrigin(chunkCoord).raw()+ChunkInfo::Extents3D
        )
        ,neighbours(N_NEIGHBOURS, std::nullopt)
        ,state(chunkCoord)
        ,gen_revision(worldgen_epoch)
        {}

    WorldChunkCoord coord;
    AABB bounding_box; 
    std::vector<std::optional<WorldChunkCoord>> neighbours;
    ChunkBlockStore block_data;
    ChunkState state;
    ChunkLightStore light_data;

    RevisionState mesh_revision;
    RevisionState gen_revision;
    RevisionState light_revision;

    void mark_mesh_deleted();
    void mark_gen_deleted();
    bool qualifies_for_mesh_enqueue()const noexcept;
    bool qualifies_for_mesh_dequeue()const noexcept;
    bool qualifies_for_gen_enqueue()const noexcept;
    bool qualifies_for_gen_dequeue()const noexcept;




    template<typename Fn>
    void state_transition(Fn&& fn) {
        auto before = ChunkState(state);
        std::invoke(fn, &state);
        auto after = ChunkState(state);
        transition_logger(before,after);
    }
};
