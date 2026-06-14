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

#include "Assertion.hpp"

static constexpr f32 ChunkDebugFillOpacity = 0.05f;
static constexpr f32 ChunkDebugOutlineOpacity = 0.9f;
static constexpr f32 ChunkDebugFillOpacitySpecial = 0.10f;

#define GEN_STATE_LIST \
X(OnGenerationQueue)\
X(FinishedGeneration)\
X(DirtyGen)

#define MESH_STATE_LIST \
X(NoMesh)\
X(OnMeshQueue)\
X(CleanMeshed)\
X(DirtyMeshed)\

#define X(name)\
    name,

enum struct ChunkGenState{
    GEN_STATE_LIST
};
enum struct ChunkMeshState{
    MESH_STATE_LIST
};

#undef X

#define state_color_match(Enum, name, col) case Enum :: name: return col(ChunkDebugFillOpacity); break;

inline glm::vec4 MeshDebugColor(ChunkMeshState state){
    using namespace Color01;
    switch (state){
        state_color_match(ChunkMeshState, NoMesh, GREY_50_a)
        state_color_match(ChunkMeshState, OnMeshQueue, ORANGE_a)
        state_color_match(ChunkMeshState, CleanMeshed, GREEN_a)
        state_color_match(ChunkMeshState, DirtyMeshed, RED_a)
    }
    return {};
}
inline glm::vec4 GenDebugColor(std::nullopt_t nullopt){
    using namespace Color01;
    return GREY_50_a(ChunkDebugFillOpacity);
}
inline glm::vec4 GenDebugColor(ChunkGenState state){
    using namespace Color01;
    switch (state){
        state_color_match(ChunkGenState, OnGenerationQueue, ORANGE_a)
        state_color_match(ChunkGenState, FinishedGeneration, GREEN_a)
        state_color_match(ChunkGenState, DirtyGen, RED_a)
    }
    return {};
}
#undef state_color_match

inline glm::vec4 GenDebugOutlineColor(std::nullopt_t nullopt) { 
    glm::vec3 rgb = static_cast<glm::vec3>(GenDebugColor(nullopt));
    return {rgb,ChunkDebugOutlineOpacity};
}
inline glm::vec4 GenDebugOutlineColor(ChunkGenState state) { 
    glm::vec3 rgb = static_cast<glm::vec3>(GenDebugColor(state));
    return {rgb,ChunkDebugOutlineOpacity};
}
inline glm::vec4 MeshDebugOutlineColor(ChunkMeshState state) { 
    glm::vec3 rgb = static_cast<glm::vec3>(MeshDebugColor(state));
    return {rgb,ChunkDebugOutlineOpacity};
}

#define state_set_str(Enum, str_identifier, name) case Enum :: name: str_identifier=#name; break;
#define X(name) state_set_str(ChunkGenState, str, name)
template<>
struct std::formatter<ChunkGenState>{
    constexpr auto parse(std::format_parse_context& ctx) {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw std::format_error("Invalid format specifier for Point.");
        }
        return it;
    }
    auto format(ChunkGenState s, std::format_context& ctx) const{
        std::string_view str = "???ChunkDataState???";
        switch (s){
            GEN_STATE_LIST
        }
        return format_to(ctx.out(), "{}", str);
    }
};

#undef X
#define X(name) state_set_str(ChunkMeshState, str, name)
template<>
struct std::formatter<ChunkMeshState>{
    inline constexpr auto parse(std::format_parse_context& ctx) {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw std::format_error("Invalid format specifier for Point.");
        }
        return it;
    }
    inline auto format(ChunkMeshState s, std::format_context& ctx) const{
        std::string_view str = "???ChunkMeshState???";
        switch (s){
            MESH_STATE_LIST 
        }
        return format_to(ctx.out(), "{}", str);
    }
};
#undef X
#undef state_set_str

// 
// makeStateTransition(from,to)
// makeStateTransition<ChunkMeshState::OnMeshQueue>()
//
// void makeStateTransition

using GenStateTransition = std::pair<ChunkGenState,ChunkGenState>;
inline constexpr std::array ValidGenStateTransitions{
    GenStateTransition{ChunkGenState::OnGenerationQueue,ChunkGenState::FinishedGeneration},
    GenStateTransition{ChunkGenState::FinishedGeneration,ChunkGenState::DirtyGen},
    GenStateTransition{ChunkGenState::DirtyGen,ChunkGenState::OnGenerationQueue},
};

using MeshStateTransition = std::pair<ChunkMeshState,ChunkMeshState>;
inline constexpr std::array ValidMeshStateTransitions{
    MeshStateTransition{ChunkMeshState::NoMesh, ChunkMeshState::OnMeshQueue},
    MeshStateTransition{ChunkMeshState::OnMeshQueue, ChunkMeshState::CleanMeshed},
    MeshStateTransition{ChunkMeshState::OnMeshQueue, ChunkMeshState::DirtyMeshed},

    MeshStateTransition{ChunkMeshState::CleanMeshed, ChunkMeshState::DirtyMeshed},
    MeshStateTransition{ChunkMeshState::DirtyMeshed, ChunkMeshState::OnMeshQueue},
};

inline consteval bool isStateTransitionValid(ChunkMeshState from, ChunkMeshState to){
    for (const auto& [valid_src,valid_dst]: ValidMeshStateTransitions){
        if (from==valid_src&& to==valid_dst){
            return true;
        }
    }
    return false;
}

inline consteval bool isStateTransitionValid(ChunkGenState from, ChunkGenState to){
    for (const auto& [valid_src,valid_dst]: ValidGenStateTransitions){
        if (from==valid_src&& to==valid_dst){
            return true;
        }
    }
    return false;
}

template<auto from, auto to>
    requires (isStateTransitionValid(from,to))
inline void makeStateTransition(auto& cur){
    assert_eq(cur,from);
    cur = to;
}


struct ChunkState{
public:
    // NOTE: 
    // try to keep all of the state updates localized to Simulation.cpp, 
    // so all the flow/lifecycle updates can be seen from there. This emphasizes that they all 
    // happen synchronously, i.e on the render thread (worker threads cannot update state)
    WorldChunkCoord key;
    MeshRevisionID goal_meshRevisionID{0};

    ChunkState()=default;
    ~ChunkState()=default;
    // what if each mesh had a 'revisionID', which was set each time we generate the mesh.
    // It was incremented each time we make the chunk dirty?

    ChunkMeshState mesh{ChunkMeshState::NoMesh};
    ChunkGenState gen{ChunkGenState::OnGenerationQueue};

    inline bool qualifiesForMeshing() const{
        return gen == ChunkGenState::FinishedGeneration && 
              (mesh == ChunkMeshState::NoMesh ||
               mesh == ChunkMeshState::DirtyMeshed);
    }

    inline bool needsRegeneration() const{
        return gen == ChunkGenState::DirtyGen;
    }
    inline bool isCleanMeshed() const{
        return mesh == ChunkMeshState::CleanMeshed;
    }
    inline bool isOnGenQueue() const{
        return gen == ChunkGenState::OnGenerationQueue;
    }
    inline bool isOnMeshQueue() const{
        return mesh == ChunkMeshState::OnMeshQueue;
    }



    inline void logDirtyGenEnqueue(){ 
        // for generated chunks which we want to regenerate (for debug work on world gen)
        makeStateTransition<ChunkGenState::DirtyGen, ChunkGenState::OnGenerationQueue>(gen);
    }

    inline void logNewGenEnqueue()=delete;
        // no state transition, since ChunkGenState is OnGenerationQueue by default. Just here for completeness

    inline void logGenDequeue(){ 
        makeStateTransition<ChunkGenState::OnGenerationQueue, ChunkGenState::FinishedGeneration>(gen);
    }




    inline void logNewMeshEnqueue(){
        assert(gen==ChunkGenState::FinishedGeneration);
        makeStateTransition<ChunkMeshState::NoMesh,ChunkMeshState::OnMeshQueue>(mesh);
    }

    inline void logDirtyMeshEnqueue(){
        assert(gen==ChunkGenState::FinishedGeneration);
        makeStateTransition<ChunkMeshState::DirtyMeshed,ChunkMeshState::OnMeshQueue>(mesh);
    }


    inline bool makeDirtyIfMeshed(){
        if (mesh==ChunkMeshState::CleanMeshed){
            markMeshAsDirty();
            return true;
        }
        return false;
    };
    inline void logDirtyMeshDequeue(){ 
        makeStateTransition<ChunkMeshState::OnMeshQueue,ChunkMeshState::DirtyMeshed>(mesh);
    }
    inline void logCleanMeshDequeue(){ 
        makeStateTransition<ChunkMeshState::OnMeshQueue,ChunkMeshState::CleanMeshed>(mesh);
    }
    inline void markMeshAsDirty(){
        makeStateTransition<ChunkMeshState::CleanMeshed,ChunkMeshState::DirtyMeshed>(mesh);
        goal_meshRevisionID++;
    };
    
    inline void markDirtyGen(){
        makeStateTransition<ChunkGenState::FinishedGeneration, ChunkGenState::DirtyGen>(gen);
    };
};

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
