#pragma once 
#include "ChunkEntry.hpp"
inline glm::vec4 ChunkDebugColor(ChunkEntryStatus status){
    if      (status.isDirty())   return {0.0,0.0,1.0,1.0}; // BLUE
    else if (status.isMeshed())       return {0.0,1.0,0.0,1.0}; // GREEN
    else if (status.isMeshing())      return {1.0,1.0,0.0,1.0}; // YELLOW
    else if (status.isGenerated())    return {0.4,.15,0.0,1.0}; // BROWN
    else if (status.isGenerating())   return {1.0,0.5,0.0,1.0}; // ORANGE
    else /* not generated, but that should be handled elsewhere*/   return {1.0,0.0,0.0,1.0}; // RED
}
