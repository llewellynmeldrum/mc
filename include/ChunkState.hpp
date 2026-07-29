#pragma once 
#include "CoordTypes.hpp"
#include "cppslop.hpp"
FORWARD_DECL_STRUCT(Engine)

#define PIPELINE_STATE_LIST \
X(pending           )\
X(ready_for_enqueue )\
X(on_queue          )\
X(done              )

enum struct PipelineState{
    #define X(v) v,
    PIPELINE_STATE_LIST
    #undef X
};


struct ChunkState{
    WorldChunkCoord coord;
    PipelineState gen{PipelineState::ready_for_enqueue};
    PipelineState lighting{PipelineState::pending};
    PipelineState mesh{PipelineState::pending};
};


void transition_logger(const ChunkState& before, const ChunkState& after);
void gen_enqueue (ChunkState* e);
void gen_dequeue (ChunkState* e);
void delete_gen(ChunkState* e);

void mesh_enqueue(ChunkState* e);
void mesh_dequeue(ChunkState* e);
void delete_mesh(ChunkState* e);
void pend_mesh(ChunkState* e) ;

void lighting_enqueue(ChunkState* e);
void lighting_dequeue(ChunkState* e);
void delete_lighting(ChunkState* e);




