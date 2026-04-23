
#include "DEBUG.hpp"

#include "Chunk.hpp"
#include "glmWrapper.hpp"

struct World{
    World()= default;
    ~World()= default;
    std::unordered_map<ChunkCoordinate,Chunk,ChunkCoordHasher> chunkMap;
    Chunk& getChunk(vec3 world_pos);
    std::vector<ChunkView> getChunksInRadius(ChunkCoordinate origin, u32 radius){
        //BUG: for now, just return all chunks until generation is automated
        std::vector<ChunkView> res;
        for (const auto& [coord, chunk]: chunkMap){
            res.emplace_back(coord, std::make_unique<Chunk>(chunk));
        }
        return res;
    }
    Chunk& getChunk(ChunkCoordinate chunk_pos);
    void genChunk(const ChunkCoordinate& chunk_pos){
        chunkMap.insert({chunk_pos,Chunk()});
    }
};

