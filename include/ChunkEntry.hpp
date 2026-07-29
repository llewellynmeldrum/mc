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
    static constexpr auto NEVER = 0uz;
    static constexpr auto FIRST_JOB = 1uz;

    auto get_candidate_rev() const noexcept{return target;};
    ID target{0uz};     // The goal/target.  A jobs candidate revision = target.
    ID inflight{0uz};   // newest revision in flight (on queue)     
    ID loaded{0uz};     // The data loaded right now
    

    void mark_job_inflight()                   noexcept { inflight = target; check_invariant(); }
    void complete_inflight(ID candidate)   noexcept { loaded = candidate; inflight = std::max(inflight,candidate); check_invariant(); }
    void drop_inflight()                 noexcept { inflight = loaded; check_invariant(); }

    void mark_dirty()                  noexcept { target++; check_invariant(); }
    void mark_deleted()                noexcept { loaded = NEVER; check_invariant(); }

    bool inflight_result_is_acceptable(ID candidate) noexcept { return candidate > loaded; };

    bool is_on_queue()           const noexcept { return inflight > loaded; check_invariant(); }

    // Previously the PipelineState::done
    bool has_data()               const noexcept { return loaded>NEVER;};
    bool is_dirty()              const noexcept { return target > loaded;  }
    bool is_clean()              const noexcept { return target == loaded;};
    bool needs_work()            const noexcept { return is_on_queue() == false && is_dirty();};

    inline bool check_invariant() const noexcept{
        return loaded<=inflight && inflight<=target;
    }
};

inline bool try_upload_candidate(RevisionState& s, RevisionState::ID candidate){
    if (s.inflight_result_is_acceptable(candidate)){
        s.complete_inflight(candidate);
        return true;
    }else{
        s.drop_inflight();
        return false;
    }
}

inline PipelineState derive_state(const RevisionState& r, bool prev_stage_done){
    if (r.is_on_queue()) 
        return PipelineState::on_queue;
    if (r.has_data()) 
        return PipelineState::done;
    if (prev_stage_done) 
        return PipelineState::ready_for_enqueue;
    else
        return PipelineState::pending;
}
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
        ,gen(worldgen_epoch)
        {}

    WorldChunkCoord coord;
    AABB bounding_box; 
    ChunkBlockStore block_data;
    GenericChunkStore<PackedLightValue> light_data;


    // These functions do not necessarily mean that they SHOULD be enqueued, as there may be other constraints,
    // e.g (neighbours of a chunk being generated is a prereq to meshing)
    bool can_be_generated()const noexcept{
        return gen.needs_work();
    }
    bool can_be_lit()const noexcept{
        return gen.has_data() && lighting.needs_work();
    }
    bool can_be_meshed()const noexcept{
        return gen.has_data() && lighting.has_data() && mesh.needs_work();
    }


    RevisionState mesh, gen, lighting;
    PipelineState gen_pipeline_state()const noexcept{
        return derive_state(gen,true);
    }
    PipelineState lighting_pipeline_state()const noexcept{
        return derive_state(lighting,gen.has_data());
    }
    PipelineState mesh_pipeline_state()const noexcept{
        return derive_state(mesh,gen.has_data() && lighting.has_data());
    }




    auto neighbour_coords8() const noexcept{
        return std::vector<WorldChunkCoord>{
            WorldChunkCoord{coord+ChunkOffset{+1,+0}},
            WorldChunkCoord{coord+ChunkOffset{+1,+1}},
            WorldChunkCoord{coord+ChunkOffset{ 0,+1}},
            WorldChunkCoord{coord+ChunkOffset{-1, 0}},
            WorldChunkCoord{coord+ChunkOffset{-1,-1}},
            WorldChunkCoord{coord+ChunkOffset{ 0,-1}},
            WorldChunkCoord{coord+ChunkOffset{-1,+1}},
            WorldChunkCoord{coord+ChunkOffset{+1,-1}},
        };
    }
    auto neighbour_coords4() const noexcept{
        auto coord = this->coord;
        return Direction_offset2D 
            | views::transform([coord](glm::ivec2 const& o){
                    return WorldChunkCoord{o+coord.raw()};
            }
        );
    }
    auto each_dir_neighbour_chunk_coords() const noexcept{
        auto coord = this->coord;
        return views::enumerate(
            Direction_offset2D 
            | views::transform([coord](glm::ivec2 const& o){ return WorldChunkCoord{o+coord.raw()}; })
        );
    }
};
