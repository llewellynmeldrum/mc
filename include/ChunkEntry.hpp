#pragma once 
#include "Chunk.hpp"
#include "ChunkHelpers.hpp"
#include "CoordTypes.hpp"
#include "Geometry.hpp"
#include "Types.h"
#include "cppslop.hpp"
#include "Bitwise.hpp"
#include "glm/vec4.hpp"
#include "Colors01.hpp"


static constexpr f32 ChunkDebugFillOpacity = 0.05f;
static constexpr f32 ChunkDebugFillOpacitySpecial = 0.10f;
struct ChunkState{
    bool ungenerated        {false}; // currently enqueued for generation 
    bool isGenerating       {false};
    bool finishedGenerating {false};
    bool isMeshing          {false};
    bool finishedMeshing    {false};
    bool meshIsDirty        {false};

    static constexpr glm::vec4 SpecialColor = Color01::PURPLE_a(ChunkDebugFillOpacity);
    bool special            {false};
    glm::vec4 dbg_color{};
    glm::vec4 DebugColor() const noexcept{
        if (special){
            return {dbg_color.r,dbg_color.g,dbg_color.b,ChunkDebugFillOpacitySpecial};
        }
        //return Color01::TRANSPARENT;
        return dbg_color;
    }
    glm::vec4 DebugOutlineColor()const{ 
        glm::vec3 rgb = DebugColor();
        return {rgb,0.9};
    }
    // comparator disregards `special`, so it doesnt influence decisions on adding to queues and such
    inline constexpr bool operator==(const ChunkState& rhs) const noexcept{
        return  ungenerated        ==rhs.ungenerated        &&
                isGenerating       ==rhs.isGenerating       &&
                finishedGenerating ==rhs.finishedGenerating &&
                isMeshing          ==rhs.isMeshing          &&
                finishedMeshing    ==rhs.finishedMeshing    &&
                meshIsDirty        ==rhs.meshIsDirty        ;
    }
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
        return state.special || (state == generationFinished || state == dirtyMeshed); 
    }

    bool isMeshing()const{
        return state.isMeshing;
    }

    bool isGenerating()const{
        return state.isGenerating;
    }

    bool isCleanMeshed()const{
        return state==cleanMeshed;
    }

    bool isDirtyMeshed()const{
        return state.finishedMeshing && state.meshIsDirty;
    }

    bool inFrustum()const {
        return state.special;
    }

    bool isGenerated()const {
        return state.isGenerating;
    }

    void markInFrustum(){
        state.special=true;
    }
    void markOutsideFrustum(){
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
    void reset(){
        state = ungenerated;
    }


    glm::vec4 DebugColor()const{ return state.DebugColor(); }
    glm::vec4 DebugOutlineColor()const{ return state.DebugOutlineColor(); }

    constexpr std::string str(){
        std::string s0{};
        if (state==ChunkEntryStatus::dirtyMeshed){s0= "dirtyMeshed";}
        else if (state==ChunkEntryStatus::cleanMeshed){s0= "cleanMeshed";}
        else if (state==ChunkEntryStatus::meshingInProgress){s0= "meshingInProgress";}
        else if (state==ChunkEntryStatus::generationFinished){s0= "generationFinished";}
        else if (state==ChunkEntryStatus::generationInProgress){s0= "generationInProgress";}
        else if (state==ChunkEntryStatus::ungenerated){s0= "ungenerated";}
        if (state.special){
            s0+=" (special) ";
        }
        return s0;
    }
    static constexpr glm::vec4 UnGeneratedColor = Color01::RED_a(ChunkDebugFillOpacity);
    static constexpr ChunkState ungenerated{
            .ungenerated        = true,

            .dbg_color=UnGeneratedColor,
    };
    static constexpr ChunkState generationInProgress={
            .ungenerated        = false,
            .isGenerating       = true,

            .dbg_color=Color01::ORANGE_a(ChunkDebugFillOpacity),
    };
    static constexpr ChunkState generationFinished{
            .ungenerated		= false,
            .isGenerating		= false,
            .finishedGenerating	= true,

            .dbg_color = Color01::BROWN_a(ChunkDebugFillOpacity),
    }; 
    static constexpr ChunkState meshingInProgress{
            .ungenerated		= false,
            .isGenerating		= false,
            .finishedGenerating	= true,
            .isMeshing		    = true,

            .dbg_color=Color01::YELLOW_a(ChunkDebugFillOpacity),
    };
    static constexpr ChunkState cleanMeshed{
            .ungenerated		= false,
            .isGenerating		= false,
            .finishedGenerating	= true,
            .isMeshing		    = false,
            .finishedMeshing	= true,
            .meshIsDirty		= false,

            // NOTE: Clean chunks are invisible at the moment!
            .dbg_color=Color01::GREEN_a(ChunkDebugFillOpacity),
    }; 
    static constexpr ChunkState dirtyMeshed{
            .ungenerated		= false,
            .isGenerating		= false,
            .finishedGenerating	= true,
            .isMeshing		    = false,
            .finishedMeshing	= true,
            .meshIsDirty		= true,
            
            .dbg_color=Color01::BLUE_a(ChunkDebugFillOpacity),
    }; 
};

// @Brief:
// represents the in memory store of a chunks data.
// A ChunkEntry is created upon request for chunk generation.
// It should be:
// 1. default constructible, probably not movable or copyable. no reason to do either
struct ChunkEntry{
    inline void makeDirty(){
        status.makeDirty();
    };
    inline void requestWorldRegen(){
        status.reset();
    };
    ChunkEntry(WorldChunkCoord chunkCoord):
    bounding_box(
                WorldFloatPos{toWorldOrigin(chunkCoord).raw()},                  
                WorldFloatPos{toWorldOrigin(chunkCoord).raw()+Chunk::Extents}
    ) {}
    ChunkEntryStatus status;
    MeshRevisionID latest_mesh_revision{0};
    AABB bounding_box; 
    ChunkMetadata metadata;
    std::vector<const Chunk*> neighbours={6, nullptr};
    Chunk block_data;
};
