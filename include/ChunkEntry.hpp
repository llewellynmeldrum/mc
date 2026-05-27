#pragma once 
#include "Chunk.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "Geometry.hpp"
#include "Types.h"
#include "cppslop.hpp"
#include "Bitwise.hpp"


struct ChunkEntryStatus{
public:
    ChunkEntryStatus()=default;
    ~ChunkEntryStatus()=default;
    constexpr ChunkEntryStatus(u32 _flags):flags(_flags){}
    bool qualifiesForGeneration()const {
        return !isGenerated() && !isGenerating();
    }
    bool qualifiesForMeshing()const {
        return isGenerated() && needsMeshing() && !isMeshing();
    }

    void beginGeneration(){
        SetBit(flags,on_gen_job_queue_offset);
    }

    void endGeneration(){
        UnsetBit(flags, on_gen_job_queue_offset);  // removed from job queue 
        SetBit(flags,generated_offset);          // marked as generated (never unmarked)
        SetBit(flags,needs_meshing_offset);// marked as needs_meshing (unmarked on mesh finish, marked again by the 
    }

    void beginMeshing(){
        SetBit(flags,on_mesh_job_queue_offset);
    }

    void endMeshing(){
        UnsetBit(flags,needs_meshing_offset);
        UnsetBit(flags,on_mesh_job_queue_offset);
        SetBit(flags,meshed_offset);
    }

    void makeDirty(){
        SetBit(flags,needs_meshing_offset);
    }

    constexpr bool isCleanMeshed()const{return isMeshed() && isClean();}
    bool isDirty()const{return GetBit(flags,meshed_offset) && GetBit(flags,needs_meshing_offset);}

    static constexpr ChunkEntryStatus Dirty(){ return       {0b110000}; }
    static constexpr ChunkEntryStatus Meshed(){ return      {0b010000}; }
    static constexpr ChunkEntryStatus Meshing(){ return     {0b001000}; }
    static constexpr ChunkEntryStatus Generated(){ return   {0b000100}; }
    static constexpr ChunkEntryStatus Generating(){ return  {0b000010}; }
    static constexpr ChunkEntryStatus Ungenerated(){ return {0b000001}; }


    constexpr bool isGenerating()const{return GetBit(flags,on_gen_job_queue_offset);}
    constexpr bool isGenerated()const{return GetBit(flags,generated_offset);}
    constexpr bool isMeshing()const{return GetBit(flags,on_mesh_job_queue_offset);}
    constexpr bool isMeshed()const{return GetBit(flags,meshed_offset);}
    constexpr bool needsMeshing()const{return GetBit(flags,needs_meshing_offset);}
    constexpr bool isClean()const{return GetBit(flags,needs_meshing_offset)==0;}

    u32 flags{}; 
private:
    static constexpr u32 on_gen_job_queue_offset     = 1; // currently enqueued for generation 
    static constexpr u32 generated_offset            = 2; // Exited gen queue, has block data
    static constexpr u32 on_mesh_job_queue_offset    = 3; // Currently enqueued for meshing
    static constexpr u32 meshed_offset               = 4; // finished meshing
    static constexpr u32 needs_meshing_offset        = 5; // mesh is finished but dirty
    
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
