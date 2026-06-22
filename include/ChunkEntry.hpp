#pragma once 
#include "Breakpoints.hpp"
#include "Chunk.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "Geometry.hpp"
#include "Types.h"
#include "cppslop.hpp"
#include "BitwiseOps.hpp"
#include "glm/vec4.hpp"
#include "Colors01.hpp"

#include "Assertion.hpp"
#include <functional>

FORWARD_DECL_STRUCT(Simulation)
void init_state_transition_logger(Simulation* _sim);

#define GEN_STAGE_LIST \
X(on_queue)\
X(done)

#define MESH_STAGE_LIST \
X(awaiting_generation   )\
X(ready_for_enqueue     )\
X(on_queue              )\
X(done                  )


enum struct GenStage{
#define X(name)\
    name,
    GEN_STAGE_LIST
#undef X
};

struct GenState{
    GenStage stage;
    bool dirty;
    bool isDirty() const noexcept{return dirty;}
    bool isClean() const noexcept{return !dirty;}
};

enum struct MeshStage{
#define X(name)\
    name,
    MESH_STAGE_LIST
#undef X
};

struct MeshState{
    MeshStage stage;
    bool dirty;
    bool isDirty() const noexcept{return dirty;}
    bool isClean() const noexcept{return !dirty;}
};

struct ChunkState{
    ChunkState(WorldChunkCoord _key) :
        coord(_key),
        gen(GenStage::on_queue,false),
        mesh(MeshStage::awaiting_generation, false) {}
    ~ChunkState()=default;
    WorldChunkCoord coord;
    GenState gen;
    MeshState mesh;



};


void transition_logger(const ChunkState& before, const ChunkState& after);
FORWARD_DECL_STRUCT(ChunkEntry)
void gen_enqueue (ChunkEntry* e);
void gen_dequeue (ChunkEntry* e);
void mesh_enqueue(ChunkEntry* e);
void mesh_dequeue(ChunkEntry* e);

void mark_mesh_dirty(ChunkEntry* e);

static constexpr u8 ChunkDebugFillOpacity = 12;
static constexpr f32 ChunkDebugOutlineOpacity = 0.9f;
static constexpr f32 ChunkDebugFillOpacitySpecial = 0.10f;




// An entry that contains information about the currently loaded mesh.
struct MeshEntry{
    std::size_t revisionID; // the mesh revision of the currently loaded mesh
};

// @Brief:
// represents the in memory store of a chunks data.
// A ChunkEntry is created upon request for chunk generation.
// It should be:
// 1. default constructible, probably not movable or copyable. no reason to do either
struct ChunkEntry{

    ChunkEntry(WorldChunkCoord chunkCoord):
    bounding_box(
                toWorldOrigin(chunkCoord).raw(),
                toWorldOrigin(chunkCoord).raw()+Chunk::Extents
    ),
    state(chunkCoord){}

    AABB bounding_box; 
    ChunkMetadata metadata;
    std::vector<std::optional<WorldChunkCoord>> neighbours = std::vector<std::optional<WorldChunkCoord>>(6,std::nullopt);
    Chunk block_data;
    ChunkState state;
    bool is_mesh_dirty()const noexcept{
        return loaded_mesh_revision!=target_mesh_revision;
    }
    bool is_mesh_clean()const noexcept{
        return loaded_mesh_revision==target_mesh_revision;
    }
    void mark_mesh_dirty()noexcept{
        target_mesh_revision++;
    }
    template<typename Fn>
    void state_transition(Fn&& fn) {
        auto before = ChunkState(state);
        std::invoke(fn, this);
        auto after = ChunkState(state);
        transition_logger(before,after);
    }
    MeshRevisionID target_mesh_revision{0};     // The actual underlying data's revision    (++ on MakeDirty())
    MeshRevisionID scheduled_mesh_revision{0};  // newest revision in flight (on queue)     ()
    MeshRevisionID loaded_mesh_revision{0};     // The data on the gpu right now
};
