#include "WorldGen_NoiseGeneration.hpp"
#include "WorldGen_Config.hpp"

ArrayList2D<NoiseParams> generate_chunk_terrain_noise(const GenConfig& cfg, WorldBlockPos chunk_origin){
    ArrayList2D<NoiseParams> res(ChunkInfo::Extents2D);
    auto to_world = [chunk_origin](i32 cx, i32 cz){
        const auto& [wx_offset, _,wz_offset] = chunk_origin;
        return std::make_pair(cx + wx_offset, cz+wz_offset);
    };
    ForEachInRangeEx({0,0},ChunkInfo::Extents2D,[&](i32 cx, i32 cz){
        auto [wx,wz] = to_world(cx,cz);
        res[cx,cz] = cfg.noise.sample_each(wx,wz);
    });
    return res;
}
