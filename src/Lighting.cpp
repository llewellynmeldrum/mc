#include <type_traits>

#include "Lighting.hpp"
#include "ChunkNeighbourhood.hpp"
#include "BlockLight.hpp"
#include "CoordIteration.hpp"
#include "PackedLightValue.hpp"
#include "SharedShaderConfig.hpp"
#include "UnpackedLightValue.hpp"
#include "ThreadTracker.hpp"
//NOTE: 
// As a general rule, the lighting propogation follows a few rules:
// -> The center chunk in a neighbourhood (passed in as a job) is the only chunk which may be modified.
//    Changes in neighbouring chunk slices are completely ignored upon upload.
// -> Neighbour chunks may however SEED lighting, i.e a torch on the edge of chunk A and B can contribute light
//    to both chunks, but the light levels themselves within A are only applied when A's job is performed.
//
//
//
static auto is_corner (auto cx, auto cz){
    static constexpr auto const& ext = ChunkInfo::Extents3D;
    // skip corner blocks, we dont store those neighbours nor evaluate them
    return  (cx == -1    && ext.z == cz)
    ||      (cx == ext.x && -1 == cz) 
    ||      (cx == -1    && -1 == cz) 
    ||      (cx == ext.x && ext.z == cz);
};
auto seed_block_light(ChunkNeighbourhood& neighbourhood){
    std::deque<ChunkBlockPos> q;
    static constexpr auto const& ext = ChunkInfo::Extents3D;
    auto lo= glm::ivec2(-1,-1);
    auto hi= glm::ivec2(ext.x,ext.z);
    for_each_xz_inclusive(lo, hi,[&](auto cx, auto cz){
        if (is_corner(cx,cz)) return;
        for (auto cy = 0;  cy < ext.y;  cy++){
            // skip corner blocks, we dont store those neighbours nor evaluate them
            ChunkBlockPos p{cx,cy,cz};
            auto block = neighbourhood.block_at(p);
            auto const& block_light_emission = block.get_emission();
            if (is_in_chunk(p)){
                // Blocks in center chunk may propogate its light, AND be written to.
                if (block_light_emission.is_nonzero()){
                    neighbourhood.set_blocklight(p, block_light_emission);
                    q.emplace_back(p);
                }
            }else {
                // Blocks in neighbour chunks may propogate their light.
                auto const& light = unpack(neighbourhood.light_at(p));
                if (light.can_propogate() || block_light_emission.is_nonzero()){
                    q.emplace_back(p);
                }
            }
        }
    });
    return q;
}

void propogate_block_light(std::deque<ChunkBlockPos>& q, ChunkNeighbourhood& neighbourhood){
    while (!q.empty()){
        auto u = q.front(); q.pop_front();
        const auto u_rgb = unpack_blocklight(neighbourhood.light_at(u));
        for (const auto& v: neighbour_block_coords(u)){
            if (!neighbourhood.is_in_center(v)) continue; // NOTE: lights in neighbour chunks are seeds but are not modified

            auto const& v_block = neighbourhood.block_at(v);
            auto const v_rgb = unpack_blocklight(neighbourhood.light_at(v));
            auto const& v_absorptance_rgb = v_block.absorptance();
            auto resolved_v_rgb = v_rgb;
            if (v_block.is_opaque()) continue;

            for (i32 channel_id = 0; channel_id < 3; channel_id++){
                auto const& u_light = u_rgb.arr[channel_id];
                if (u_light <= 1) continue;
                auto const& v_light = v_rgb.arr[channel_id];
                auto const& v_absorptance = v_absorptance_rgb.arr[channel_id];

                // 1. apply absorptance to the new neighbour
                u8 candidate = std::clamp(u_light - v_absorptance, 0,15);
                resolved_v_rgb.arr[channel_id] = std::max(candidate, v_light);
            }
            if (resolved_v_rgb != v_rgb){
                neighbourhood.set_blocklight(v, resolved_v_rgb);
                q.push_back(v);
            }
        }
    }
}

auto seed_sunlight(ChunkNeighbourhood& neighbourhood){
    // Sunlight seeding is quite different to block seeding: 
    std::deque<ChunkBlockPos> q;
    // for each column, 
    static constexpr auto const& ext = ChunkInfo::Extents3D;

    auto lo= glm::ivec2(-1,-1);
    auto hi= glm::ivec2(ext.x,ext.z);
    for_each_xz_inclusive(lo, hi,[&](auto cx, auto cz){
        if (is_corner(cx,cz)) return;
        // until reaching first opaque block, all values are max.
        // The block right before that is the only one added to the queue. 


        bool in_center = is_in_chunk({cx,0,cz});
        auto ray_intensity  = static_cast<u8>(SUNLIGHT_INTENSITY_MAX);
        for (auto cy = ext.y-1; cy >=1; cy--){
            ChunkBlockPos p{cx,cy,cz};
            ChunkBlockPos p_below{cx,cy-1,cz};
            auto block = neighbourhood.block_at(p);
            auto block_below = neighbourhood.block_at(p_below);
            if (block_below.is_air()){
                continue;
            } else{
                q.push_back(p); // seed the block above the surface
                if (in_center){
                    neighbourhood.set_sunlight(p,ray_intensity);
                }

                auto surf_absorptance = block_below.sunlight_ray_absorptance();
                ray_intensity = std::max(0,ray_intensity - surf_absorptance);

                if (block.is_opaque()){
                    break; // ray cannot directly reach anything below an opaque block
                }
            }
        }
    });
    return q;
}
auto propogate_sunlight(std::deque<ChunkBlockPos>& q, ChunkNeighbourhood& neighbourhood) {
    while (!q.empty()) {
        auto u = q.front();
        q.pop_front();
        const auto u_sunlight = unpack_sunlight(neighbourhood.light_at(u));
        if (u_sunlight <= 1)
            continue;
        for (const auto& v : neighbour_block_coords(u)) {
            if (!neighbourhood.is_in_center(v))
                continue;  // NOTE: lights in neighbour chunks are seeds but are not modified

            auto const& v_block = neighbourhood.block_at(v);
            auto const  v_sunlight = unpack_sunlight(neighbourhood.light_at(v));
            auto const& v_absorptance = v_block.absorptance();
            auto        resolved_v_sunlight = v_sunlight;
            if (v_block.is_opaque())
                continue;

            // 1. apply absorptance to the new neighbour
            u8 candidate = std::clamp(u_sunlight - v_absorptance.r, 0, 15);
            resolved_v_sunlight = std::max(candidate, v_sunlight);
            if (resolved_v_sunlight != v_sunlight) {
                neighbourhood.set_sunlight(v, resolved_v_sunlight);
                q.push_back(v);
            }
        }
    }
}

LightingResult perform_light_work(LightingJob && job){
    LightingResult res{
        .coord = job.coord,
        .rev = job.rev,
        .lights = {},
    };
    res.lights.reset();
    auto const center_coord = job.coord;
    auto& lights = res.lights;
    assert_eq(lights.buf.size(), ChunkInfo::SIZE);

    // Contains a central chunk and 1 block 'slices' of each axis aligned neighbour
    ChunkNeighbourhood neighbourhood{
        center_coord,
        &job.block_data,
        job.neighbour_block_slices,
        &lights,
        job.neighbour_light_slices,
    };


    assert_eq(lights.buf.size(), ChunkInfo::SIZE);
    
    auto blocklight_q = seed_block_light(neighbourhood);
    propogate_block_light(blocklight_q, neighbourhood);

    auto sunlight_q = seed_sunlight(neighbourhood);
    propogate_sunlight(sunlight_q, neighbourhood);


    return res;
}
