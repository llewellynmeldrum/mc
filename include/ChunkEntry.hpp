#pragma once 
#include "Chunk.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "Geometry.hpp"
#include "Types.h"
struct ChunkEntryStatus{
public:
    bool qualifiesForGeneration()const {
        return !generated && !on_gen_job_queue;
    }
    bool qualifiesForMeshing()const {
        return generated && needs_meshing && !on_mesh_job_queue;
    }

    void beginGeneration(){
        on_gen_job_queue = 1;
    }
    void endGeneration(){
        on_gen_job_queue = 0;  // removed from job queue 
        generated = 1;          // marked as generated (never unmarked)
        needs_meshing = 1;      // marked as needs_meshing (unmarked on mesh finish, marked again by the 
                                // camera if the player moves across a chunk boundary or a block boundary)
    }

    void beginMeshing(){
        on_mesh_job_queue = 1;
    }
    void endMeshing(){
        needs_meshing = 0;
        on_mesh_job_queue = 0;
        meshed = 1;
    }

    void makeDirty(){
        needs_meshing = 1;
    }
    bool isGenerated()const{return generated;}
    bool isGenerating()const{return on_gen_job_queue;}
    bool isMeshed()const{return meshed;}
    bool isMeshing()const{return on_mesh_job_queue;}

private:
    u32 on_gen_job_queue:      1 = 0; // currently enqueued for generation
    u32 generated:             2 = 0; // Exited gen queue, has block data

    u32 on_mesh_job_queue:     3 = 0; // Currently enqueued for meshing
    u32 meshed:                4 = 0; 
    u32 needs_meshing:         5 = 0;
    
};
// @Brief:
// represents the in memory store of a chunks data.
// A ChunkEntry is created upon request for chunk generation.
// It should be:
// 1. default constructible, probably not movable or copyable. no reason to do either
struct ChunkEntry{
    inline void requestMeshRegen(){
        status.makeDirty();
    };
    ChunkEntry(WorldChunkCoord chunkCoord):
    bounding_box(toWorldBlockPos(chunkCoord),                   // [0,0,0]
                (toWorldBlockPos(chunkCoord)+Chunk::Extents))   // [cxwidth,cheight,czwidth]
    {}
    ChunkEntryStatus status;
    MeshRevisionID latest_mesh_revision{0};
    AABB bounding_box; 
    ChunkMetadata metadata;
    std::vector<const Chunk*> neighbours={6, nullptr};
    Chunk block_data;
};
