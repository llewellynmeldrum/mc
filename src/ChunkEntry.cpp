#include "ChunkEntry.hpp"
#include "CoordTypes.hpp"
#include "DebugUI.hpp"
using ChunkGenState::DirtyGen;
using ChunkGenState::OnGenerationQueue;
using ChunkGenState::FinishedGeneration;

using ChunkMeshState::NoMesh;
using ChunkMeshState::OnMeshQueue;
using ChunkMeshState::CleanMeshed;
using ChunkMeshState::DirtyMeshed;


#include "Simulation.hpp"
static Simulation* sim{};

void init_state_transition_logger(Simulation* _sim){
    sim=_sim;
}

template<ChunkGenState A, ChunkGenState B>
void state_transition_wrapper(WorldChunkCoord coord, ChunkGenState& cur){
    auto str = makeStateTransition<A, B,true>(cur);
    log_to_chunk(coord,"G: {}",str);
}

template<ChunkMeshState A, ChunkMeshState B>
void state_transition_wrapper(WorldChunkCoord coord, ChunkMeshState& cur){
    auto str = makeStateTransition<A, B,true>(cur);
    log_to_chunk(coord,"M: {}",str);
}


void ChunkState::logDirtyGenEnqueue(){ 
    // for generated chunks which we want to regenerate (for debug work on world gen)
    state_transition_wrapper<DirtyGen, OnGenerationQueue>(key,gen);
}

 void ChunkState::logGenDequeue(){ 
    state_transition_wrapper<OnGenerationQueue, FinishedGeneration>(key,gen);
}




 void ChunkState::logNewMeshEnqueue(){
    assert(gen==FinishedGeneration);
    state_transition_wrapper<NoMesh,OnMeshQueue>(key,mesh);
}

 void ChunkState::logDirtyMeshEnqueue(){
    assert(gen==FinishedGeneration);
    state_transition_wrapper<DirtyMeshed,OnMeshQueue>(key,mesh);
}


 bool ChunkState::makeDirtyIfMeshed(){
    if (mesh==CleanMeshed){
        markMeshAsDirty();
        return true;
    }
    return false;
};
 void ChunkState::logDirtyMeshDequeue(){ 
    state_transition_wrapper<OnMeshQueue,DirtyMeshed>(key,mesh);
}
 void ChunkState::logCleanMeshDequeue(){ 
    state_transition_wrapper<OnMeshQueue,CleanMeshed>(key,mesh);
}
 void ChunkState::markMeshAsDirty(){
    state_transition_wrapper<CleanMeshed,DirtyMeshed>(key,mesh);
    goal_meshRevisionID++;
};

 void ChunkState::markDirtyGen(){
    state_transition_wrapper<FinishedGeneration, DirtyGen>(key,gen);
};
