#pragma once 
#include "Chunk.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "Geometry.hpp"
#include "Types.h"
#include "cppslop.hpp"


// Tracks where a chunk sits in the generation -> meshing pipeline.
// Everything defaults to false, so a fresh entry is ungenerated and unmeshed.
struct ChunkEntryStatus{
    bool generating   = false; // enqueued for generation
    bool generated    = false; // generation finished, block data is present
    bool meshing      = false; // enqueued for meshing
    bool meshed       = false; // meshed at least once
    bool needsMeshing = false; // mesh is stale and wants (re)building

    bool qualifiesForGeneration() const { return !generated && !generating; }
    bool qualifiesForMeshing()    const { return generated && needsMeshing && !meshing; }

    void beginGeneration() { generating = true; }
    void endGeneration()   { generating = false; generated = true; needsMeshing = true; }

    void beginMeshing()    { meshing = true; }
    void endMeshing()      { meshing = false; meshed = true; needsMeshing = false; }

    void makeDirty()       { needsMeshing = true; }
    bool isDirty() const   { return meshed && needsMeshing; }

    static constexpr ChunkEntryStatus Ungenerated() { return {}; }
    static constexpr ChunkEntryStatus Generating()  { return {.generating = true}; }
    static constexpr ChunkEntryStatus Generated()   { return {.generated = true}; }
    static constexpr ChunkEntryStatus Meshing()     { return {.meshing = true}; }
    static constexpr ChunkEntryStatus Meshed()      { return {.meshed = true}; }
    static constexpr ChunkEntryStatus Dirty()       { return {.meshed = true, .needsMeshing = true}; }
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
    bounding_box(
                WorldFloatPos{toChunkOrigin(chunkCoord).raw()},                  
                WorldFloatPos{toChunkOrigin(chunkCoord).raw()+Chunk::Extents}
    ) {}
    ChunkEntryStatus status;
    MeshRevisionID latest_mesh_revision{0};
    AABB bounding_box; 
    ChunkMetadata metadata;
    std::vector<const Chunk*> neighbours={6, nullptr};
    Chunk block_data;
};
