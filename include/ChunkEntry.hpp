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

FORWARD_DECL_STRUCT(Engine)

#define GEN_STATE_LIST \
X(on_queue)\
X(done)

#define MESH_STATE_LIST \
X(awaiting_generation   )\
X(ready_for_enqueue     )\
X(on_queue              )\
X(done                  )


enum struct GenState{
#define X(name)\
    name,
    GEN_STATE_LIST
#undef X
};


enum struct MeshState{
#define X(name)\
    name,
    MESH_STATE_LIST
#undef X
};


struct ChunkState{
    ChunkState(WorldChunkCoord _key) : coord(_key){}
    ~ChunkState()=default;
    WorldChunkCoord coord;
    GenState gen{};
    MeshState mesh{};
};


void transition_logger(const ChunkState& before, const ChunkState& after);
void gen_enqueue (ChunkState* e);
void gen_dequeue (ChunkState* e);
void mesh_enqueue(ChunkState* e);
void mesh_dequeue(ChunkState* e);
void delete_mesh(ChunkState* e);






// @Brief:
// represents the in memory store of a chunks data.
// A ChunkEntry is created upon request for chunk generation.
// It should be:
// 1. default constructible, probably not movable or copyable. no reason to do either
struct ChunkEntry{

    ChunkEntry(WorldChunkCoord chunkCoord):
    bounding_box(
                toWorldOrigin(chunkCoord).raw(),
                toWorldOrigin(chunkCoord).raw()+ChunkInfo::Extents3D
    ),
    state(chunkCoord){}

#ifdef CHUNK_NOISE_DEBUG
    PerColumnDebugStore<NoiseParams> noise{};
#endif 
    AABB bounding_box; 
    std::vector<std::optional<WorldChunkCoord>> neighbours = std::vector<std::optional<WorldChunkCoord>>(N_NEIGHBOURS,std::nullopt);
    ChunkStore block_data;
    ChunkState state;
    bool is_mesh_dirty()const noexcept{
        return loaded_mesh_revision!=target_mesh_revision;
    }
    bool is_mesh_clean()const noexcept{
        return loaded_mesh_revision==target_mesh_revision;
    }
    // WARNING: DO NOT USE THIS FUNCTION BY ITSELF! SHOULD ONLY BE USED FROM DIRECTOR
    void _mark_mesh_dirty(std::string_view reason = "n/a")noexcept{
        log_to_chunk("mesh_endirtying",state.coord, 
                     "Mesh dirtied ({}->{}). Reason:{}",
                     target_mesh_revision,
                     target_mesh_revision+1,
                     reason);
        target_mesh_revision++;
    }
    bool qualifies_for_gen_dequeue() const noexcept{
        return state.gen==GenState::on_queue;
    }
    bool qualifies_for_mesh_enqueue()const noexcept{
        const bool target_is_newer_than_inflight = (target_mesh_revision > inflight_mesh_revision);
        const bool gen_done = state.gen == GenState::done;
        const bool ready = state.mesh==MeshState::ready_for_enqueue;
        const bool dirty_done = state.mesh==MeshState::done && is_mesh_dirty();
        return target_is_newer_than_inflight && gen_done && (ready || dirty_done);
    }
    bool is_candidate_mesh_newer_than_loaded(MeshRevisionID candidate_mesh_revision_id) const noexcept{
        return candidate_mesh_revision_id > loaded_mesh_revision;
    }
    bool is_mesh_on_queue()const noexcept {
        return state.mesh == MeshState::on_queue;
    }
    bool is_gen_dirty() const noexcept{
        return loaded_gen_revision != target_gen_revision;
    }
    bool is_gen_clean() const noexcept{
        return loaded_gen_revision == target_gen_revision;
    }
    bool mark_gen_dirty() noexcept{
        // NOTE: currently unused
        return target_gen_revision++;
    }
    void mark_mesh_deleted(){
        state_transition(delete_mesh);
//        target_mesh_revision = 0;
        inflight_mesh_revision = 0;
        loaded_mesh_revision = 0;
    }
    template<typename Fn>
    void state_transition(Fn&& fn) {
        auto before = ChunkState(state);
        std::invoke(fn, &state);
        auto after = ChunkState(state);
        transition_logger(before,after);
    }
    MeshRevisionID target_mesh_revision{0};     // The actual underlying data's revision    (++ on MakeDirty())
    MeshRevisionID inflight_mesh_revision{0};  // newest revision in flight (on queue)     ()
    MeshRevisionID loaded_mesh_revision{0};     // The data on the gpu right now
    //
    GenRevisionID target_gen_revision{0};     // The actual underlying data's revision    (++ on MakeDirty())
    GenRevisionID scheduled_gen_revision{0};  // newest revision in flight (on queue)     ()
    GenRevisionID loaded_gen_revision{0};     // The data on the gpu right now
};
