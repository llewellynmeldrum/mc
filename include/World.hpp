#include "Chunk.hpp"
#include "ChunkConcurrency.hpp"
#include "ChunkMap.hpp"
#include "CommonUtils.hpp"
#include "CoordTypes.hpp"
#include "glmWrapper.hpp"
#include <algorithm>
#include <memory>
#include <print>

#include "Camera.hpp"
    inline std::size_t distcount{0};
    inline f32 dist_sum{0};
struct World {
    World() = default;
    ~World() = default;
    World(World const&) = delete;
    World& operator=(World const&) = delete;
    World(World&&) = delete;
    World& operator=(World&&) = delete;

    inline void setupWorld(){
        chunkMap.setup_chunk_map();
    }

    ChunkMap chunkMap;
    GenConfig genConfig;

    static constexpr i64 NUM_VERTICAL_CHUNKS = 16;

    inline std::vector<std::pair<bool, WorldChunkCoord>> chunksInRadius(WorldChunkCoord chunkCoord, i32 dist) {
        const size_t nChunksInRadius = std::pow(2*dist+1,3);
        std::vector<std::pair<bool,WorldChunkCoord>> candidates;
        candidates.reserve(nChunksInRadius);

        auto add = [this, &candidates](i32 x, i32 y, i32 z){
            const auto key = WorldChunkCoord{x,y,z}; // dont you have to 
            candidates.emplace_back(chunkMap.has_entry(key),key);
        };

        const i32& oy = chunkCoord.y;

        i32 minY = oy-dist;
        i32 maxY = oy+dist;
        for (i32 y = maxY; y>=minY; y--){
            i32 x{chunkCoord.x}, z{chunkCoord.z};
            add(x,y,z); // center point
            for (i32 r = 1; r<= dist; r++){
                const i32 r2 = 2*r;
                add(--x,y,z); // move out of the centre point
                for (int j = 0; j<r2 - 1;j++)    add(x,y,++z); // traverse the remaining (-X) edge
                for (int j = 0; j<r2 ; j++)     add(++x,y,z);  // traverse the whole     (Z+) edge
                for (int j = 0; j<r2 ; j++)     add(x,y,--z);  // traverse the whole     (+X) edge
                for (int j = 0; j<r2 ; j++)     add(--x,y,z);  // traverse the whole     (+X) edge
            }
        }
        return candidates;
    }
    inline std::vector<WorldChunkCoord> 
    meshReadyChunksInRad(WorldChunkCoord chunkCoord, i32 dist, i32 maxChunks=0) {
        const size_t nChunks= maxChunks==0 ? std::pow(2*dist+1,3) : maxChunks;
        std::vector<WorldChunkCoord> candidates;
        candidates.reserve(nChunks);

        auto add = [this, &candidates](i32 x, i32 y, i32 z){
            const auto key = WorldChunkCoord{x,y,z}; // dont you have to 
            auto entry = chunkMap.try_get_entry(key);
            if (entry.has_value()){
                if ((*entry)->status.qualifiesForMeshing()){
                    candidates.emplace_back(key);
                }
            }
        };

        const i32& oy = chunkCoord.y;

        // TODO: refactor, pull this out into an iota like function maybe?
        i32 minY = oy-dist;
        i32 maxY = oy+dist;
        // this genuinely took like 2 hours to make, holy christ man i hate geometric shit
        // Anyways this iterates in 'expanding spirals', per horizontal slice, such that we get 
        // chunks for meshing in a reasonable-ish order, (close->far, +Y first)
        for (i32 y = maxY; y>=minY; y--){
            i32 x{chunkCoord.x}, z{chunkCoord.z};
            add(x,y,z); // center point
            for (i32 r = 1; r<= dist; r++){
                const i32 r2 = 2*r;
                add(--x,y,z); // move out of the centre point
                for (int j = 0; j<r2 - 1;j++)    add(x,y,++z); // traverse the remaining (-X) edge
                for (int j = 0; j<r2 ; j++)     add(++x,y,z);  // traverse the whole     (Z+) edge
                for (int j = 0; j<r2 ; j++)     add(x,y,--z);  // traverse the whole     (+X) edge
                for (int j = 0; j<r2 ; j++)     add(--x,y,z);  // traverse the whole     (+X) edge
            }
            if (candidates.size() >= nChunks){
                return candidates;
            }
        }
        return candidates;
    }

    std::vector<std::pair<Block, Direction>> getNeighbourBlocks(WorldBlockPos world_pos) const;

    Block getBlock(WorldBlockPos world_pos) const;
};
