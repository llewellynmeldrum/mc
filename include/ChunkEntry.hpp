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

// @brief: 
// A logical 'chunk' gets a ChunkState entry when it is first enqueued into the gen queue.
struct ChunkState{
    ChunkState(WorldChunkCoord _key) :
        coord(_key),
        gen(GenStage::on_queue,false),
        mesh(MeshStage::awaiting_generation, false) {}
    ~ChunkState()=default;
    MeshRevisionID goal_meshRevisionID{};
    WorldChunkCoord coord;
    GenState gen;
    MeshState mesh;

    template<typename Fn>
    void transition(Fn&& fn) {
        std::invoke(fn, this);
    }
    template<typename Fn>
    void transition_if(const Fn& fn) &{
        std::invoke(fn, *this);
    }

    inline void mark_dirty_mesh(){ 
        goal_meshRevisionID++;
        mesh.dirty=true; 
    }
    inline void mark_clean_mesh(MeshRevisionID clean_id){
        assert_eq(clean_id, goal_meshRevisionID);
        mesh.dirty=false; 
    }
    inline void mark_dirty_gen (){
        gen.dirty=true; 
    }
    inline void mark_clean_gen (){
        gen.dirty=false; 
    }
    inline bool ready_for_mesh()const{
        return mesh.isDirty() || mesh.stage == MeshStage::ready_for_enqueue;
    }

};


inline void gen_enqueue(ChunkState* s) {
   1; // zorp
}
inline void gen_dequeue(ChunkState* s) {
    assert(s->gen.stage==GenStage::on_queue);
    assert(s->mesh.stage == MeshStage::awaiting_generation);
    s->mesh.stage = MeshStage::ready_for_enqueue;
    s->gen.stage = GenStage::done;
}

inline void mesh_enqueue(ChunkState* s) {
    assert(s->gen.stage==GenStage::done);
    assert(s->mesh.stage == MeshStage::ready_for_enqueue);
    s->mesh.stage = MeshStage::on_queue;
}

inline void mesh_dequeue(ChunkState* s) {
    assert(s->gen.stage==GenStage::done);
    assert(s->mesh.stage == MeshStage::ready_for_enqueue);
    s->mesh.stage = MeshStage::done;
}
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
    ) {}

    AABB bounding_box; 
    ChunkMetadata metadata;
    std::vector<const Chunk*> neighbours={6, nullptr};
    Chunk block_data;
};
