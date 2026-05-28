#pragma once 
#include "Chunk.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "Geometry.hpp"
#include "Types.h"
#include "cppslop.hpp"
#include "Bitwise.hpp"
#include "glm/vec4.hpp"


struct ChunkState{
    bool ungenerated        {false}; // currently enqueued for generation 
    bool isGenerating       {false};
    bool finishedGenerating {false};
    bool isMeshing          {false};
    bool finishedMeshing    {false};
    bool meshIsDirty        {false};


    bool special            {false};
    glm::vec4 dbg_color{};
    constexpr bool operator==(const ChunkState& rhs) const noexcept = default;
};
struct ChunkEntryStatus{
public:
    ChunkEntryStatus()=default;
    ~ChunkEntryStatus()=default;

    ChunkState state{ungenerated};

    // for debugging

    bool qualifiesForGeneration() const {
        return state == ungenerated;
    }
    bool qualifiesForMeshing()const {
        return state == generationFinished; 
    }

    bool isMeshing()const{
        return state.isMeshing;
    }

    bool isGenerating()const{
        return state.isGenerating;
    }

    bool isCleanMeshed()const{
        return state.finishedMeshing && state.meshIsDirty;
    }

    bool isDirtyMeshed()const{
        return state.finishedMeshing && state.meshIsDirty;
    }

    bool isSpecial()const {
        return state.special;
    }

    bool isGenerated()const {
        return state.isGenerating;
    }

    void setSpecial(){
        state.special=true;
    }
    void unsetSpecial(){
         state.special=false;
    }

    void beginGeneration(){
        state = generationInProgress;
    }

    void endGeneration(){
        state = generationFinished;
    }

    void beginMeshing(){
        state = meshingInProgress;
    }

    void endMeshing(){
        state = cleanMeshed;
    }

    void makeDirty(){
        state = dirtyMeshed;
    }


    const glm::vec4& dbg_color()const{
        if (isSpecial()){
            return SpecialColor;
        }
        return state.dbg_color;
    }

    static constexpr glm::vec4 SpecialColor{1, 0,1,1};
    static constexpr glm::vec4 UnGeneratedColor{1, 1,1,1};
    static constexpr ChunkState ungenerated{
            .ungenerated        = true,

            .dbg_color={1.0,0.0,0.0,1.0},
    };
    static constexpr ChunkState generationInProgress={
            .ungenerated        = false,
            .isGenerating       = true,

            .dbg_color={1.0,0.5,0.0,1.0}, // orange
    };
    static constexpr ChunkState generationFinished{
            .ungenerated		= false,
            .isGenerating		= false,
            .finishedGenerating	= true,

            .dbg_color = {0.4,.15,0.0,1.0}, // BROWN
    }; 
    static constexpr ChunkState meshingInProgress{
            .ungenerated		= false,
            .isGenerating		= false,
            .finishedGenerating	= true,
            .isMeshing		    = true,

            .dbg_color={1.0,1.0,0.0,1.0}, // YELLOW
    };
    static constexpr ChunkState cleanMeshed{
            .ungenerated		= false,
            .isGenerating		= false,
            .finishedGenerating	= true,
            .isMeshing		    = false,
            .finishedMeshing	= true,
            .meshIsDirty		= false,

            .dbg_color={0.0,1.0,0.0,1.0}, // GREEN
    }; 
    static constexpr ChunkState dirtyMeshed{
            .ungenerated		= false,
            .isGenerating		= false,
            .finishedGenerating	= true,
            .isMeshing		    = false,
            .finishedMeshing	= true,
            .meshIsDirty		= true,
            
            .dbg_color={0.0,0.0,1.0,1.0}, // BLUE
    }; 
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
