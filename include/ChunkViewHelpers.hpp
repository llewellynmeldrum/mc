#pragma once 
#include "ChunkView.hpp"
constexpr inline const auto is_all_air(ChunkBlockView chunk_view) {
    for (const auto& block: chunk_view){
        if (!block.is_air()){
            return false;
        }
    }
    return true;
}
